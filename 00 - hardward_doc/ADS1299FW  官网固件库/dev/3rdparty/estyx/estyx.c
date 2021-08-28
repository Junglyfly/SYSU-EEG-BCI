
#include "estyx.h"
#include <string.h>

#ifdef C55X
#include "portconf.h"
#endif

char	Enomem[] =	"out of memory";
char	Eperm[] =	"permission denied";
char	Enodev[] =	"no free devices";
char	Ehungup[] =	"write to hungup channel";
char	Eexist[] =	"file exists";
char	Enonexist[] =	"file does not exist";
char	Ebadcmd[] =	"bad command";
char	Ebadarg[] =	"bad arg in system call";
char	Enofid[] =	"no such fid";
char	Enotdir[] =	"not a directory";
char	Eopen[] =	"already open";
char	Ebadfid[] =	"bad fid";
char	Enoauth[] =	"authorisation not supported";
char	Eio[] =		"i/o error";

#define TABSZ	32	/* power of 2 */
#define MAXSTAT	512

u16 estyx_calclen(u32 datasize, u32 offset, u16 requested)
{
	u16 len;

	if (!requested||offset>=datasize) return 0;
	len=datasize-offset;
	if (len>requested) len=requested;
	return len;
}

#if 0
static char *nilconv(char *s)
{
	if(s != nil && s[0] == '\0')
		return nil;
	return s;
}
#endif

static void incref(estyx_file_t *f)
{
	if (f!=nil) f->ref++;
}

static void decref(estyx_file_t *f)
{
	if (f!=nil) --f->ref;
}

static void increff(estyx_fid_t *f)
{
	if (f) incref(f->file);
}

static void decreff(estyx_fid_t *f)
{
	if (f) decref(f->file);
}

static void incopen(estyx_fid_t *f)
{
	if (f&&f->file)	f->file->open++;
	//printf("incopen on fid %d, file [%s], now %d\n",(int)f->fid,f->file->d.name,(int)f->file->open);
}

static void decopen(estyx_fid_t *f)
{
	if (f&&f->file) f->file->open--;
	//printf("decopen on fid %d, file [%s], now %d\n",(int)f->fid,f->file->d.name,(int)f->file->open);
}

static void flushtag(int oldtag)
{
//	USED(oldtag);
}

static int hash(int path)
{
	return path&(TABSZ-1);
}

int estyx_perm(estyx_file_t *f, char *uid, int mode)
{
	int m, p;

	p = 0;
	switch(mode&3){
	case OREAD:
		p = AREAD;
		break;
	case OWRITE:
		p = AWRITE;
		break;
	case ORDWR:
		p = AREAD+AWRITE;
		break;
	case OEXEC:
		p = AEXEC;
		break;
	default:
		return -1;
	}
	if (mode&OTRUNC)
		p |= AWRITE;
	if (f->d.mode&DMDIR)
		m=AREAD|AEXEC;
	else {
		m=f->d.mode;
		if (!f->ops.read) m&=~AREAD;
		if (!f->ops.write) m&=~AWRITE;
	}
	m = m&7;
	if ((p&m) == p)
		return 1;
	return 0;
}

static const estyx_fops_t default_fops={0,0,0,0,0};

