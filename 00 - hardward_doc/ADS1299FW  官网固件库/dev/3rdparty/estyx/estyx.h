
#ifndef GUARD_estyx_h
#define GUARD_estyx_h

//:wrap=soft:

#include "types.h"

#define	VERSION9P "9P2000.USB.estyx"

#define ESTYX_DIR_IOUNIT 2048

#define Qroot	0

#define	OREAD	0	/* open for read */
#define	OWRITE	1	/* write */
#define	ORDWR	2	/* read and write */
#define	OEXEC	3	/* execute, == read but check execute permission */
#define	OTRUNC	16	/* or'ed in (except for exec), truncate file first */
#define	OCEXEC	32	/* or'ed in, close on exec */
#define	ORCLOSE	64	/* or'ed in, remove on close */
#define	OEXCL	0x1000	/* or'ed in, exclusive use (create only) */

#define	AEXIST	0	/* accessible: exists */
#define	AEXEC	1	/* execute access */
#define	AWRITE	2	/* write access */
#define	AREAD	4	/* read access */

#define	GBIT8(p)	((u8)((p)[0]))
#define	GBIT16(p)	( ((u16)((p)[0])) | ((u16)((p)[1])<<8) )
#define	GBIT32(p)	( ((u32)((p)[0])) |\
	((u32)((p)[1])<<8) |\
	((u32)((p)[2])<<16) |\
	((u32)((p)[3])<<24) )

#if 0
s64 GBIT64(u8 *p)
{
	s64 d;

	d.l=GBIT32(p);
	d.h=GBIT32(p);
}
#define	GBIT64(p)	((long long)((p)[0]|((p)[1]<<8)|((p)[2]<<16)|((p)[3]<<24)) |\
				((long long)((p)[4]|((p)[5]<<8)|((p)[6]<<16)|((p)[7]<<24)) << 32))
#endif

#define	PBIT8(p,v)	(p)[0]=((u8)v)
#define	PBIT16(p,v)	(p)[0]=((u16)v)&0xff;(p)[1]=(((u16)v)>>8)&0xff
#define	PBIT32(p,v)	(p)[0]=((u32)v)&0xff;\
	(p)[1]=(((u32)v)>>8)&0xff;\
	(p)[2]=(((u32)v)>>16)&0xff;\
	(p)[3]=(((u32)v)>>24)&0xff

#if 0
void PBIT64(char *p, s64 v)
{
	PBIT32(p,v.l);
	PBIT32(p,v.h);
}

#define	PBIT64(p,v)	(p)[0]=(v);(p)[1]=(v)>>8;(p)[2]=(v)>>16;(p)[3]=(v)>>24;\
			(p)[4]=(v)>>32;(p)[5]=(v)>>40;(p)[6]=(v)>>48;(p)[7]=(v)>>56
#endif

#define	BIT8SZ		1
#define	BIT16SZ		2
#define	BIT32SZ		4
#define	BIT64SZ		8
#define	QIDSZ	(BIT8SZ+BIT32SZ+BIT64SZ)

/* STATFIXLEN includes leading 16-bit count */
/* The count, however, excludes itself; total size is BIT16SZ+count */
#define STATFIXLEN	(BIT16SZ+QIDSZ+5*BIT16SZ+4*BIT32SZ+1*BIT64SZ)	/* amount of fixed length data in a stat buffer */

#define	NOTAG		(u16)~0U	/* Dummy tag */
#define	NOFID		(u32)~0U	/* Dummy fid */
#define	IOHDRSZ		24	/* ample room for Twrite/Rread header (iounit) */

enum {
	Tversion =	100,
	Rversion,
	Tauth =		102,
	Rauth,
	Tattach =	104,
	Rattach,
	Terror =	106,	/* illegal */
	Rerror,
	Tflush =	108,
	Rflush,
	Twalk =		110,
	Rwalk,
	Topen =		112,
	Ropen,
	Tcreate =	114,
	Rcreate,
	Tread =		116,
	Rread,
	Twrite =	118,
	Rwrite,
	Tclunk =	120,
	Rclunk,
	Tremove =	122,
	Rremove,
	Tstat =		124,
	Rstat,
	Twstat =	126,
	Rwstat,
	Tmax
};

