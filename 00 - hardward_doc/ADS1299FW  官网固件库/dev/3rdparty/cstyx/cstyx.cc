
#include "cstyx.h"
#include "cstyx.hh"
#include <string.h>
#include <vector>
#include <string>
#include "Directory.hh"

using namespace cstyx;
using namespace std;

EXPORT bool cstyx_close_connection(cstyx_connection_t* c)
{
    if (!c) return false;
    delete (Connection*)c;
    return true;
}

EXPORT bool cstyx_close_client(cstyx_client_t* c)
{
    if (!c) return false;
    delete (Client*)c;
    return true;
}

EXPORT cstyx_dir_t* cstyx_read_dir(cstyx_client_t* c, const char* p)
{
    if (!c) return NULL;
    Directory* d;
    if (p)
        d=new Directory(*(Client*)c,p);
    else
        d=new Directory(*(Client*)c);
    return (cstyx_dir_t*)d;
}

EXPORT int cstyx_path_isdir(cstyx_client_t* c, const char* p)
{
    if (!c) return 0;
    return ((Client*)c)->isdir(p);
}

EXPORT int cstyx_dirent_isdir(cstyx_dirent_t* e)
{
    if (!e) return 0;
    return ((Dirent*)e)->isdir();
}

EXPORT int cstyx_get_dir_length(cstyx_dir_t* d)
{
    if (!d) return -1;
    return ((Directory*)d)->list().size();
}

EXPORT const char* cstyx_get_dir_path(cstyx_dir_t* d)
{
    if (!d) return NULL;
    return ((Directory*)d)->path().c_str();
}

EXPORT cstyx_dirent_t* cstyx_get_dirent(cstyx_dir_t* d, int i)
{
    if (!d) return NULL;
    return (cstyx_dirent_t*)(new Dirent( ((Directory*)d)->list()[i] ) );
}

//! Finish reading a directory
EXPORT void cstyx_close_dir(cstyx_dir_t* d)
{
    if (!d) return;
    delete (Directory*)d;
}

EXPORT long cstyx_get_dirent_mode(cstyx_dirent_t* e)
{
    if (!e) return 0;
    return ((Dirent*)e)->mode();
}

EXPORT const char* cstyx_get_dirent_name(cstyx_dirent_t* e)
{
    if (!e) return 0;
    return ((Dirent*)e)->name().c_str();
}

EXPORT u32 cstyx_get_dirent_len(cstyx_dirent_t* e)
{
    if (!e) return 0;
    return ((Dirent*)e)->len();
}

EXPORT bool cstyx_get_cwd(cstyx_client_t* d, char* cwd, unsigned length)
{
    if (!d) return false;
    std::string temp;
    Client* client = reinterpret_cast<Client*>(d);
    if (false == client->cwd(temp)) return false;
    // +1 to account for NULL
    if (length < temp.length() + 1) return false;
    memcpy(cwd, temp.c_str(), temp.length());
    cwd[temp.length()] = '\0';
    return true;
}

EXPORT int cstyx_set_cwd(cstyx_client_t* d, const char* path)
{
    if (!d) return -1;
    if (!path) return 0;
    Client* c=(Client*)d;
    if (false == c->setCWD(path)) return -1;
    return 0;
}

EXPORT cstyx_file_t* cstyx_open(cstyx_client_t* d, const char* path, int mode)
{
    if (!d) return NULL;
    if (!path) return NULL;
    bool r = (mode & CSTYX_READ)  ? true : false;
    bool w = (mode & CSTYX_WRITE) ? true : false;
    return (cstyx_file_t*)(new File(*(Client*)d,path,r,w));
}

EXPORT cstyx_dirent_t* cstyx_stat(cstyx_file_t* f)
{
    if (!f) return 0;
    Dirent* pDirent = new Dirent();
    if (false == ((File*)f)->stat(*pDirent))
    {
        delete pDirent;
        return 0;
    }
    return (cstyx_dirent_t*)(pDirent);
}

EXPORT void cstyx_delete_dirent(cstyx_dirent_t* d)
{
    if (!d) return;
    delete (Dirent*)d;
}

EXPORT long cstyx_get_file_len(cstyx_file_t* f)
{
    if (!f) return -1;
    return ((File*)f)->size();
}

EXPORT void cstyx_close(cstyx_file_t* f)
{
    if (!f) return;
    delete (File*)f;
}

EXPORT int cstyx_read(cstyx_file_t* f, char* d, u16* len)
{
    if (!f||!d||!len) return -1;
    string buf;
    if (false == ((File*)f)->read(buf,*len)) { return -1; }
    if (0 == *len) return 0;
    else if (0 > *len) return -1;
    memcpy(d,buf.data(),buf.size());
    return 0;
}

EXPORT int cstyx_read_all_path(cstyx_client_t* c, const char* path, char* buf, int lim)
{
    if (!c||!path||!buf) return -1;
    std::string str;
    if (false == ((Client*)c)->readAll(path,str,lim)) return -1;
    memcpy((void*)buf,str.data(),str.size());
    return str.size();
}

EXPORT int cstyx_write(cstyx_file_t* f, const char* d, u16* len)
{
    if (!f||!d||!len) return -1;
    string buf(d);
    if (!((File*)f)->write(d,*len)) return -1;
    return *len;
}

EXPORT int cstyx_write_path(cstyx_client_t* c, const char* path, const char* d, int len)
{
    if (!c||!path||!d) return -1;
    std::string s(d,len);
    return ((Client*)c)->write(path,s);
}

EXPORT cstyx_client_t* cstyx_connect(cstyx_connection_t* c)
{
    if (!c) return 0;
    return (cstyx_client_t*)(new Client(new Connection(c)));
}

EXPORT const char* cstyx_get_latest_error(int* d)
{
    const Error& e=Error::getLatestError();
    if (d) *d=e.code();
    return e.msg().c_str();
}
