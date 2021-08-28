
#include <string.h>
#include <stdlib.h>
#include "estyx.h"

#if 0
u16 sizeD2M(estyx_dir_t *d)
{
	char *sv[4];
	int i, ns;

	sv[0] = d->name;
	sv[1] = d->uid;
	sv[2] = d->gid;
	sv[3] = d->muid;

	ns = 0;
	for(i = 0; i < 4; i++)
		if(sv[i])
			ns += strlen(sv[i]);

	return STATFIXLEN + ns;
}
#endif

u16 sizeD2M(estyx_dir_t *d)
{
	return STATFIXLEN + strlen(d->name)+18;
}

u16 convD2M(estyx_dir_t *d, u8 *buf, u16 nbuf, u32 len)
{

	u8 *p, *ebuf;
	const char *sv[4];
	int i, ns, nsv[4], ss;

	if(nbuf < BIT16SZ)
		return 0;

	p = buf;
	ebuf = buf + nbuf;

	sv[0] = d->name;
	sv[1] = "nobody";
	sv[2] = "nobody";
	sv[3] = "nobody";

	ns = 0;
	for(i = 0; i < 4; i++){
		if(sv[i])
			nsv[i] = strlen(sv[i]);
		else
			nsv[i] = 0;
		ns += nsv[i];
	}

	ss = STATFIXLEN + ns;

	/* set size befor erroring, so user can know how much is needed */
	/* note that length excludes count field itself */
	PBIT16(p, ss-BIT16SZ);
	p += BIT16SZ;

	if(ss > nbuf)
		return BIT16SZ;

	PBIT16(p, d->type);
	p += BIT16SZ;
	PBIT32(p, d->dev);
	p += BIT32SZ;
	PBIT8(p, d->qid.type);
	p += BIT8SZ;
	PBIT32(p, d->qid.vers);
	p += BIT32SZ;
	PBIT32(p, d->qid.path);
	p += BIT32SZ;
	PBIT32(p,0);
	p += BIT32SZ;
	PBIT32(p, d->mode);
	p += BIT32SZ;
	//PBIT32(p, d->atime);
	PBIT32(p,0);
	p += BIT32SZ;
	//PBIT32(p, d->mtime);
	PBIT32(p,0);
	p += BIT32SZ;
	//PBIT64(p, d->length);
	PBIT32(p,len);
	p += BIT32SZ;
	PBIT32(p,0);
	p += BIT32SZ;

	for(i = 0; i < 4; i++){
		ns = nsv[i];
		if(p + ns + BIT16SZ > ebuf)
			return 0;
		PBIT16(p, ns);
		p += BIT16SZ;
		if(ns)
			memmove(p, sv[i], ns);
		p += ns;
	}

	if(ss != p - buf)
		return 0;

	return p - buf;
}