#define nil		((void*)0)

#define	MAXWELEM	16

/* bits in Qid.type */
#define QTDIR		0x80		/* type bit for directories */
#define QTAPPEND	0x40		/* type bit for append only files */
#define QTEXCL		0x20		/* type bit for exclusive use files */
#define QTMOUNT		0x10		/* type bit for mounted channel */
#define QTAUTH		0x08		/* type bit for authentication file */
#define QTFILE		0x00		/* plain file */

/* bits in estyx_dir_t.mode */
#define DMDIR		0x80000000	/* mode bit for directories */
#define DMAPPEND	0x40000000	/* mode bit for append only files */
#define DMEXCL		0x20000000	/* mode bit for exclusive use files */
#define DMMOUNT		0x10000000	/* mode bit for mounted channel */
#define DMAUTH		0x08000000	/* mode bit for authentication file */
#define DMREAD		0x4		/* mode bit for read permission */
#define DMWRITE		0x2		/* mode bit for write permission */
#define DMEXEC		0x1		/* mode bit for execute permission */

typedef struct estyx_server_t	estyx_server_t;
typedef struct estyx_ops_t	estyx_ops_t;
typedef struct estyx_fops_t	estyx_fops_t;
typedef struct estyx_file_t	estyx_file_t;
//typedef struct estyx_client_t	estyx_client_t;
typedef struct estyx_fid_t	estyx_fid_t;
typedef struct estyx_dir_t	estyx_dir_t;
typedef struct estyx_fcall_t	estyx_fcall_t;
typedef struct Walkqid		Walkqid;

//! Transmitter callback
/*! estyx uses this callback to transmit packets.  It should be handled by code capable of marshalling the packets onto a bus.

This callback uses two data pointers.  The first, \p msg, is always present and contains the Styx packet itself.  The other, \p data, is used only for Rread transactions.  It contains the data portion.

There are two length parameters, both given in bytes.  \p len is the length of \p msg.  \p len does not include the length of \p data.  \p datalen gives the length of \p data.

This is done for performance reasons.  Data is obtained from file callbacks, while the packet header is constructed by estyx.  The data must be concatenated to the packet header, which requires a potentially expensive copy operation.  Since this copy must be performed a second time in the USB hardware, we require the USB driver to perform the concatenation at that time.  Often, the copying can be combined at that time.

The \p data pointer is user-supplied, and is neither owned nor freed by estyx.  The \p msg pointer is owned by estyx.

Since estyx does not spawn threads, this call can only be made when an estyx call is made.  A user application may, however, use multiple threads, and this means that this call is not guaranteed to be made in a particular thread.

Currently, estyx will make this call in one of two situations:

- In response to a call to estyx_rx().  This is the most common situation.

- In response to a call from the filesystem layer.  This may happen for delayed replies or split read transactions.

Since the filesystem backend may operate a separate thread from the I/O frontend, the call may be placed from either the filesystem thread or the I/O thread, if these are separate.  This may give rise to a resource conflict.  If multiple threads are in use, estyx_tx() should contain a mutex locking system.

estyx_tx() may block while waiting for a mutex.  If this is the case, it should implement a timeout, to recover from possible deadlock.

\param server Pointer to server instance under which the callback is made
\param len Length of \p msg
\param msg Styx message
\param datalen Length of \p data
\param data Data block for Rread messages

\retval 0 Packet successfully transmitted
\retval -1 Error occurred -- packet not transmitted
\retval -2 Timeout -- could not access I/O
*/
typedef int (*estyx_tx_cb)(estyx_server_t *server, u16 len, u8 *msg, u16 datalen, u8 *data);

