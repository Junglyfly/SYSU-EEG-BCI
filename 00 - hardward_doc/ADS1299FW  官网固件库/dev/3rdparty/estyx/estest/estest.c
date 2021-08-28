
/* estyx tester.

File structure:

/
	msg - returns the string "estyx test"; cannot be written
	inc - returns a larger number each time read; cannot be written
	str - stores a string up to 64 characters, which can be read back; 
	      is seekable
	no_open - file that can't be opened
	lg/ - directory, containing files for "large" tests
		datain - accepts a large amount of data; cannot be read
		dataincrc - returns latest crc of written data, in hex
		dataout - data can be read from this
		dataoutcrc - latest crc of read data, in hex
	util/ - files for testing the utility nodes (utilfiles.c)
		msg
		bool
		data
		s32
		u32
		u32hex
		u32hex2 - fixed-length hex output, same no. as u32hex.
		u32hex4
		u32hex6
		u32hex8
		u32hex9
		boolr - sets bool's read permission
		boolw - sets bool's write permission
		enum
		float - floating pt i/o
*/

#include "estest.h"
#include "../utilfiles.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void *estyx_malloc(u32 s)
{
	return malloc(sizeof(u8)*s);
}

void estyx_free(void *p)
{
	free(p);
}

static int file_msg_read(estyx_fid_t *f, u16 len, u32 pos)
{
	return estyx_rread(f,estyx_calclen(10,pos,len),"estyx test"+pos);
}

static u32 file_msg_len(estyx_file_t *f)
{
	return 10;
}

static const estyx_fops_t file_msg_ops={
	file_msg_read,
	0,
	0,
	0,
	file_msg_len
};

static int file_inc_data=0;

static int file_inc_read(estyx_fid_t *f, u16 len, u32 pos)
{
	u16 slen;
	char buf[10];

	if (pos) return estyx_rread(f,0,0);
	slen=sprintf(buf,"%d",file_inc_data++);
	return estyx_rread(f,estyx_calclen(slen,pos,len),buf+pos);
}

static const estyx_fops_t file_inc_ops={
	file_inc_read,
	0,
	0,
	0,
	0
};

static char file_str_data[64];
static u8 file_str_datalen=0;

static int file_str_read(estyx_fid_t *f, u16 len, u32 pos)
{
	return estyx_rread(f,estyx_calclen(file_str_datalen,pos,len),file_str_data+pos);
}

/* rq length: *len
begin pos: pos
end pos: clamped(pos+*len)
final *len: end pos - pos
final data len: end pos
*/
static int file_str_write(estyx_fid_t *f, u16 *len, u8 *data, u32 pos)
{
	u32 end;

	end=pos+*len;
	if (pos>64) pos=64;
	if (end>64) end=64;
	if (end>pos) {
		*len=end-pos;
		memcpy(file_str_data+pos,data,*len);
	} else
		*len=0;
	file_str_datalen=end;
	return 0;
}

static u32 file_str_len(estyx_file_t *f)
{
	return file_str_datalen;
}

int file_str_open(estyx_fid_t *f, u16 *iounit, u8 mode)
{
	if (mode&OTRUNC) file_str_datalen=0;
	return 0;
}

static const estyx_fops_t file_str_ops={
	file_str_read,
	file_str_write,
	file_str_open,
	0,
	file_str_len
};

static const estyx_fops_t file_no_open_ops={
	0,
	0,
	0,
	0,
	0
};

// from crc32.c
unsigned long crc32(unsigned long crc, unsigned char *buf, unsigned int len);

static u32 file_lg_datain_crc;
static u32 file_lg_dataout_crc;

static int file_lg_datain_write(estyx_fid_t *f, u16 *len, u8 *data, u32 pos)
{
	file_lg_datain_crc=crc32(file_lg_datain_crc,data,*len);
	return 0;
}

static const estyx_fops_t file_lg_datain_ops={
	0,
	file_lg_datain_write,
	0,
	0,
	0
};

static int file_lg_dataincrc_read(estyx_fid_t *f, u16 len, u32 pos)
{
	char buf[9];

	if (len) sprintf(buf,"%08.8lX",file_lg_datain_crc);
	return estyx_rread(f,estyx_calclen(9,pos,len),buf+pos);
}

static u32 file_lg_dataincrc_len(estyx_file_t *f)
{
	return 8;
}

static const estyx_fops_t file_lg_dataincrc_ops={
	file_lg_dataincrc_read,
	0,
	0,
	0,
	file_lg_dataincrc_len
};

// random gens from *DOCTOR* George Marsaglia
#define znew   (z=36969*(z&65535)+(z>>16))
#define wnew   (w=18000*(w&65535)+(w>>16))
#define MWC    ((znew<<16)+wnew )

/* Generates len bytes in buf */
static void genrnd(unsigned char *buf, u16 len)
{
	static u32 z=362436069, w=521288629;

	while (len) {
		*buf++=MWC;
		len--;
	}
}

static int file_lg_dataout_read(estyx_fid_t *f, u16 len, u32 pos)
{
	u8 *data=estyx_malloc(len);
	if (!data) return -1;
	genrnd(data,len);
	file_lg_dataout_crc=crc32(file_lg_dataout_crc,data,len);
	estyx_rread(f,len,data);
	estyx_free(data);
	return 0;
}

static const estyx_fops_t file_lg_dataout_ops={
	file_lg_dataout_read,
	0,
	0,
	0,
	0
};

static int file_lg_dataoutcrc_read(estyx_fid_t *f, u16 len, u32 pos)
{
	char buf[9];

	if (len) sprintf(buf,"%08.8lX",file_lg_dataout_crc);
	return estyx_rread(f,estyx_calclen(9,pos,len),buf+pos);
}

