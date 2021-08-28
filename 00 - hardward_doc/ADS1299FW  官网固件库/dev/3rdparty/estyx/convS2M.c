#include <string.h>
#include "estyx.h"

//! Write a Pascal string
/*! \internal Writes \p s as a Pascal string to \p p.  The string begins with a 16-bit length.

\param p Output buffer
\param s Input C string

\return Resulting Pascal string
*/
static u8 *pstring(u8 *p, char *s)
{
	u16 n;

	if(s == nil){
		PBIT16(p, 0);
		p += BIT16SZ;
		return p;
	}

	n = strlen(s);
	PBIT16(p, n);
	p += BIT16SZ;
	memmove(p, s, n);
	p += n;
	return p;
}

//! Serialise a Qid
/*! \internal Serialises Qid \p q to buffer \p p.

\param p Output buffer
\param q Qid to serialise

\return Result
*/
static u8 *pqid(u8 *p, Qid *q)
{
	PBIT8(p, q->type);
	p += BIT8SZ;
	PBIT32(p, q->vers);
	p += BIT32SZ;
	PBIT32(p, q->path);
	p += BIT32SZ;
	PBIT32(p,0);
	p+=BIT32SZ;
	return p;
}

//! Return string size plus length
/*! \internal Returns number of characters in \p s plus two bytes for length.  This is the length the string would occupy if converted to a Pascal string using pstring().

\param s String to examine
\return Size of "Pascalised" string
*/
static u16 stringsz(char *s)
{
	if(s == nil)
		return BIT16SZ;

	return BIT16SZ+strlen(s);
}

u16 sizeS2M(estyx_fcall_t *f)
{
	u16 n;

	n = 0;
	n += BIT32SZ;	/* size */
	n += BIT8SZ;	/* type */
	n += BIT16SZ;	/* tag */

	switch(f->type)
	{
	default:
		return 0;
#if 0
	case Tversion:
		n += BIT32SZ;
		n += stringsz(f->u.version.version);
		break;

	case Tflush:
		n += BIT16SZ;
		break;

	case Tauth:
		n += BIT32SZ;
		n += stringsz(f->u.attach.uname);
		n += stringsz(f->u.attach.aname);
		break;

	case Tattach:
		n += BIT32SZ;
		n += BIT32SZ;
		n += stringsz(f->u.attach.uname);
		n += stringsz(f->u.attach.aname);
		break;

	case Twalk:
		n += BIT32SZ;
		n += BIT32SZ;
		n += BIT16SZ;
		for(i=0; i<f->u.twalk.nwname; i++)
			n += stringsz(f->u.twalk.wname[i]);
		break;

	case Topen:
		n += BIT32SZ;
		n += BIT8SZ;
		break;

	case Tcreate:
		n += BIT32SZ;
		n += stringsz(f->u.create.name);
		n += BIT32SZ;
		n += BIT8SZ;
		break;

	case Tread:
		n += BIT32SZ;
		n += BIT64SZ;
		n += BIT32SZ;
		break;

	case Twrite:
		n += BIT32SZ;
		n += BIT64SZ;
		n += BIT32SZ;
		n += f->u.rw.count;
		break;

	case Tclunk:
	case Tremove:
		n += BIT32SZ;
		break;

	case Tstat:
		n += BIT32SZ;
		break;

	case Twstat:
		n += BIT32SZ;
		n += BIT16SZ;
		n += f->u.stat.nstat;
		break;
/*
 */
#endif
	case Rversion:
		n += BIT32SZ;
		n += stringsz(f->u.version.version);
		break;

	case Rerror:
		n += stringsz(f->u.ename);
		break;

	case Rflush:
		break;

	case Rauth:
		n += QIDSZ;
		break;

	case Rattach:
		n += QIDSZ;
		break;

	case Rwalk:
		n += BIT16SZ;
		n += f->u.rwalk.nwqid*QIDSZ;
		break;

	case Ropen:
	case Rcreate:
		n += QIDSZ;
		n += BIT32SZ;
		break;

	case Rread:
		n += BIT32SZ;
		//n += f->u.rw.count;
		break;

	case Rwrite:
		n += BIT32SZ;
		break;

	case Rclunk:
		break;

	case Rremove:
		break;

	case Rstat:
		n += BIT16SZ;
		n += f->u.stat.nstat;
		break;

	case Rwstat:
		break;
	}
	return n;
}