//! File operations structure
/*! This structure contains the calls which estyx makes for files. */
struct estyx_fops_t
{
	/*! Called when estyx reads from the file.  If this is null, estyx returns permission denied on reads.

	This function must call estyx_rread() with a buffer containing no more than \p len bytes.  Fewer bytes than this may be returned, but not more.  If it returns more bytes, the client may issue an error.

	To return EOF, call estyx_rread() with zero \p len or a null buffer pointer.

	estyx_rread() triggers the user-supplied transmit callback (see estyx_tx_cb).  Note that this may retain the buffer for a background transfer.  It is the user's responsibility to ensure that a buffer still in use is not overwritten or freed.

	estyx never calls estyx_rread() automatically.  It must be called from this function.  If it is not, no reply is issued, unless an error code is returned.

	\p pos is the position, in number of bytes from the beginning of the file, at which the read is requested to begin.  The read need not actually begin at \p pos.  For some files, such as streams, \p pos could be ignored.  However, \p pos may need to be checked to determine whether EOF should be returned.

	The function returns an error code.  Zero indicates success, -1 indicates an error, and -2 indicates that data is not yet available, but may be in future.

	\warning estyx may issue an error response if an error code is returned, even if estyx_rread() has been called.  Only return an error if no read response has been sent.

	\note Currently, estyx does nothing if -2 is returned.  Split transactions will be handled in a future release.

	\param[in] f fid being used for writing
	\param[in] len Length of data
	\param[in] pos Read position

	\retval 0 Success
	\retval -1 Error
	\retval -2 Data not yet available
	
	\todo Implement split transactions
	*/
	int (*read)(estyx_fid_t *f, u16 len, u32 pos);

	/*! Called when estyx writes to the file.  If this is null, estyx 
	returns permission denied on writes.

	\p len is the amount of data to be written.  Fewer bytes than this may be written, but not more.  The function must return the number of bytes actually read.  If it returns a number greater than \p len, estyx issues an I/O error.
	
	\p data points to \p len bytes of data.

	\p pos is the position, in number of bytes from the beginning of the file, at which the write is requested to begin.  The read need not actually begin at \p pos; in particular, for device files, this parameter may have no meaning.

	\param[in] f fid being used for writing
	\param[in,out] len Length
	\param[in] data Data
	\param[in] pos Write position

	\retval 0 No data written (EOF)
	\retval >0 Number of bytes written
	\retval -1 Error
	*/
	int (*write)(estyx_fid_t *f, u16 *len, u8 *data, u32 pos);

	/*! Called when estyx opens the file.  If this is null, an open always succeeds.
	
	If this returns non-zero, estyx returns permission denied.

	\p mode is a byte describing the open mode.  Bits 0-1 are the requested access mode and can take one of four values:

	- 0 (OREAD) -- read access
	- 1 (OWRITE) -- write access
	- 2 (ORDWR) -- read and write access
	- 3 (OEXEC) -- execute access

	estyx checks the access mode against available file operations before calling open().  Read access is not permitted on a file with no read operation, and write access is not permitted on a file with no write operation.  Once the file has been opened, estyx will not permit read() to be called if the access mode is OWRITE, and it will not permit write() to be called if the access mode is OREAD.

	OEXEC is not used by estyx.

	Bit 4 (ORTRUNC), if set, requests that the file be truncated to zero on open, for OWRITE and ORDWR.  For OREAD, this bit will be zero.

	All other bits in \p mode should be ignored.

	\param[in] f fid being used for open
	\param[out] iounit iounit value returned in Ropen.  Default is 65535
	\param[in] mode Open mode, as described above
	*/
	int (*open)(estyx_fid_t *f, u16 *iounit, u8 mode);

	/*! Called when estyx closes the file.  May be null. */
	void (*close)(estyx_fid_t *f);

	/*! Obtain the current length of a file.

	If this is null, the file's length is reported as zero.

	\param f File pointer

	\return Current length of file in bytes
	*/
	u32 (*len)(estyx_file_t *f);
};

typedef struct Qid
{
	u32	path;
	u32	vers;
	u8	type;
} Qid;

struct Walkqid
{
	estyx_fid_t	*clone;
	int	nqid;
	Qid	qid[1];
};

