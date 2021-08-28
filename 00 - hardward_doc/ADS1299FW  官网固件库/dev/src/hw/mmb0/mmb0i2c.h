
#ifndef GUARD_i2c_h
#define GUARD_i2c_h

#include "types.h"

//:wrap=soft:

// addresses for on-board peripherals
#define I2C_ADR_MUX	0xE2
#define I2C_ADR_GPIO	0x40
#define I2C_ADR_LED	0x42
#define I2C_ADR_CTL	0x44
#define I2C_ADR_CLK	(0x69<<1)

// bus numbers for the I2C mux
#define I2C_BUS_DC 0
#define I2C_BUS_MB 1
#define I2C_BUS_EXT 2

/*! These functions provide access to the I2C system on the MMB0.  Functions for both multi-byte (i2c_wr(), i2c_rd()) and single-byte (i2c_w8(), i2c_r8()) transactions are provided.  There is also a read-modify-write function (i2c_frob()).

Before calling any of these functions, call i2c_init().

The MMB0's I2C bus is multiplexed into three separate busses.  Each data access function takes a parameter \p bus which identifies the bus on which to operate the transaction.  The following busses are available:

- I2C_BUS_DC -- the I2C pins on the daughtercard connector
- I2C_BUS_MB -- bus serving the MMB0's on-board I2C peripherals
- I2C_BUS_EXT -- the I2C expansion connector

For all I/O functions, I2C addresses must be in 8-bit format; the lower bit is ignored.  Note that some I2C documentation gives I2C addresses in 7-bit format.  Such addresses must be left-shifted once before use with these functions.

Each I/O function, with the exception of i2c_r8(), returns an error code indicating whether acknowledge bits were received as expected.  If 0, the transaction was acknowledged.
*/

//! Write one byte to I2C
/*! Writes one byte to the given address and bus.

\param[in] bus Bus number
\param[in] adr I2C address, 8-bit format
\param[in] b Byte to be written
*/
int i2c_w8(u8 bus, u8 adr, u8 b);

//! Write to I2C
/*! Writes the \p len bytes at \p b to bus \p bus and address \p adr.

\param[in] bus Bus number
\param[in] adr I2C address, 8-bit format
\param[in] b Bytes to be written
\param[in] len Number of bytes to write
\retval 0 Write succeeded
\retval negative Write was not acknowledged
*/
int i2c_wr(u8 bus, u8 adr, u8 *b, int len);

//! Read one byte from I2C
/*! Reads one byte from the given address and bus.

If the read is not acknowledged, a negative number is returned.  A successful read results in a byte, which is 0-255.

\param[in] bus Bus number
\param[in] adr I2C address, 8-bit format
\retval 0-255 Read byte
\retval negative Read was not acknowledged
*/
short i2c_r8(u8 bus, u8 adr);

//! Read from I2C
/*! Writes the \p len bytes at \p b to bus \p bus and address \p adr.

\param[in] bus Bus number
\param[in] adr I2C address, 8-bit format
\param[in] b Bytes to be read
\param[in] len Number of bytes to read
\retval 0 Succeeded
\retval negative Read was not acknowledged
*/
int i2c_rd(u8 bus, u8 adr, u8 *b, int len);

//! Read-modify-write on I2C
/*! Performs a "frob" operation on an I2C address.  This is done as follows:

1. A byte is read from \p bus and \p adr.
2. The byte is modified with bits from \p data, but only those bits with a corresponding 1 in \p mask.  The C expression is (byte&~mask)|(data&mask).
3. The result is written to \p bus and \p adr.

This function is particularly useful for controlling the PCF8574 GPIO peripheral.
*/
int i2c_frob(u8 bus, u8 adr, u8 data, u8 mask);

//! Initialise I2C system
/*! Initialises the I2C subsystem.

\warning This function must be called before any of the other I2C functions. */
void i2c_init(void);

#endif