static estyx_file_t *newfile(estyx_server_t *server, 
	estyx_file_t *parent, int isdir, 
	const char *name, estyx_fops_t *fops, void *u)
{
	estyx_file_t *file;
	estyx_dir_t d;
	int h;
	int qid;

	qid = server->qidgen++;
	if (parent) {
		for (file = parent->child; file; file = file->sibling)
			if (!strcmp(name, file->d.name)) return 0;
	} else
		parent=server->root;
	file = (estyx_file_t *)estyx_malloc(sizeof(estyx_file_t));
    if (!file) return file;

    memset(file, 0, sizeof(estyx_file_t));
	file->parent = parent;
	file->child = nil;
	h = hash(qid);
	file->next = server->ftab[h];
	server->ftab[h] = file;
	if (parent) {
		file->sibling = parent->child;
		parent->child = file;
	} else
		file->sibling = nil;

	d.type = 'X';
	d.dev = 'x';
	d.qid.path = qid;
	d.qid.type = 0;
	d.qid.vers = 0;
	if (isdir)
		d.mode=0555;
	else if (fops) {
		d.mode=0;
		if (fops->read) d.mode|=0444;
		if (fops->write) d.mode|=0222;
	}
//	d.atime = time(0);
//	d.mtime = boottime;
//	d.length = 0;
	d.name=name;
//	d.name = strdup(name);
//	d.uid = strdup(owner);
//	d.gid = strdup(eve);
//	d.muid = "";

	if(isdir) {
		d.qid.type |= QTDIR;
		d.mode |= DMDIR;
	} else {
		d.qid.type &= ~QTDIR;
		d.mode &= ~DMDIR;
	}
	if (fops)
		file->ops=*fops;
	else
		file->ops=default_fops;

	file->d = d;
	file->ref = 0;
	file->open = 0;
	file->u=u;
#if DEBUG
	styx_print(2, "New file %s Qid=%llx\n", name, qid);
#endif
	return file;
}

estyx_file_t *estyx_findfile(estyx_server_t *server, int path)
{
	estyx_file_t *f;

	for(f = server->ftab[hash(path)]; f != nil; f = f->next){
		if(f->d.qid.path == path)
			return f;
	}
	return nil;
}

//! Serialise & write an fcall
static int wr(estyx_server_t *s, estyx_fcall_t *r)
{
	int n;
	u8 buf[32];
	u8 *lbuf;

	n=sizeS2M(r);
	lbuf=0;
	if (n>32) 
    {
        lbuf=estyx_malloc(n);
        if (!lbuf) return -1;
    }
	else lbuf=buf;
	n = convS2M(r, &lbuf);
	if (!n) {
		r->u.ename = "error in message conversion";
		return -1;
	}
	/* styx_print(2, "wr: %F\n", r); */
	if (r->type!=Rread)
		n=s->txcb(s,n,lbuf,0,0);
	else
		n=s->txcb(s,n,lbuf,r->u.rw.count,r->u.rw.data);
	if (lbuf!=buf) estyx_free(lbuf);
	return 0;
}

static estyx_fid_t *findfid(estyx_server_t *c, short fid)
{
	estyx_fid_t *f;

	for(f = c->fids; f && f->fid != fid; f = f->next);
	return f;
}

estyx_fid_t *newfid(estyx_server_t *c, estyx_file_t *file, short fid, Qid qid)
{
	estyx_fid_t *f;

	f = estyx_malloc(sizeof(estyx_fid_t));
	if (!f) return 0;
	//ASSERT(f, "newfid");
	f->server = c;
	f->fid = fid;
	f->file=file;
	f->open = 0;
	f->dri = 0;
	f->qid = qid;
	f->next = c->fids;
	c->fids = f;
	increff(f);
	return f;
}

static estyx_fid_t *fidclone(estyx_fid_t *old, short fid)
{
	estyx_fid_t *new;

	new = newfid(old->server, old->file, fid, old->qid);
	return new;
}

static void deletefid(estyx_fid_t *d)
{
	//### TODO end any outstanding reads on this fid
	estyx_fid_t **f;
	estyx_server_t *s;

	//ASSERT(d,"Null styx_fid_t in deletefid");
	s=d->server;
	for(f = &s->fids; *f; f = &(*f)->next)
		if (*f==d) {
			decreff(d);
			if (d->open) decopen(d);
			*f = d->next;
			estyx_free(d);
			return;
		}
}

static void deletefids(estyx_server_t *e)
{
	estyx_fid_t *f, *g;

	for(f = e->fids; f; f = g){
		decreff(f);
		if (f->open) decopen(f);
		g = f->next;
		estyx_free(f);
	}
	e->fids=0;
}

static void deletefiles(estyx_file_t *file)
{
    if (file != nil)
    {
        if (file->child != nil)
        {
            deletefiles(file->child);
        }

        if (file->sibling != nil)
        {
            deletefiles(file->sibling);
        }

        estyx_free(file);
    }
}

void estyx_destroy(estyx_server_t *server)
{
    /*! free the file IDs allocated by the server */
    deletefids(server);
    /*! free the files in the server file system table */
    deletefiles(server->root);
    /*! free the file search hash table */
    estyx_free(server->ftab);
}