struct estyx_dir_t
{
	/* system-modified data */
	u16	type;	/* server type */
	u16	dev;	/* server subtype */
	/* file data */
	Qid	qid;	/* unique id from server */
// we don't do mod times or users
	u32	mode;	/* permissions */
//	u32	atime;	/* last read time */
//	u32	mtime;	/* last write time */
//	s64	length;	/* file length */
	const char *name;	/* last element of path */
//	char	*uid;	/* owner name */
//	char	*gid;	/* group name */
//	char	*muid;	/* last modifier name */
};

struct estyx_file_t
{
	void *u;		// user pointer
	estyx_dir_t d;		// dir struct: name, perms, etc
	estyx_file_t *parent;	// filesystem stuff
	estyx_file_t *child;
	estyx_file_t *sibling;
	estyx_file_t *next;
	int ref;
	int open;
	estyx_fops_t ops;	// operations
	u16 iounit;
};

struct estyx_server_t
{
	estyx_ops_t *ops;
	int qidgen;
	int connfd;
	int needfile;
// we don't distinguish multiple clients
//	styx_client_t *clients;
//	styx_client_t *curc;
	estyx_file_t *root;
	estyx_file_t **ftab; // filesystem table
	estyx_tx_cb txcb; // transmit callback
	estyx_fid_t *fids;
	void *priv;	/* private */
};

struct estyx_fid_t
{
	estyx_fid_t *next;
	short	fid;
	u16	open;
	u16	mode;	/* read/write */
	//u32	offset;	/* in file */
	int	dri;		/* dirread index */
	u16	drofs;
	Qid	qid;
	estyx_server_t *server;
	estyx_file_t *file;
	estyx_fcall_t *fcall;
};

struct estyx_fcall_t
{
	u8	type;
	u32	fid;
	u16	tag;
	union {
		struct {
			u32	msize;		/* Tversion, Rversion */
			char	*version;	/* Tversion, Rversion */
		} version;
		u16	oldtag;		/* Tflush */
		char	*ename;		/* Rerror */
		struct {
			Qid	qid;		/* Rattach, Ropen, Rcreate */
			u32	iounit;		/* Ropen, Rcreate */
		} open;
		Qid	aqid;		/* Rauth */
		struct {
			u32	afid;		/* Tauth, Tattach */
			char	*uname;		/* Tauth, Tattach */
			char	*aname;		/* Tauth, Tattach */
		} attach;
		struct {
			u32	perm;		/* Tcreate */ 
			char	*name;		/* Tcreate */
			u8	mode;		/* Tcreate, Topen */
		} create;
		struct {
			u32	newfid;		/* Twalk */
			u16	nwname;		/* Twalk */
			char	*wname[MAXWELEM];	/* Twalk */
		} twalk;
		struct {
			u16	nwqid;		/* Rwalk */
			Qid	wqid[MAXWELEM];		/* Rwalk */
		} rwalk;
		struct {
			u32	offset;		/* Tread, Twrite */
			u32	count;		/* Tread, Twrite, Rread */
			u8	*data;		/* Twrite, Rread */
		} rw;
		struct {
			u16	nstat;		/* Twstat, Rstat */
			u8	*stat;		/* Twstat, Rstat */
		} stat;
	} u;
};

//! Protocol operations structure
struct estyx_ops_t
{
//	char *(*newclient)(styx_client_t *c);
//	char *(*freeclient)(styx_client_t *c);

	char *(*attach)(char *uname, char *aname);
	char *(*walk)(Qid *qid, char *name);
	char *(*open)(Qid *qid, int mode);
	char *(*create)(Qid *qid, char *name, int perm, int mode);
	char *(*close)(Qid qid, int mode);
	char *(*remove)(Qid qid);
	char *(*stat)(Qid qid, estyx_dir_t *d);
	char *(*wstat)(Qid qid, estyx_dir_t *d);
};

//! Calculate size of a serialised Fcall
/*! \internal Returns the number of bytes that the given Fcall would occupy as a Styx message.

\param f Fcall to inspect

\return Number of bytes in serialised Fcall
*/
u16 sizeS2M(estyx_fcall_t *f);

//! Convert an Fcall into a serialised message
/*! \internal Converts Fcall \p f into a Styx message.  The message is placed in a dynamically allocated buffer, which is returned in \p ap.

Allocation is performed using estyx_malloc().

\param f Fcall structure to convert
\param ap Pointer to data buffer
*/
u16 convS2M(estyx_fcall_t *f, u8 **ap);

