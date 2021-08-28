
#include <string.h>
#include "estyx.h"

static u8* gstring(u8 *p, u8 *ep, char **s)
{
	u16 n;

	if(p+BIT16SZ > ep)
		return nil;
	n = GBIT16(p);
	p += BIT16SZ - 1;
	if(p+n+1 > ep)
		return nil;
	/* move it down, on top of count, to make room for '\0' */
	memmove(p, p + 1, n);
	p[n] = '\0';
	*s = (char*)p;
	p += n+1;
	return p;
}

static u8* gqid(u8 *p, u8 *ep, Qid *q)
{
	if(p+QIDSZ > ep)
		return nil;
	q->type = GBIT8(p);
	p += BIT8SZ;
	q->vers = GBIT32(p);
	p += BIT32SZ;
	q->path = GBIT32(p);
	p += BIT64SZ;
	return p;
}

u16 convM2S(u8 *ap, u16 nap, estyx_fcall_t *f)
{
	u8 *p, *ep;
	u16 i, size;

	p = ap;
	ep = p + nap;

	if(p+BIT32SZ+BIT8SZ+BIT16SZ > ep)
		return 0;
	size = GBIT32(p);
	p += BIT32SZ;

	if (size<BIT32SZ+BIT8SZ+BIT16SZ)
		return 0;

	f->type = GBIT8(p);
	p += BIT8SZ;
	f->tag = GBIT16(p);
	p += BIT16SZ;

	switch(f->type) {
	default:
		return 0;

	case Tversion:
		if(p+BIT32SZ > ep)
			return 0;
		f->u.version.msize = GBIT32(p);
		p += BIT32SZ;
		p = gstring(p, ep, &f->u.version.version);
		break;

	case Tflush:
		if(p+BIT16SZ > ep)
			return 0;
		f->u.oldtag = GBIT16(p);
		p += BIT16SZ;
		break;

	case Tauth:
		if(p+BIT32SZ > ep)
			return 0;
		f->u.attach.afid = GBIT32(p);
		p += BIT32SZ;
		p = gstring(p, ep, &f->u.attach.uname);
		if(p == nil)
			break;
		p = gstring(p, ep, &f->u.attach.aname);
		if(p == nil)
			break;
		break;

	case Tattach:
		if(p+BIT32SZ > ep)
			return 0;
		f->fid = GBIT32(p);
		p += BIT32SZ;
		if(p+BIT32SZ > ep)
			return 0;
		f->u.attach.afid = GBIT32(p);
		p += BIT32SZ;
		p = gstring(p, ep, &f->u.attach.uname);
		if(p == nil)
			break;
		p = gstring(p, ep, &f->u.attach.aname);
		if(p == nil)
			break;
		break;

	case Twalk:
		if(p+BIT32SZ+BIT32SZ+BIT16SZ > ep)
			return 0;
		f->fid = GBIT32(p);
		p += BIT32SZ;
		f->u.twalk.newfid = GBIT32(p);
		p += BIT32SZ;
		f->u.twalk.nwname = GBIT16(p);
		p += BIT16SZ;
		if(f->u.twalk.nwname > MAXWELEM)
			return 0;
		for(i=0; i<f->u.twalk.nwname; i++){
			p = gstring(p, ep, &f->u.twalk.wname[i]);
			if(p == nil)
				break;
		}
		break;

	case Topen:
		if(p+BIT32SZ+BIT8SZ > ep)
			return 0;
		f->fid = GBIT32(p);
		p += BIT32SZ;
		f->u.create.mode = GBIT8(p);
		p += BIT8SZ;
		break;

	case Tcreate:
		if(p+BIT32SZ > ep)
			return 0;
		f->fid = GBIT32(p);
		p += BIT32SZ;
		p = gstring(p, ep, &f->u.create.name);
		if(p == nil)
			break;
		if(p+BIT32SZ+BIT8SZ > ep)
			return 0;
		f->u.create.perm = GBIT32(p);
		p += BIT32SZ;
		f->u.create.mode = GBIT8(p);
		p += BIT8SZ;
		break;

	case Tread:
		if(p+BIT32SZ+BIT64SZ+BIT32SZ > ep)
			return 0;
		f->fid = GBIT32(p);
		p += BIT32SZ;
		f->u.rw.offset = GBIT32(p);
		p += BIT64SZ;
		f->u.rw.count = GBIT32(p);
		p += BIT32SZ;
		break;

	case Twrite:
		if(p+BIT32SZ+BIT64SZ+BIT32SZ > ep)
			return 0;
		f->fid = GBIT32(p);
		p += BIT32SZ;
		f->u.rw.offset = GBIT32(p);
		p += BIT64SZ;
		f->u.rw.count = GBIT32(p);
		p += BIT32SZ;
		if(p+f->u.rw.count > ep)
			return 0;
		f->u.rw.data = p;
		p += f->u.rw.count;
		break;

	case Tclunk:
	case Tremove:
		if(p+BIT32SZ > ep)
			return 0;
		f->fid = GBIT32(p);
		p += BIT32SZ;
		break;

	case Tstat:
		if(p+BIT32SZ > ep)
			return 0;
		f->fid = GBIT32(p);
		p += BIT32SZ;
		break;

	case Twstat:
		if(p+BIT32SZ+BIT16SZ > ep)
			return 0;
		f->fid = GBIT32(p);
		p += BIT32SZ;
		f->u.stat.nstat = GBIT16(p);
		p += BIT16SZ;
		if(p+f->u.stat.nstat > ep)
			return 0;
		f->u.stat.stat = p;
		p += f->u.stat.nstat;
		break;
#if 0
// we never receive R packets so no need for this
/*
 */
	case Rversion:
		if(p+BIT32SZ > ep)
			return 0;
		f->u.version.msize = GBIT32(p);
		p += BIT32SZ;
		p = gstring(p, ep, &f->u.version.version);
		break;

	case Rerror:
		p = gstring(p, ep, &f->u.ename);
		break;

	case Rflush:
		break;

	case Rauth:
		p = gqid(p, ep, &f->u.aqid);
		if(p == nil)
			break;
		break;

	case Rattach:
		p = gqid(p, ep, &f->u.open.qid);
		if(p == nil)
			break;
		break;

	case Rwalk:
		if(p+BIT16SZ > ep)
			return 0;
		f->u.rwalk.nwqid = GBIT16(p);
		p += BIT16SZ;
		if(f->u.rwalk.nwqid > MAXWELEM)
			return 0;
		for(i=0; i<f->u.rwalk.nwqid; i++){
			p = gqid(p, ep, &f->u.rwalk.wqid[i]);
			if(p == nil)
				break;
		}
		break;

	case Ropen:
	case Rcreate:
		p = gqid(p, ep, &f->u.open.qid);
		if(p == nil)
			break;
		if(p+BIT32SZ > ep)
			return 0;
		f->u.open.iounit = GBIT32(p);
		p += BIT32SZ;
		break;

	case Rread:
		if(p+BIT32SZ > ep)			return 0;
		f->u.rw.count = GBIT32(p);
		p += BIT32SZ;
		if(p+f->u.rw.count > ep)
			return 0;
		f->u.rw.data = data;
		p += f->u.rw.count;
		break;

	case Rwrite:
		if(p+BIT32SZ > ep)
			return 0;
		f->u.rw.count = GBIT32(p);
		p += BIT32SZ;
		break;

	case Rclunk:
	case Rremove:
		break;

	case Rstat:
		if(p+BIT16SZ > ep)
			return 0;
		f->u.stat.nstat = GBIT16(p);
		p += BIT16SZ;
		if(p+f->u.stat.nstat > ep)
			return 0;
		f->u.stat.stat = p;
		p += f->u.stat.nstat;
		break;

	case Rwstat:
		break;
#endif
	}

	if(p==nil || p>ep)
		return 0;
	if(ap+size == p)
		return size;
	return 0;
}