void estyx_detach(estyx_server_t *server)
{
    /*! free the file IDs allocated by the server */
	deletefids(server);
}

static Walkqid *devwalk(estyx_server_t *server, estyx_file_t *file,
	estyx_fid_t *fp, 
	estyx_fid_t *nfp, char **name, int nname, char **err)
{
	long j;
	Walkqid *wq;
	char *n;
	estyx_file_t *p, *f;
	estyx_ops_t *ops;
	Qid qid;

	*err = nil;
	ops = server->ops;

	wq = estyx_malloc(sizeof(Walkqid)+(nname-1)*sizeof(Qid));
    if (!wq) return wq;
	wq->nqid = 0;

	p = file;
	qid = p != nil ? p->d.qid : fp->qid;
	for(j = 0; j < nname; j++){
		if(!(qid.type&QTDIR)){
			if(j == 0) {
				//styx_fatal("devwalk error");
				estyx_free(wq);
				return 0;
			}
			*err = Enotdir;
			goto Done;
		}
		n = name[j];
		if(strcmp(n, ".") == 0) {
    Accept:
			wq->qid[wq->nqid++] = nfp->qid;
			continue;
		}
		if(p != nil && strcmp(n, "..") == 0 && p->parent){
			decref(p);
			nfp->qid.path = p->parent->d.qid.path;
			nfp->qid.type = p->parent->d.qid.type;
			nfp->qid.vers = 0;
			nfp->file=p->parent;
			incref(p->parent);
			p = p->parent;
			qid = p->d.qid;
			goto Accept;
		}
		
		if(ops&&ops->walk != nil){
			char *e;

			e = ops->walk(&qid, n);
			if(!e){
				decreff(nfp);
				nfp->qid = qid;
				increff(nfp);
				p = estyx_findfile(server, qid.path);
				if(server->needfile && p == nil)
					goto Done;
				qid = p != nil ? p->d.qid : nfp->qid;
				goto Accept;
			}
		}

		if(p != nil)
		for(f = p->child; f != nil; f = f->sibling){
			if(strcmp(n, f->d.name) == 0){
				decref(p);
				nfp->qid.path = f->d.qid.path;
				nfp->qid.type = f->d.qid.type;
				nfp->qid.vers = 0;
				nfp->file=f;
				incref(f);
				p = f;
				qid = p->d.qid;
				goto Accept;
			}
		}
		/* The following condition is the original. It only works for relative paths */
		/* if(j == 0 && *err == nil) */
		if(*err == nil) /* Need to ignore j if we want walks to fail for absolute paths, too. */
			*err = Enonexist;
		goto Done;
	}
Done:
	if(*err != nil){
		estyx_free(wq);
		return nil;
	}
	return wq;
}

static int wrerr(estyx_server_t *s, estyx_fcall_t *f, char *err)
{
	f->u.ename=err;
	f->type=Rerror;
	wr(s,f);
	return 0;
}

int estyx_rread(estyx_fid_t *fp, u16 len, u8 *buf)
{
	estyx_fcall_t *f;
	
	if (!fp) return -3;
	f=fp->fcall;
	if (!buf||!len) f->u.rw.count=0;
	else f->u.rw.count=len;
	f->type=Rread;
	f->u.rw.data=buf;
	wr(fp->server,f);
	return 0;
}

#ifdef C55X
/* this packs the data packet so we can transmit it on the 5509 */
int estyx_rread55(estyx_fid_t *f,u16 len,u8 *data)
{
	u8 *buf;
	int err;

	if (!len) return estyx_rread(f,0,0);
	buf=estyx_malloc(len/2+1);
	if (!buf) return -1;
	usbhw_pack55(data,(u16 *)buf,len);
	err=estyx_rread(f,len,buf);
	estyx_free(buf);
	return err;
}
#endif