//! Calculate size of a serialised Dir
u16 sizeD2M(estyx_dir_t *d);

//! Serialise Dir structure
/*!
\internal

\param[in] d Dir structure to serialise
\param[in] buf Buffer to place data in
\param[in] nbuf Length of buffer
\param[in] len Current length of file
*/
u16 convD2M(estyx_dir_t *d, u8 *buf, u16 nbuf, u32 len);

//! Convert Styx message to fcall structure
/*! \internal Converts a Styx message to an fcall structure.

\note This function performs no syntactic checks.
three causes for error:
 1. message size field is incorrect
 2. input buffer too short for its own data (counts too long, etc.)
 3. too many names or qids
gqid() and gstring() return nil if they would reach beyond buffer.
main switch statement checks range and also can fall through
to test at end of routine.

\param ap Pointer to packet
\param nap Length of packet
\param f fcall structure to fill in

\retval 0 \p f is filled in; packet was correct
\retval ~0 packet was not correct, \p f is not filled in
 */
u16 convM2S(u8 *ap, u16 nap, estyx_fcall_t *f);

u16 convM2D(u8 *buf, u16 nbuf, estyx_dir_t *d, char *strs);
int statcheck(u8 *buf, u16 nbuf);

//! Allocate memory
/*! This function is not defined in the estyx library.  It must be supplied by user code.

Allocates a block of memory.  Returns a pointer to it, or null if the allocation failed.

Unlike malloc(), this function takes a number of bytes as its size specifier.  User code must allocate a block containing at least the requested number of bytes, or fail by returning null.

\param s Number of bytes to allocate

\retval 0 Could not allocate block
\retval ~0 Pointer to allocated block
*/
void *estyx_malloc(u32 bytes);

//! Free memory
/*! This function is not defined in the estyx library.  It must be supplied by user code.

Frees memory allocated with estyx_malloc().

This function must ignore invalid or null pointers.

\param p Memory to be freed
*/
void estyx_free(void *p);

//! Complete a read operation
/*! Triggers the user-supplied transmit callback (see estyx_tx_cb).

estyx never calls estyx_rread() automatically.  It must be called from the read callback.  If it is not, no reply is issued, unless an error code is returned.

Note that the transmit callback may retain \p buf for a background transfer.  It is the user's responsibility to ensure that a buffer still in use is not overwritten or freed.
*/
int estyx_rread(estyx_fid_t *fp, u16 len, u8 *buf);

#ifdef C55X
int estyx_rread55(estyx_fid_t *f,u16 len,u8 *data);
#endif

//! Check file permissions
/*! Checks the permissions of \p f against \p mode.

If \p f is a directory, its permissions are taken to be 0555 (read and execute).

If \p f is a file, its operations table is checked.  If it has a read procedure, it receives read permission; if it has a write procedure, it receives write function.

\p mode may be one of OREAD, OWRITE, ORDWR, and OEXEC.

In this implementation, \p uid is ignored.  It is retained for compatibility.

\param f File to check
\param uid ID of user performing the check (ignored)
\param mode Permission requested

\return 0 if permission denied, non-zero otherwise
*/
int estyx_perm(estyx_file_t *f, char *uid, int mode);

estyx_file_t *estyx_findfile(estyx_server_t *server, int path);

//! Process a data packet
/*! The packet in \p buf is processed.  In response, estyx will usually call estyx_tx.

With one exception, the packet must be complete.  If the packet is not complete, this function returns -1.

The exception is Twr.  The data block contained in this packet can be fed to the filesystem in pieces.  When estyx receives an incomplete Twr packet, it interprets subsequent packets as continuations, until all of the expected data has been received.

A call to estyx_reset() will terminate this and cause estyx to resume listening for new packets.

\p len is the amount of "good" data in the buffer.  It should represent the length of received data.

The 16-bit size of len limits the buffer size to 65,535 bytes.  Zero is a legal value and causes the packet to be ignored.

When this call returns, \p buf is guaranteed to be available for reuse on subsequent calls; estyx will not continue using the buffer.

\param server Server instance
\param len Maximum length of \p buf
\param buf Buffer containing packet
*/
int estyx_rx(estyx_server_t *s, u16 len, u8 *buf);

