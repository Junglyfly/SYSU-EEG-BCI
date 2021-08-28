
cdef extern from "stdlib.h":
    void* malloc(int)
    void free(void*)

cdef extern from "Python.h":
    object PyString_FromStringAndSize(char *, int)

cdef extern from "cstyx.h":
    cdef struct cstyx_connection_t:
        pass
    cdef struct cstyx_client_t:
        pass
    cdef struct cstyx_dir_t:
        pass
    cdef struct cstyx_dirent_t:
        pass
    cdef struct cstyx_file_t:
        pass

    cstyx_client_t* cstyx_connect(cstyx_connection_t* c)
    void cstyx_close_client(cstyx_client_t* c)
    cstyx_dir_t* cstyx_read_dir(cstyx_client_t* c, char* p)
    int cstyx_path_isdir(cstyx_client_t* c, char*)
    int cstyx_dirent_isdir(cstyx_dirent_t* e)
    int cstyx_get_dir_length(cstyx_dir_t* d)
    char* cstyx_get_dir_path(cstyx_dir_t* d)
    cstyx_dirent_t* cstyx_get_dirent(cstyx_dir_t* d, int i)
    int cstyx_close_dir(cstyx_dir_t* d)
    char* cstyx_get_cwd(cstyx_client_t* d)
    int cstyx_set_cwd(cstyx_client_t* d, char* path)
    cstyx_file_t* cstyx_get_file(cstyx_client_t* d, char* path)
    cstyx_dirent_t* cstyx_stat(cstyx_file_t* f)
    long cstyx_get_file_len(cstyx_file_t* f)
    int cstyx_open(cstyx_client_t* c, char* path, int mode)
    void cstyx_close(cstyx_file_t* f)
    int cstyx_read(cstyx_file_t* f, char* d, unsigned short* len)
    int cstyx_write(cstyx_file_t* f, char* d, unsigned short* len)
    int cstyx_write_path(cstyx_client_t* f, char* path, char* d, int len)
    long cstyx_get_dirent_mode(cstyx_dirent_t* e)
    char* cstyx_get_dirent_name(cstyx_dirent_t* e)
    unsigned long cstyx_get_dirent_len(cstyx_dirent_t* e)
    void cstyx_delete_dirent(cstyx_dirent_t* d)
    char* cstyx_get_latest_error(int* d)
    int cstyx_read_all_path(cstyx_client_t* c, char* path, char* s, int lim)

cdef extern from "cstyx_local.h":
    cstyx_connection_t* cstyx_get_local_connection()

cdef extern from "cstyx_usb.h":
    cdef struct cstyx_usb_connection_t:
        pass

    cstyx_usb_connection_t* cstyx_get_usb_connection()
    int cstyx_usb_search(cstyx_usb_connection_t* c, unsigned short vid, unsigned short pid)
    void cstyx_delete_usb_connection(cstyx_usb_connection_t* c)

def modeStr(mode):
    bits = ["---", "--x", "-w-", "-wx", "r--", "r-x", "rw-", "rwx"]
    d = "-"
    if mode & 020000000000L:
        d = "d"
    return "%s%s%s%s" % (d, bits[(mode>>6)&7], bits[(mode>>3)&7], bits[mode&7])

class Error:
    def __init__(self,msg="no error",code=0):
        self._msg=msg
        self._code=code

    def __str__(self):
        return "%s (code %d)"%(self._msg,self._code)

class StyxError(Error):
    def __init__(self):
        cdef int i
        m=cstyx_get_latest_error(&i)
        Error.__init__(self,m,i)

cdef class Connection:
    cdef cstyx_connection_t *connection

cdef class LocalConnection(Connection):
    def __init__(self):
        self.connection=cstyx_get_local_connection()

cdef class USBConnection(Connection):
    def __init__(self):
        self.connection=<cstyx_connection_t*>cstyx_get_usb_connection()

    def __del__(self):
        cstyx_delete_usb_connection(<cstyx_usb_connection_t*>self.connection)

    def search(self,vid,pid):
        r=cstyx_usb_search(<cstyx_usb_connection_t*>self.connection,vid,pid)
        if r==0: return False
        elif r==1: return True
        elif r==-1: raise StyxError()