/*!
\p fp is the directory's fid.  \p file is the file structure.  \p n bytes are read into \p d.  The read begins at directory entry \p fp->dri, 
*/
int dir_read(estyx_fid_t *fp, u16 len, u32 pos)
{
	long dsz, m, q; // m: where the new entry begins in temp
		// q: where the new entry ends in temp
	u16 l;
	estyx_file_t *f;
	int i;
	char *temp;
	u8 *data;
	u32 flen;

	//printf("----------------dirread\n");
	f=fp->file;
	if (len>ESTYX_DIR_IOUNIT) len=ESTYX_DIR_IOUNIT;
	data=estyx_malloc(len);
	if (!data) return -1;
	f = f->child;
	if (!pos) {
		fp->dri=0;
		fp->drofs=0;
	} else {
		for(i = 0; i < fp->dri; i++) {
			if (!f) {
				estyx_free(data);
				estyx_rread(fp,0,0);
				return 0;
			}
			else f = f->sibling;
		}
	}
	l=len;
	//printf("len=%d\n",len);
	for(m = 0; m < l; fp->dri++) {
		if (!f) break;
		dsz=sizeD2M(&(f->d));
		if (dsz>(l-m)) break;
		temp=estyx_malloc(dsz);
		if (!temp) return -1;
		if (f->ops.len) flen=f->ops.len(f);
		else flen=0;
		dsz = convD2M(&(f->d),(u8*)temp,dsz,flen);
		//printf("*** entry %s\ndsz=%ld ",f->d.name,dsz);
		q=m+dsz;
		if (q>l) q=l;
		if (m<0) {
			estyx_free(temp);
			return -1;
		}
		memcpy(data+m,temp+fp->drofs,q-m);
		estyx_free(temp);
		if (q<l) fp->drofs=0;
		else fp->drofs=q-m;
		m=q;
		//printf("m=%ld\n",m);
		f = f->sibling;
	}
	len=m;
	//printf("<<<final len=%d (%X)\n",len,len);
	#ifdef C55X
	estyx_rread55(fp,len,data);
	#else
	estyx_rread(fp,len,data);
	#endif
	estyx_free(data);
	return 0;
}

static int dir_open(estyx_fid_t *f, u16 *iounit, u8 mode)
{
	*iounit=ESTYX_DIR_IOUNIT;
	f->dri=0;
	f->drofs=0;
	return 0;
}

static const estyx_fops_t dirops={
	dir_read,
	0,
	dir_open,
	0,
	0
};

void estyx_setrw(estyx_file_t *f, int r, int w)
{
	if (!f) return;
	if (f->d.mode&DMDIR) return;
	if (r>=0) {
		if (r&&f->ops.read) f->d.mode|=0444;
		else f->d.mode&=~0444;
	}
	if (w>=0) {
		if (w&&f->ops.write) f->d.mode|=0222;
		else f->d.mode&=~0222;
	}
}

int estyx_canread(estyx_file_t *f)
{
	if (!f) return 0;
	return ((f->d.mode&4)&&(f->ops.read));
}

int estyx_canwrite(estyx_file_t *f)
{
	if (!f) return 0;
	return ((f->d.mode&2)&&(f->ops.write));
}

int tread(estyx_server_t *s, estyx_fcall_t *f, estyx_fid_t *fp)
{
	estyx_file_t *file;
	u16 len;
	int err;
	volatile int p;

	file=fp->file;
	if (!file) return wrerr(s,f,Ebadfid);
	if (!file->open) return wrerr(s,f,Ebadfid);
	if (!file->ops.read) return wrerr(s,f,Eperm);
	if (f->u.rw.count>0xffff) f->u.rw.count=0xffff;
	len=f->u.rw.count;
	//int (*read)(estyx_file_t *f, u16 len, u32 pos);
	f->u.rw.data=0;
	fp->fcall=f;
	err=file->ops.read(fp,len,f->u.rw.offset);
	if (err==-2) {
		return 0; // TODO set up splits here
	}
	if (err<0) {
		return wrerr(s,f,Eio);
	}
	return 0;
}

