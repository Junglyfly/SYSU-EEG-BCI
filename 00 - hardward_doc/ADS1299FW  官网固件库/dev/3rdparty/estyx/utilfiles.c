
/* estyx modules

Utility file nodes.

- estyx_file_bool_* -- a file which sets a boolean value
- estyx_file_msg_* -- a read-only file which contains a string
- estyx_file_data_* -- simple set/get for data
- estyx_file_s32_* -- get/set a signed 32-bit integer in decimal
- estyx_file_u32_* -- get/set an unsigned 32-bit integer in dec or hex
- estyx_file_float_* -- get/set a single-precision float

each has a creation routine

*/

#include "utilfiles.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

/* predeclaration */
#ifdef C55X
void usbhw_unpack55(u16 *src, u8 *dest, u16 len);
#endif

struct msg_data_t {
	const char *str;
	int len;
};

static int msg_read(estyx_fid_t *f, u16 len, u32 pos)
{
	struct msg_data_t *d;
	
	d=(struct msg_data_t *)f->file->u;
#ifdef C55X
	return estyx_rread55(f,estyx_calclen(d->len,pos,len),(u8 *)d->str+pos);
#else
	return estyx_rread(f,estyx_calclen(d->len,pos,len),(u8 *)d->str+pos);
#endif
}

static u32 msg_len(estyx_file_t *f)
{
	struct msg_data_t *d;
	
	d=(struct msg_data_t *)f->u;
	return d->len;
}

static const estyx_fops_t msg_ops={
	msg_read,
	0,
	0,
	0,
	msg_len
};

estyx_file_t *estyx_file_msg_make(estyx_server_t *server, 
	estyx_file_t *parent, const char *name, const char *str)
{
	int l=strlen(str);
	struct msg_data_t *d;

	if (!l) return 0;
	d=estyx_malloc(sizeof(struct msg_data_t));
	if (!d) return 0;
	d->str=str;
	d->len=l;
	return estyx_mkfile(server,parent,name,(estyx_fops_t *)&msg_ops,d);
}

struct bool_data_t {
	estyx_fops_t ops;
	estyx_file_bool_get_cb get;
	estyx_file_bool_set_cb set;
	char val;
};

static int bool_read(estyx_fid_t *f, u16 len, u32 pos)
{
	struct bool_data_t *d;

	if (pos) return estyx_rread(f,0,0);
	d=(struct bool_data_t *)f->file->u;
	d->val=d->get()?'1':'0';
#ifdef C55X
	d->val=d->val<<8;
#endif
	return estyx_rread(f,1,(u8 *)&(d->val));
}

static int bool_write(estyx_fid_t *f, u16 *len, u8 *data, u32 pos)
{
	struct bool_data_t *d;
	char c;

#ifdef C55X
	c=(*(u16 *)data)>>8;
#else
	c=*data;
#endif
	if (pos||!*len||(c!='0'&&c!='1')) {
		*len=0;
		return 0;
	}
	d=(struct bool_data_t *)f->file->u;
	*len=1;
	d->set(c=='1');
	return 0;
}

static u32 bool_len(estyx_file_t *f)
{
	return 1;
}

estyx_file_t *estyx_file_bool_make(estyx_server_t *server, 
	estyx_file_t *parent, const char *name,
	estyx_file_bool_get_cb get, estyx_file_bool_set_cb set)
{
	struct bool_data_t *d;
	estyx_file_t *f;

	d=estyx_malloc(sizeof(struct bool_data_t));
	if (!d) return 0;
	d->get=get;
	d->set=set;
	d->ops.read=get? bool_read:0;
	d->ops.write=set?bool_write:0;
	d->ops.open=0;
	d->ops.close=0;
	d->ops.len=bool_len;
	f=estyx_mkfile(server,parent,name,(estyx_fops_t *)&(d->ops),d);
	if (!f) estyx_free(d);
	return f;
}

struct data_data_t {
	estyx_fops_t ops;
	estyx_file_data_get_cb get;
	estyx_file_data_set_cb set;
};

static int data_read(estyx_fid_t *f, u16 len, u32 pos)
{
	struct data_data_t *d;
	char *buf;
	u16 l;

	d=(struct data_data_t *)f->file->u;
	d->get(&l,&buf);
	if (!l) return estyx_rread(f,0,0);
#ifdef C55X
	return estyx_rread55(f,estyx_calclen(l,pos,len),(u8 *)buf);
#else
	return estyx_rread(f,estyx_calclen(l,pos,len),(u8 *)buf);
#endif
}

int data_write(estyx_fid_t *f, u16 *len, u8 *data, u32 pos)
{
	struct data_data_t *d;
#ifdef C55X
	u8 *buf;
#endif

	if (pos||!*len) {
		*len=0;
		return 0;
	}
	d=(struct data_data_t *)f->file->u;
#ifdef C55X
	buf=malloc(*len);
	if (!buf) return -1;
	usbhw_unpack55((u16 *)data,buf,*len);
	d->set(len,(char *)buf);
	free(buf);
#else
	d->set(len,data);
#endif
	return 0;
}

