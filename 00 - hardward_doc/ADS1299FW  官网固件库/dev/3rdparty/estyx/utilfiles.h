
#ifndef GUARD_utilfiles_h
#define GUARD_utilfiles_h

#include "estyx.h"

// :wrap=soft:

typedef int (*estyx_file_bool_get_cb)(void);
typedef void (*estyx_file_bool_set_cb)(int);
typedef void (*estyx_file_data_get_cb)(u16 *len, char **data);
typedef void (*estyx_file_data_set_cb)(u16 *len, char *data);
typedef s32 (*estyx_file_s32_get_cb)(void);
typedef void (*estyx_file_s32_set_cb)(s32);
typedef u32 (*estyx_file_u32_get_cb)(void);
typedef void (*estyx_file_u32_set_cb)(u32);
typedef float (*estyx_file_float_get_cb)(void);
typedef void (*estyx_file_float_set_cb)(float);
typedef int (*estyx_file_enum_get_cb)(void);
typedef void (*estyx_file_enum_set_cb)(int);

//! Create a message file
/*! Creates a read-only file containing a text message.

\p str is the null-terminated string which the file will contain.  The pointer \p str is kept; the string is not duplicated.  For this reason, \p str is declared const.

The length of the string is calculated at creation time and is used as the length of the file; for this reason, it is not recommended to change the length of \p str during operation.

This node is not intended for storage of arbitrary binary data.

\param[in] server estyx server
\param[in] parent Parent directory
\param[in] name File name
\param[in] str String to return
\return Pointer to created file, or 0 on error
*/
estyx_file_t *estyx_file_msg_make(estyx_server_t *server, 
	estyx_file_t *parent, const char *name, const char *str);

//! Create a boolean file
/*! Creates a one-character file which accepts and returns either a '1' or '0' character.  The boolean value is determined and reported using a get/set callback pair.  These files are useful for controlling boolean parameters.

When the file is read, it calls the get callback, passed at creation time as \p get.  This returns an integer.  If the integer is zero, the character '0' is read; if one, the character '1' is read.

When the file is written, it calls the set callback, passed at creation time as \p set.  This takes a single integer argument.  If the character is '1', 1 is passed.  If the character is '0', 0 is passed.

If the character is not '1' or '0', the length is zero, or the position is non-zero, the callback is not called.

The length of these files is always 1.

\param[in] server estyx server
\param[in] parent Parent directory
\param[in] name File name
\param[in] get Get function
\param[in] set Set function
\return Pointer to created file, or 0 on error
*/
estyx_file_t *estyx_file_bool_make(estyx_server_t *server, 
	estyx_file_t *parent, const char *name,
	estyx_file_bool_get_cb get, estyx_file_bool_set_cb set);

//! Create a get/set file
/*! Creates a file which calls a get/set pair with arbitrary data.

When the file is written, the set callback is called with the data buffer and the length of data written.  The callback can report the amount of data accepted by modifying the length parameter.

When the file is read, it calls the get callback, which returns a pointer to data and a length.

The file must be written and read at position zero, i.e., it is not seekable.  If the file is read at a position other than zero, the get callback is not called, and EOF is returned (zero-length result).  If the file is written at a position other than zero, the set callback is not called.

The length is always reported as zero.

\param[in] server estyx server
\param[in] parent Parent directory
\param[in] name File name
\param[in] get Get function
\param[in] set Set function
\return Pointer to created file, or 0 on error
*/
estyx_file_t *estyx_file_data_make(estyx_server_t *server, 
	estyx_file_t *parent, const char *name,
	estyx_file_data_get_cb get, estyx_file_data_set_cb set);