int topen(estyx_server_t *s, estyx_fcall_t *f, estyx_fid_t *fp)
{
	estyx_file_t *file;
	u8 mode;

	file=fp->file;
	if (!file) return wrerr(s,f,Ebadfid);
	f->u.ename = nil;
	if (fp->open)
		f->u.ename = Eopen;
	else if ((fp->qid.type&QTDIR) && (f->u.create.mode&(OWRITE|OTRUNC|ORCLOSE)))
		f->u.ename = Eperm;
	else if (file != nil && !estyx_perm(file, 0, f->u.create.mode))
		f->u.ename = Eperm;
	else if ((f->u.create.mode&ORCLOSE) && file != nil && file->parent != nil && !estyx_perm(file->parent, 0, OWRITE))
		f->u.ename = Eperm;
	if(f->u.ename != nil){
		f->type = Rerror;
		wr(s,f);
		return 0;
	}
	f->u.ename = Enonexist;
	decreff(fp);
	if(!s->ops || s->ops->open == nil || (f->u.ename = s->ops->open(&fp->qid, f->u.create.mode)) == nil) {
		f->type = Ropen;
		if (file->ops.open) {
			mode=f->u.create.mode;
			if (((mode&3)==OEXEC)||((mode&3)==OREAD))
				mode&=~OTRUNC;
			if (file->ops.open(fp,&(file->iounit),mode))
				return wrerr(s,f,"cannot open");
		}

		if (0 != file->iounit) { f->u.open.iounit=file->iounit; }
		else { f->u.open.iounit=65535; }
		f->u.open.qid = fp->qid;
		fp->mode = f->u.create.mode;
		fp->open = 1;
		//fp->offset = 0;
		incopen(fp);
	} else
		f->type = Rerror;
	increff(fp);
	wr(s,f);
	return 0;
}

int tcreate(estyx_server_t *s, estyx_fcall_t *f, estyx_fid_t *fp)
{
	return wrerr(s,f,"create not supported");
#if 0
// create disabled -- always returns noperm
#if DEBUG
	styx_print(2, "Tcreate %d %s\n", f.fid, f.u.create.name);
#endif
	f.u.ename = nil;
	if(fp->open)
		f.u.ename = Eopen;
	else if(!(fp->qid.type&QTDIR))
		f.u.ename = Enotdir;
	else if((f.u.create.perm&DMDIR) && (f.u.create.mode&(OWRITE|OTRUNC|ORCLOSE)))
		f.u.ename = Eperm;
	else if(file != nil && !estyx_perm(file, c->uname, OWRITE))
		f.u.ename = Eperm;
	if(f.u.ename != nil){
		f.type = Rerror;
		wr(s, &f);
		break;
	}
	f.u.ename = Eperm;
	decreff(fp);
	if(file != nil){
		if(f.u.create.perm&DMDIR)
			f.u.create.perm = (f.u.create.perm&~0777) | (file->d.mode&f.u.create.perm&0777) | DMDIR;
		else
			f.u.create.perm = (f.u.create.perm&(~0777|0111)) | (file->d.mode&f.u.create.perm&0666);
	}
	if(!ops || ops->create && (f.u.ename = ops->create(&fp->qid, f.u.create.name, f.u.create.perm, f.u.create.mode)) == nil){
		f.type = Rcreate;
		f.u.open.qid = fp->qid;
		fp->mode = f.u.create.mode;
		fp->open = 1;
		//fp->offset = 0;
		incopen(fp);
	}
	else
		f.type = Rerror;
	increff(fp);
	wr(s, &f);
#endif
}

int tstat(estyx_server_t *s, estyx_fcall_t *f, estyx_fid_t *fp)
{
	estyx_file_t *file;
	u32 flen;

#if DEBUG
	styx_print(2, "Tstat %d qid=%llx\n", f->fid, fp->qid.path);
#endif
	if (!fp) return wrerr(s,f,"stat error");
	file=fp->file;
	if (!file) return wrerr(s,f,Ebadfid);
	f->u.stat.stat = estyx_malloc(MAXSTAT);
	if (!f->u.stat.stat) return wrerr(s,f,"malloc error");
	f->u.ename = "stat error";
	if (file->ops.len)
		flen=file->ops.len(file);
	else
		flen=0;
	f->type = Rstat;
	f->u.stat.nstat = convD2M(&file->d, f->u.stat.stat, MAXSTAT,flen);
	wr(s, f);
	estyx_free(f->u.stat.stat);
	return 0;
}