estyx_file_t *estyx_file_data_make(estyx_server_t *server, 
	estyx_file_t *parent, const char *name,
	estyx_file_data_get_cb get, estyx_file_data_set_cb set)
{
	struct data_data_t *d;
	estyx_file_t *f;

	d=estyx_malloc(sizeof(struct data_data_t));
	if (!d) return 0;
	d->get=get;
	d->set=set;
	d->ops.read=get? data_read:0;
	d->ops.write=set?data_write:0;
	d->ops.open=0;
	d->ops.close=0;
	d->ops.len=0;
	f=estyx_mkfile(server,parent,name,(estyx_fops_t *)&(d->ops),d);
	if (!f) estyx_free(d);
	return f;
}

struct float_data_t {
	estyx_fops_t ops;
	estyx_file_float_get_cb get;
	estyx_file_float_set_cb set;
	const char *fmt;
	char val[17];
};

int float_write(estyx_fid_t *f, u16 *len, u8 *data, u32 pos)
{
	struct float_data_t *d;
	char b[17];
	float val;
	char *p, *q;

	if (pos||!*len) {
		*len=0;
		return 0;
	}
	if (*len>16) *len=16;
#ifdef C55X
	usbhw_unpack55((u16 *)data,(u8 *)b,*len);
#else
	memcpy(b,data,*len);
#endif
	b[*len]=0;
	d=(struct float_data_t *)f->file->u;
	q=b;
	while (*q&&isspace(*q)) ++q;
	if (!*q) { *len=0; return 0; }
	val=strtod(q,&p);
	if (p==q) *len=0;
	else d->set(val);
	return 0;
}

int float_read(estyx_fid_t *f, u16 len, u32 pos)
{
	struct float_data_t *d;
	int l;

	if (pos) return estyx_rread(f,0,0);
	d=(struct float_data_t *)f->file->u;
	l=sprintf(d->val,d->fmt,d->get());
	if (l<=0) return estyx_rread(f,0,0);
#ifdef C55X
	return estyx_rread55(f,l,(u8 *)d->val);
#else
	return estyx_rread(f,l,d->val);
#endif
}

estyx_file_t *estyx_file_float_make(estyx_server_t *server, 
	estyx_file_t *parent, const char *name,
	estyx_file_float_get_cb get, estyx_file_float_set_cb set,
	const char *fmt)
{
	struct float_data_t *d;
	estyx_file_t *f;

	d=estyx_malloc(sizeof(struct float_data_t));
	if (!d) return 0;
	d->get=get;
	d->set=set;
	d->ops.read=get?float_read:0;
	d->ops.write=set?float_write:0;
	d->ops.open=0;
	d->ops.close=0;
	d->ops.len=0;
	d->fmt=fmt;
	f=estyx_mkfile(server,parent,name,(estyx_fops_t *)(&(d->ops)),d);
	if (!f) estyx_free(d);
	return f;
}

struct long_data_t {
	estyx_fops_t ops;
	unsigned int sign:1,
		showhex:4;
	union {
		estyx_file_s32_get_cb s;
		estyx_file_u32_get_cb u;
	} get;
	union {
		estyx_file_s32_set_cb s;
		estyx_file_u32_set_cb u;
	} set;
	char val[11];
};

static char hexfmt[]="0x%lX";

int long_read(estyx_fid_t *f, u16 len, u32 pos)
{
	struct long_data_t *d;
	int l;
	char *fmt;

	if (pos) return estyx_rread(f,0,0);
	d=(struct long_data_t *)f->file->u;
	if (d->sign) {
		l=sprintf(d->val,"%ld",d->get.s());
	} else {
		if (d->showhex==0) fmt="%lu";
		else if (d->showhex==1) fmt="0x%lX";
		else {
			fmt=hexfmt;
			//fmt[4]='0'+d->showhex;
		}
		l=sprintf(d->val,fmt,d->get.u());
	}
	if (l<=0) return estyx_rread(f,0,0);
#ifdef C55X
	return estyx_rread55(f,l,(u8 *)d->val);
#else
	return estyx_rread(f,l,d->val);
#endif
}