u16 convS2M(estyx_fcall_t *f, u8 **ap)
{
	u8 *p;
	u16 i, size;

	size = sizeS2M(f);
	if(size == 0) return 0;
	if (!ap) return 0;
	//*ap=estyx_malloc(size);
	if (!*ap) return 0;
	p = *ap;

	if (f->type==Rread) {
		PBIT32(p, size+f->u.rw.count);
	} else {
		PBIT32(p, size);
	}
	p += BIT32SZ;
	PBIT8(p, f->type);
	p += BIT8SZ;
	PBIT16(p, f->tag);
	p += BIT16SZ;

	switch(f->type)
	{
	default:
		return 0;
#if 0
// we're a server; we never send T-messages
	case Tversion:
		PBIT32(p, f->u.version.msize);
		p += BIT32SZ;
		p = pstring(p, f->u.version.version);
		break;

	case Tflush:
		PBIT16(p, f->u.oldtag);
		p += BIT16SZ;
		break;

	case Tauth:
		PBIT32(p, f->u.attach.afid);
		p += BIT32SZ;
		p  = pstring(p, f->u.attach.uname);
		p  = pstring(p, f->u.attach.aname);
		break;

	case Tattach:
		PBIT32(p, f->fid);
		p += BIT32SZ;
		PBIT32(p, f->u.attach.afid);
		p += BIT32SZ;
		p  = pstring(p, f->u.attach.uname);
		p  = pstring(p, f->u.attach.aname);
		break;

	case Twalk:
		PBIT32(p, f->fid);
		p += BIT32SZ;
		PBIT32(p, f->u.twalk.newfid);
		p += BIT32SZ;
		PBIT16(p, f->u.twalk.nwname);
		p += BIT16SZ;
		if(f->u.twalk.nwname > MAXWELEM)
			return 0;
		for(i=0; i<f->u.twalk.nwname; i++)
			p = pstring(p, f->u.twalk.wname[i]);
		break;

	case Topen:
		PBIT32(p, f->fid);
		p += BIT32SZ;
		PBIT8(p, f->u.create.mode);
		p += BIT8SZ;
		break;

	case Tcreate:
		PBIT32(p, f->fid);
		p += BIT32SZ;
		p = pstring(p, f->u.create.name);
		PBIT32(p, f->u.create.perm);
		p += BIT32SZ;
		PBIT8(p, f->u.create.mode);
		p += BIT8SZ;
		break;

	case Tread:
		PBIT32(p, f->fid);
		p += BIT32SZ;
		PBIT32(p, f->u.rw.offset);
		p += BIT32SZ;
		PBIT32(p,0);
		p+=BIT32SZ;
		PBIT32(p, f->u.rw.count);
		p += BIT32SZ;
		break;

	case Twrite:
		PBIT32(p, f->fid);
		p += BIT32SZ;
		PBIT32(p, f->u.rw.offset);
		p += BIT32SZ;
		PBIT32(p,0);
		p+=BIT32SZ;
		PBIT32(p, f->u.rw.count);
		p += BIT32SZ;
		memmove(p, f->u.rw.data, f->u.rw.count);
		p += f->u.rw.count;
		break;

	case Tclunk:
	case Tremove:
		PBIT32(p, f->fid);
		p += BIT32SZ;
		break;

	case Tstat:
		PBIT32(p, f->fid);
		p += BIT32SZ;
		break;

	case Twstat:
		PBIT32(p, f->fid);
		p += BIT32SZ;
		PBIT16(p, f->u.stat.nstat);
		p += BIT16SZ;
		memmove(p, f->u.stat.stat, f->u.stat.nstat);
		p += f->u.stat.nstat;
		break;
#endif
/*
 */

	case Rversion:
		PBIT32(p, f->u.version.msize);
		p += BIT32SZ;
		p = pstring(p, f->u.version.version);
		break;

	case Rerror:
		p = pstring(p, f->u.ename);
		break;

	case Rflush:
		break;

	case Rauth:
		p = pqid(p, &f->u.aqid);
		break;

	case Rattach:
		p = pqid(p, &f->u.open.qid);
		break;

	case Rwalk:
		PBIT16(p, f->u.rwalk.nwqid);
		p += BIT16SZ;
		if(f->u.rwalk.nwqid > MAXWELEM)
			return 0;
		for(i=0; i<f->u.rwalk.nwqid; i++)
			p = pqid(p, &f->u.rwalk.wqid[i]);
		break;

	case Ropen:
	case Rcreate:
		p = pqid(p, &f->u.open.qid);
		PBIT32(p, f->u.open.iounit);
		p += BIT32SZ;
		break;

	case Rread:
		PBIT32(p, f->u.rw.count);
		p += BIT32SZ;
		//-- we pass data separately -- see wr()
		//memmove(p, f->u.rw.data, f->u.rw.count);
		//p += f->u.rw.count;
		break;

	case Rwrite:
		PBIT32(p, f->u.rw.count);
		p += BIT32SZ;
		break;

	case Rclunk:
		break;

	case Rremove:
		break;

	case Rstat:
		PBIT16(p, f->u.stat.nstat);
		p += BIT16SZ;
		memmove(p, f->u.stat.stat, f->u.stat.nstat);
		p += f->u.stat.nstat;
		break;

	case Rwstat:
		break;
	}
	if(size != p-*ap)
		return 0;
	return size;
}