cdef class Client:
    cdef cstyx_client_t* _client

    def __init__(self,c):
        cdef Connection conn
        cdef cstyx_connection_t* connptr
        conn=c
        connptr=conn.connection
        self._client=cstyx_connect(connptr)
        if self._client==NULL:
            raise StyxError()

    def cwd(self):
        s=cstyx_get_cwd(self._client)
        return s

    def cd(self,p):
        self.setcwd(p)
        
    def setcwd(self,p):
        if cstyx_set_cwd(self._client,p)!=0:
            raise StyxError()

    def ls(self,p=''):
        return Directory(self,p)

    def write(self,path,s):
        l=len(s)
        if l>32767: l=32767
        l2=cstyx_write_path(self._client,path,s,l)
        if l2<0:
            raise StyxError()
        return l2

    def readAll(self,path,lim=32767):
        cdef int l2
        cdef char* buf
        if lim>32767: lim=32767
        buf=<char*>malloc(lim)
        if buf==NULL:
            raise Error("out of memory!")
        l2=cstyx_read_all_path(self._client, path, buf, lim)
        if l2<0:
            free(buf)
            raise StyxError()
        s=PyString_FromStringAndSize(buf,l2)
        free(buf)
        return s

cdef class File:
    cdef cstyx_file_t* _fp

    def __del__(self):
        cstyx_close(self._fp)
        
    def stat(self):
        cdef cstyx_dirent_t* e
        e=cstyx_stat(self._fp)
        #if e==0:
        #    raise Error("cannot stat")
        #return Dirent(e)
    
    def open(self):
        if self._isopen: return
        #if cstyx_open(self._fp)!=0:
        #    raise StyxError()

    def read(self,unsigned short l):
        cdef unsigned short l2
        cdef char* buf
        if l>65535: l=65535
        buf=<char*>malloc(l)
        if buf==NULL:
            raise Error("out of memory!")
        l2=l
        if cstyx_read(self._fp, buf, &l2)!=0:
            free(buf)
            raise StyxError()
        l=l2
        s=PyString_FromStringAndSize(buf,l)
        free(buf)
        return s

    def write(self,s):
        cdef unsigned short l
        l=len(s)
        if cstyx_write(self._fp,s,&l)!=0:
            raise StyxError()
        return l

cdef class Directory:
    cdef cstyx_dir_t* _dir

    def __init__(self,client,path=''):
        cdef Client cl
        cl=client
        self._dir=cstyx_read_dir(cl._client,path)
        if self._dir==NULL:
            raise StyxError()

    def __del__(self):
        if self._dir!=NULL:
            cstyx_close_dir(self._dir)

    def __len__(self):
        if self._dir==NULL: return 0
        return cstyx_get_dir_length(self._dir)

    def __getitem__(self,key):
        cdef cstyx_dirent_t* dirent

        if not isinstance(key,int): raise TypeError
        if key<0: key=key+self.__len__()
        if key>=self.__len__() or key<0: raise IndexError
        return Dirent(self,key)

    def path(self):
        cdef char* s
        if self._dir==NULL: return ''
        s=cstyx_get_dir_path(self._dir)
        if s==NULL: return ''
        return s

    def ls(self, l=False):
        if not l:
            print str(self)
        else:
            for de in self:
                print "%s nobody nobody %-8d %s"%(modeStr(de.mode()),de.size(),de.name())

    def names(self):
        l=[]
        for i in self:
            l.append(i.name())
        return l

    def __str__(self):
        return ' '.join(self.names())

cdef class Dirent:
    cdef cstyx_dirent_t* _dirent
    
    def __init__(self, d, key):
        cdef Directory di
        di=d
        self._dirent=cstyx_get_dirent(di._dir,key)
        if self._dirent==NULL:
            raise IndexError

    def __del__(self):
        if self._dirent!=NULL:
            cstyx_delete_dirent(self._dirent)

    def name(self):
        if self._dirent==NULL: return ''
        return cstyx_get_dirent_name(self._dirent)

    def mode(self):
        if self._dirent==NULL: return 0
        return cstyx_get_dirent_mode(self._dirent)

    def size(self):
        if self._dirent==NULL: return 0
        return cstyx_get_dirent_len(self._dirent)

    def __str__(self):
        return "%s : %d bytes : mode=%d"%(self.name(),self.size(),self.mode())

    def __repr__(self):
        return '<Dirent: name = "%s", mode = %d, size = %d>'%(self.name(),self.mode(),self.size())