int long_write(estyx_fid_t *f, u16 *len, u8 *data, u32 pos)
{
	struct long_data_t *d;
	union {
		s32 s;
		u32 u;
	} val;
	char b[11];
	char *p;

	if (pos||!*len) {
		*len=0;
		return 0;
	}
	if (*len>10) *len=10;
#ifdef C55X
	usbhw_unpack55((u16 *)data,(u8 *)b,*len);
#else
	memcpy(b,data,*len);
#endif
	b[*len]=0;
	d=(struct long_data_t *)f->file->u;
	if (d->sign)
		val.s=strtol(b,&p,10);
	else {
		if (b[0]=='0'&&b[1]=='x')
			val.u=strtoul(b+2,&p,16);
		else
			val.u=strtoul(b,&p,10);
	}
	if (d->sign)
		d->set.s(val.s);
	else
		d->set.u(val.u);
	return 0;
}

estyx_file_t *estyx_file_s32_make(estyx_server_t *server, 
	estyx_file_t *parent, const char *name,
	estyx_file_s32_get_cb get, estyx_file_s32_set_cb set)
{
	struct long_data_t *d;
	estyx_file_t *f;

	d=estyx_malloc(sizeof(struct long_data_t));
	if (!d) return 0;
	d->get.s=get;
	d->set.s=set;
	d->ops.read=get?long_read:0;
	d->ops.write=set?long_write:0;
	d->ops.open=0;
	d->ops.close=0;
	d->ops.len=0;
	d->sign=1;
	d->showhex=0;
	f=estyx_mkfile(server,parent,name,(estyx_fops_t *)(&(d->ops)),d);
	if (!f) estyx_free(d);
	return f;
}

estyx_file_t *estyx_file_u32_make(estyx_server_t *server, 
	estyx_file_t *parent, const char *name,
	estyx_file_u32_get_cb get, estyx_file_u32_set_cb set,
	int showhex)
{
	struct long_data_t *d;
	estyx_file_t *f;

	d=estyx_malloc(sizeof(struct long_data_t));
	if (!d) return 0;
	d->get.u=get;
	d->set.u=set;
	d->ops.read=get?long_read:0;
	d->ops.write=set?long_write:0;
	d->ops.open=0;
	d->ops.close=0;
	d->ops.len=0;
	d->sign=0;
	d->showhex=showhex<9?showhex:8;
	f=estyx_mkfile(server,parent,name,(estyx_fops_t *)(&(d->ops)),d);
	if (!f) estyx_free(d);
	return f;
}

struct enum_data_t {
	estyx_fops_t ops;
	estyx_file_enum_get_cb get;
	estyx_file_enum_set_cb set;
	char **strlist;
	int strandind;
	char val[23];
};

int enum_read(estyx_fid_t *f, u16 len, u32 pos)
{
	struct enum_data_t *d;
	int i,si,val,l;
	char **p;
	char s[17];

	if (pos) return estyx_rread(f,0,0);
	d=(struct enum_data_t *)f->file->u;
	si=d->strandind;
	val=d->get();
	i=0;
	p=d->strlist;
	for(i=0;(i!=val)&&*(p++);++i);
	if (!*p) si=0;
	if (si) {
		strncpy(s,*p,16);
		s[16]=0;
		l=sprintf(d->val,"%d (%s)",val,s);
	} else {
		l=sprintf(d->val,"%d",val);
	}
	if (l<=0) return estyx_rread(f,0,0);
#ifdef C55X
	return estyx_rread55(f,l,(u8 *)d->val);
#else
	return estyx_rread(f,l,d->val);
#endif
}

int enum_write(estyx_fid_t *f, u16 *len, u8 *data, u32 pos)
{
	struct enum_data_t *d;
	char b[17];
	char **p;
	char *q;
	int i;

	if (pos||!*len) {
		*len=0;
		return 0;
	}
	if (*len>16) *len=16;
#ifdef C55X
	usbhw_unpack55((u16 *)data,(u8 *)b,*len);
#else
	memcpy(b,data,*len);
#endif
	b[*len]=0;
	d=(struct enum_data_t *)f->file->u;
	p=d->strlist;
	i=0;
	while (*p) {
		if (!strncmp(b,*p,16))
			break;
		++i;
		++p;
	}
	if (!*p) {
		i=strtoul(b,&q,10);
		if (q==b) return 0;
	}
	d->set(i);
	return 0;
}

estyx_file_t *estyx_file_enum_make(estyx_server_t *server, 
	estyx_file_t *parent, const char *name,
	estyx_file_enum_get_cb get, estyx_file_enum_set_cb set,
	const char **strlist, int print_index_and_str)
{
	struct enum_data_t *d;
	estyx_file_t *f;

	d=estyx_malloc(sizeof(struct long_data_t));
	if (!d) return 0;
	d->get=get;
	d->set=set;
	d->ops.read=get? enum_read:0;
	d->ops.write=set?enum_write:0;
	d->ops.open=0;
	d->ops.close=0;
	d->ops.len=0;
	d->strlist=(char **)strlist;
	d->strandind=print_index_and_str;
	f=estyx_mkfile(server,parent,name,(estyx_fops_t *)(&(d->ops)),d);
	if (!f) estyx_free(d);
	return f;
}