int twrite(estyx_server_t *s, estyx_fcall_t *f, estyx_fid_t *fp)
{
	estyx_file_t *file;
	u16 ct;
	int err;

#if DEBUG
	styx_print(2, "Twrite %d\n", f.fid);
#endif
	file=fp->file;
	if (!file) return wrerr(s,f,Ebadfid);
	if (!file->open) return wrerr(s,f,Ebadfid);
	if (!file->ops.write) return wrerr(s,f,Eperm);
	//int (*write)(estyx_file_t *f, u16 len, char *data, u32 pos);
	if (f->u.rw.count>0xffff) f->u.rw.count=0xffff;
	ct=f->u.rw.count;
	err=file->ops.write(fp,&ct,f->u.rw.data,f->u.rw.offset);
	if (err<0) return wrerr(s,f,Eio);
	f->type=Rwrite;
	f->u.rw.count=ct;
	//if (ops->write && (f.u.ename = ops->write(fp->qid, f.u.rw.data, (u32*)(&f.u.rw.count), f.u.rw.offset)) == nil) {
	//	f.type = Rwrite;
	//}
	wr(s, f);
	return 0;
}

int twalk(estyx_server_t *s, estyx_fcall_t *f, estyx_fid_t *fp)
{
	estyx_fid_t *nfp;
	Walkqid *wq;
	int i;

#if DEBUG
	styx_print(2, "Twalk %d %d", f->fid, f->u.twalk.newfid);
	for(i = 0; i < f->u.twalk.nwname; i++)
		styx_print(2, " %s", f->u.twalk.wname[i]);
	styx_print(2, "\n");
#endif
	if (!fp) return wrerr(s,f,"no such fid");
	if (fp->open) return wrerr(s,f,"fid in use");
	nfp = findfid(s, f->u.twalk.newfid);
	if (nfp&&(nfp!=fp)) {
		deletefid(nfp);
		nfp=0;
	}
	if (!nfp)
		nfp = fidclone(fp, f->u.twalk.newfid);

	if ((wq = devwalk(s, fp->file, fp, nfp, f->u.twalk.wname, f->u.twalk.nwname, &f->u.ename)) == nil) {
		if(nfp != fp)
			deletefid(nfp);
		return wrerr(s,f,"path not found");
	}
	if (nfp != fp) {
		if (wq->nqid!=f->u.twalk.nwname)
			deletefid(nfp);
	}
	f->type = Rwalk;
	f->u.rwalk.nwqid = wq->nqid;
	for(i = 0; i < wq->nqid; i++)
		f->u.rwalk.wqid[i] = wq->qid[i];
	estyx_free(wq);
	wr(s,f);
	return 0;
}