//! Reset estyx
/*! Resets any pending transactions and returns estyx to its "ground state".

Any necessary cleanup on the filesystem side is also performed.
*/
//int estyx_reset(estyx_server_t *server);

//! Set the transmit callback
/*! Sets the transmit callback to \p cb.  If \p cb is null, this function has no effect. */
void estyx_set_tx_cb(estyx_server_t *server, estyx_tx_cb cb);

//! Initialise estyx
/*! Initialises estyx.  This must be called before any other estyx functions.

Following this call, the filesystem contains the root directory, 

\param server Pointer to an empty estyx_server_t struct.  This must be passed to most estyx calls.
\param ops Structure containing default operations.  See estyx_ops_t for details.  It may be null, in which case estyx supplies its own operations.
\param txcb Transmit callback.  See estyx_tx_cb for details.
*/
int estyx_init(estyx_server_t *server, estyx_ops_t *ops, estyx_tx_cb txcb);

//! Create a file
/*! Creates a file named \p name in the directory identified by \p parent.

\p parent must exist and must be a directory, or must be 0.  If \p parent is 0, the file is created in the root directory.

\p fops is a function table pointing to read and write calls to be used with the file.  See estyx_fops_t for details.

\p u is a void pointer passed to the file's callbacks.

\param server Server instance
\param parent Parent node
\param name New filename
\param fops File operations structure
\param u User value

\return Pointer to the new file structure, or null if an error occured
*/
estyx_file_t *estyx_mkfile(estyx_server_t *server, estyx_file_t *parent, 
	const char *name, estyx_fops_t *fops, void *u);

//! Create a directory
/*! Creates a directory named \p name having Qid \p qid in the tree in the directory identified by \p pqid.

\p parent must exist and must be a directory, or must be 0.  If \p parent is 0, the directory is created in the root directory.

\param server Server instance
\param parent Parent node
\param name Name of the new directory

\return Pointer to the new file structure, or null if an error occured
*/
estyx_file_t *estyx_mkdir(estyx_server_t *server, estyx_file_t *parent,
	const char *name);

u16 estyx_calclen(u32 datasize, u32 offset, u16 requested);

//! Change read and write permissions on a file
/*! Changes the read and write permissions on a file.

If \p r is non-zero and the file has a read function, read permission is enabled; otherwise, it is disabled.  If \p w is non-zero and the file has a write function, write permission is enabled; otherwise, it is disabled.

Execute permission is not affected (in estyx, files never have execute permission).  This function alters group, owner, and world permissions.

estyx checks permissions only when a file is opened.  This function therefore does not cause access to be disabled or enabled on files which are already open.  Subsequent opens will be affected.

This function does not operate on directories; in estyx, directories always have world read and execute permissions.  If \p f is a directory, this function does nothing.

\param[in] f File to alter permissions on
\param[in] r Whether to grant read permission
\param[in] w Whether to grant write permission
*/
void estyx_setrw(estyx_file_t *f, int r, int w);

//! See whether a file can be read
/*! Returns non-zero if \p f has read permission, zero otherwise.

\param[in] f File to query
\return Non-zero if read allowed, zero if not
*/
int estyx_canread(estyx_file_t *f);

//! See whether a file can be written
/*! Returns non-zero if \p f has write permission, zero otherwise.

\param[in] f File to query
\return Non-zero if write allowed, zero if not
*/
int estyx_canwrite(estyx_file_t *f);

//! Detach client
/*! Performs any actions necessary when the client detaches.

Currently, this only deletes all fids.

\param[in] s Server to detach
*/
void estyx_detach(estyx_server_t *s);

//! Destroy server 
/*! Performs any actions necessary when the server is destroyed.
 * frees fids, files and anything allocated

\param[in] s Server to destroy 
*/
void estyx_destroy(estyx_server_t *s);


#endif