static u32 file_lg_dataoutcrc_len(estyx_file_t *f)
{
	return 8;
}

static const estyx_fops_t file_lg_dataoutcrc_ops={
	file_lg_dataoutcrc_read,
	0,
	0,
	0,
	file_lg_dataoutcrc_len
};

static int boolval=0;

static int boolval_get(void)
{
	return boolval;
}

static void boolval_set(int d)
{
	boolval=d;
}

static char data[64];

static void data_get(u16 *len, char **d)
{
	if (*len>64) *len=64;
	*d=data;
}

static void data_set(u16 *len, char *d)
{
	if (*len>64) *len=64;
	memcpy(data,d,*len);
}

static s32 s32data=0;
static u32 u32data=0;
static u32 u32hexdata=0;

static s32 s32_get(void)
{
	return s32data;
}

static void s32_set(s32 d)
{
	s32data=d;
}

static u32 u32_get(void)
{
	return u32data;
}

static void u32_set(u32 d)
{
	u32data=d;
}

static u32 u32hex_get(void)
{
	return u32hexdata;
}

static void u32hex_set(u32 d)
{
	u32hexdata=d;
}

static float float_val;

static float float_get(void)
{
	return float_val;
}

static void float_set(float f)
{
	float_val=f;
}

static estyx_file_t *boolfile;

static void boolr_set(int i)
{
	estyx_setrw(boolfile,i,estyx_canwrite(boolfile));
	printf("%lo\n",boolfile->d.mode);
}

static void boolw_set(int i)
{
	estyx_setrw(boolfile,estyx_canread(boolfile),i);
}

static int enumval=0;

static int enum_get(void)
{
	return enumval;
}

static void enum_set(int i)
{
	enumval=i;
}

static estyx_server_t srv;

static const char *enumlist1[]={"apple","pear","plum","cherry",
	"waaaaay_toooo_loooongggg","more extra stuff etc. blah",0};
static const char *enumlist2[]={"goat","sheep","ox","cow","mmmbbbaaaaaaaaaaaaaaa",0};

estyx_server_t* estest_init(estyx_tx_cb txcb)
{
	int err,i;
	estyx_file_t *lg, *util;

	file_lg_datain_crc=0;
	file_lg_dataout_crc=0;

	for (i=0;i<64;++i) data[i]='.';

	if (err=estyx_init(&srv,0,txcb)) return 0;
	if (!estyx_mkfile(&srv,0,"msg",(estyx_fops_t *)&file_msg_ops,0)) return 0;
	if (!estyx_mkfile(&srv,0,"inc",(estyx_fops_t *)&file_inc_ops,0)) return 0;
	if (!estyx_mkfile(&srv,0,"str",(estyx_fops_t *)&file_str_ops,0)) return 0;
	if (!estyx_mkfile(&srv,0,"no_open",(estyx_fops_t *)&file_no_open_ops,0)) return 0;
	if (!(lg=estyx_mkdir(&srv,0,"lg"))) return 0;
	if (!estyx_mkfile(&srv,lg,"datain",(estyx_fops_t *)&file_lg_datain_ops,0)) return 0;
	if (!estyx_mkfile(&srv,lg,"dataincrc",(estyx_fops_t *)&file_lg_dataincrc_ops,0)) return 0;
	if (!estyx_mkfile(&srv,lg,"dataout",(estyx_fops_t *)&file_lg_dataout_ops,0)) return 0;
	if (!estyx_mkfile(&srv,lg,"dataoutcrc",(estyx_fops_t *)&file_lg_dataoutcrc_ops,0)) return 0;
	if (!(util=estyx_mkdir(&srv,0,"util"))) return 0;
	if (!(boolfile=estyx_file_bool_make(&srv,util,"bool",boolval_get,boolval_set))) return 0;
	if (!estyx_file_msg_make(&srv,util,"msg","Do not multiply objects without necessity")) return 0;
	if (!estyx_file_data_make(&srv,util,"data",data_get,data_set)) return 0;
	if (!estyx_file_s32_make(&srv,util,"s32",s32_get,s32_set)) return 0;
	if (!estyx_file_u32_make(&srv,util,"u32",u32_get,u32_set,0)) return 0;
	if (!estyx_file_u32_make(&srv,util,"u32hex",u32hex_get,u32hex_set,1)) return 0;
	if (!estyx_file_u32_make(&srv,util,"u32hex2",u32hex_get,u32hex_set,2)) return 0;
	if (!estyx_file_u32_make(&srv,util,"u32hex4",u32hex_get,u32hex_set,4)) return 0;
	if (!estyx_file_u32_make(&srv,util,"u32hex6",u32hex_get,u32hex_set,6)) return 0;
	if (!estyx_file_u32_make(&srv,util,"u32hex8",u32hex_get,u32hex_set,8)) return 0;
	if (!estyx_file_u32_make(&srv,util,"u32hex9",u32hex_get,u32hex_set,9)) return 0;
	if (!estyx_file_bool_make(&srv,util,"boolr",0,boolr_set)) return 0;
	if (!estyx_file_bool_make(&srv,util,"boolw",0,boolw_set)) return 0;
	if (!estyx_file_enum_make(&srv,util,"enum",enum_get,enum_set,enumlist1,0)) return 0;
	if (!estyx_file_enum_make(&srv,util,"enumstr",enum_get,enum_set,enumlist2,1)) return 0;
	if (!estyx_file_float_make(&srv,util,"float",float_get,float_set,"number %06.6f")) return 0;
	return &srv;
}

int estest_rx(u16 len, char *buf)
{
	return estyx_rx(&srv,len,buf);
}