//! Create a file for single-precision floating-point numbers
/*! Creates a file which accepts, in text form, single-precision floating-point numbers.  Surrounding whitespace is ignored.

When the file is read, the number is printed using the printf()-type format string given in \p fmt.  This must accept a single float argument; if it does not, the results are undefined.

The length is always reported as zero.

\param[in] server estyx server
\param[in] parent Parent directory
\param[in] name File name
\param[in] get Get function
\param[in] set Set function
\param[in] fmt Format string
\return Pointer to created file, or 0 on error
*/
estyx_file_t *estyx_file_float_make(estyx_server_t *server, 
	estyx_file_t *parent, const char *name,
	estyx_file_float_get_cb get, estyx_file_float_set_cb set,
	const char *fmt);

//! Create a file for signed 32-bit numbers
/*! Creates a file which accepts, in text form, numbers ranging from -2^31 to 2^31-1 inclusive.  The numbers must be written in decimal, and are returned in decimal.  Surrounding whitespace is ignored.

When the file is written, the string is parsed as a signed decimal number.  When the file is read, the number is printed as a signed decimal number.

The length is always reported as zero.

\param[in] server estyx server
\param[in] parent Parent directory
\param[in] name File name
\param[in] get Get function
\param[in] set Set function
\return Pointer to created file, or 0 on error
*/
estyx_file_t *estyx_file_s32_make(estyx_server_t *server, 
	estyx_file_t *parent, const char *name,
	estyx_file_s32_get_cb get, estyx_file_s32_set_cb set);

//! Create a file for unsigned 32-bit numbers
/*! Creates a file which accepts, in text form, numbers as large as 2^32-1.  The numbers may be written in decimal, hexidecimal, or octal form, and are returned in decimal or hexidecimal form.  Surrounding whitespace is ignored.

When the file is written, the string is parsed as an unsigned number.  If the string begins in '0x', it is parsed as hexidecimal; otherwise, it is parsed as decimal.

When the file is read, the returned string is formatted in hexidecimal or decimal, depending on the value of \p showhex.  If \p showhex is 0, the number is formatted in decimal; if 1, the number is formatted in hexidecimal, beginning in '0x' and using capital letters; if greater than 1, the number is formatted in hexidecimal, using exactly that many digits.  For instance, if \p showhex is 4, the number is formatted as '0xnnnn', where 'n' are hexidecimal digits, padded with zeros on the left.  If the number is greater than 0xFFFF, the extra digits are shown.

The length is always reported as zero.

\param[in] server estyx server
\param[in] parent Parent directory
\param[in] name File name
\param[in] get Get function
\param[in] set Set function
\param[in] showhex Output format; see text
\return Pointer to created file, or 0 on error
*/
estyx_file_t *estyx_file_u32_make(estyx_server_t *server, 
	estyx_file_t *parent, const char *name,
	estyx_file_u32_get_cb get, estyx_file_u32_set_cb set,
	int showhex);

//! Create a file for enumerated lists
/*! Creates a file which accepts one of a list of indexed strings.  The get/set pair is passed indexes.

When the file is written, \p strlist is searched for the string until a null pointer is encountered.  If the string is found, the set function is called with the list index of the string.  If not, the function attempts to parse the string as an unsigned integer.  If this succeeds, the integer is taken to be an index number, and the set function is called with it.  If numeric parsing fails, the write is ignored.

When the file is read, the string returned depends on the value of \p print_index_and_str.  If this is 0, the index number is printed.  If 1, the index number is printed, followed by a space, followed by the string index in parentheses.

The string list may have up to 256 items, and indexes may be no greater than 255.  Up to 16 characters of each string are compared and printed.

The length is always reported as zero.

\param[in] server estyx server
\param[in] parent Parent directory
\param[in] name File name
\param[in] get Get function
\param[in] set Set function
\param[in] strlist String list; array of pointers to strings; last item must be null
\param[in] print_index_and_str Whether to print both index and string
\return Pointer to created file, or 0 on error
*/
estyx_file_t *estyx_file_enum_make(estyx_server_t *server, 
	estyx_file_t *parent, const char *name,
	estyx_file_enum_get_cb get, estyx_file_enum_set_cb set,
	const char **strlist, int print_index_and_str);

#endif