int estyx_rx(estyx_server_t *s, u16 len, u8 *msg)
{
	estyx_fcall_t f;
	estyx_fid_t *fp;
	int open;
	estyx_file_t *file;
	Qid qid;

  file = nil;
	if (!convM2S(msg, len, &f))
		return -1;
	if(f.type == Tflush) {
		flushtag(f.u.oldtag);
		f.type = Rflush;
		wr(s, &f);
		return 0;
	}
	file = 0;
	fp = findfid(s, f.fid);
	if (f.type != Tversion && f.type != Tauth && f.type != Tattach) {
		if (fp == nil) {
			f.type = Rerror;
			f.u.ename = Enofid;
			wr(s, &f);
			return 0;
		} else {
			file = estyx_findfile(s, fp->qid.path);
			if(s->needfile && file == nil){
				f.type = Rerror;
				f.u.ename = Enonexist;
				wr(s, &f);
				return 0;
			}
		}
	}

	switch(f.type){
	case Twalk:
		twalk(s,&f,fp);
		break;
	case Topen:
#if DEBUG
		styx_print(2, "Topen %d\n", f.fid);
#endif
		topen(s,&f,fp);
		break;
	case Tcreate:
		tcreate(s,&f,fp);
		break;
	case Tread:
#if DEBUG
		styx_print(2, "Tread %d\n", f.fid);
#endif
		tread(s,&f,fp);
		break;
	case Twrite:
		twrite(s,&f,fp);
		break;
	case Tclunk:
#if DEBUG
		styx_print(2, "Tclunk %d\n", f.fid);
#endif
		open = fp->open;
		qid = fp->qid;
		if (open&&fp->file&&fp->file->ops.close)
			fp->file->ops.close(fp);
		deletefid(fp);
		f.type = Rclunk;
		wr(s, &f);
		break;
	case Tremove:
#if 0
// remove disabled, always noperm
#if DEBUG
		styx_print(2, "Tremove %d\n", f.fid);
#endif
		if (file != nil && file->parent != nil && !estyx_perm(file->parent, c->uname, OWRITE)) {
			f.type = Rerror;
			f.u.ename = Eperm;
			deletefid(s,fp);
			wr(s,&f);
			break;
		}
		f.u.ename = Eperm;
		if(ops->remove && (f.u.ename = ops->remove(fp->qid)) == nil) 
			f.type = Rremove;
		else
			f.type = Rerror;
		deletefid(c, fp);
		wr(s, &f);
		break;
#else
		f.u.ename = Eperm;
		f.type = Rerror;
		wr(s, &f);
		break;
#endif
	case Tstat:
		tstat(s,&f,fp);
		break;
	case Twstat:
#if 0
#if DEBUG
		styx_print(2, "Twstat %d\n", f.fid);
#endif
		f.u.ename = Eperm;
		convM2D(f.u.stat.stat, f.u.stat.nstat, &dir, strs);
		//dir.name = nilconv(dir.name);
		//dir.uid = nilconv(dir.uid);
		//dir.gid = nilconv(dir.gid);
		//dir.muid = nilconv(dir.muid);
		if(ops && ops->wstat && (f.u.ename = ops->wstat(fp->qid, &dir)) == nil)
			f.type = Rwstat;
		else
			f.type = Rerror;
		wr(s, &f);
#endif
		wrerr(s,&f,Eperm);
		break;
	case Tversion:
#if DEBUG
		styx_print(2, "Tversion\n");
#endif
		f.type = Rversion;
		f.tag = NOTAG;
		wr(s, &f);
		break;
	case Tauth:
#if DEBUG
		styx_print(2, "Tauth\n");
#endif
		f.u.ename = Enoauth;
		f.type = Rerror;
		wr(s, &f);
		break;
	case Tattach:
#if DEBUG
		styx_print(2, "Tattach %d %s\n", f.fid, f.u.attach.uname[0] ? f.u.attach.uname : c->uname);
#endif
		estyx_detach(s); // just in case
		qid.path = Qroot;
		qid.type = QTDIR;
		qid.vers = 0;
		fp = newfid(s, s->root, f.fid, qid);
		if (!fp) {
			wrerr(s,&f,"Out of memory");
			return 0;
		}
		f.type = Rattach;
		f.fid = fp->fid;
		f.u.open.qid = qid;
		wr(s, &f);
		break;
	}
	return 0;
}

estyx_file_t *estyx_mkfile(estyx_server_t *server, 
	estyx_file_t *parent, 
	const char *name, estyx_fops_t *fops, void *u)
{
	return newfile(server,parent,0,name,fops,u);
}

estyx_file_t *estyx_mkdir(estyx_server_t *server, estyx_file_t *parent,
	const char *name)
{
	return newfile(server,parent,1,name,(estyx_fops_t *)&dirops,0);
}

void estyx_set_tx_callback(estyx_server_t *server, estyx_tx_cb cb)
{
	server->txcb=cb;
}

int estyx_init(estyx_server_t *server, estyx_ops_t *ops, estyx_tx_cb txcb)
{
	int i;

	server->ops = ops;
	//server->clients = nil;
	server->root = nil;
	server->ftab = (estyx_file_t**)estyx_malloc(TABSZ*sizeof(estyx_file_t*));
    if (!server->ftab) return -1;
	for(i = 0; i < TABSZ; i++)
		server->ftab[i] = nil;
	server->qidgen = 0;
	server->root = newfile(server, 0, 1, "/", (estyx_fops_t *)&dirops, 0);
	server->txcb=txcb;
	server->fids=0;
	return 0;
}
