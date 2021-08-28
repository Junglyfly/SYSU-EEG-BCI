
#ifndef GUARD_eeprom_h
#define GUARD_eeprom_h

/****************************************************************/
/* eeprom utility file                                          */
/* Copyright (C) 2008 Texas Instruments Incorporated            */
/*                                                              */
/* Permission is hereby granted, free of charge, to any person  */
/* obtaining a copy of this software and associated             */
/* documentation files (the "Software"), to deal in the Software*/
/* without restriction, including without limitation the rights */
/* to use, copy, modify, merge, publish, distribute, sublicense,*/
/* and/or sell copies of the Software, and to permit persons to */
/* whom the Software is furnished to do so, subject to the      */
/* following conditions:                                        */
/*                                                              */
/* The above copyright notice and this permission notice shall  */
/* be included in all copies or substantial portions of the     */
/* Software.                                                    */
/*                                                              */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY    */
/* KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE   */
/* WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR      */
/* PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE AUTHORS  */
/* OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR     */
/* OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR   */
/* OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE    */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.       */
/*--------------------------------------------------------------*/
/** \file
 * eeprom.h / eeprom.c: Functions to read the EEPROM contents and
 * fields on the daughtercards
 */

#include "estyx.h"
#include "mmb0i2c.h"

#define EEPROM_LENGTH (1024)

#define EEPROM_MMB0 (I2C_BUS_EXT)
#define EEPROM_EVM  (I2C_BUS_DC)

/************MMB0 EEPROM ACCESS*****************************/
//! MMB0 build date retrieval function
/**
 * - returns the date code when the MMB0 was manufactured as
 * programmed into the EEPROM
 * \param *p array to be filled by the function
 * \retval status
 *          - 0 success
 *          - -1 no EEPROM found
*/
int eeprom_get_mmb0_date(char *p);

//! MMB0 name retrieval function
/**
 * - returns the name of the EVM as programmed into the EEPROM
 *
 * \param *p character array to be filled by this function
 * \retval status
 *          - 0 success
 *          - -1 no EEPROM found
*/
int eeprom_get_mmb0_name(char *p);
//! MMB0 revision retrieval function
/**
 * - returns the MMB0 revision in a single string as
 * programmed into the EEPROM
 *
 * \param *p array to be filled by this function
 * \retval status
 *          - 0 success
 *          - -1 no EEPROM found
*/
int eeprom_get_mmb0_rev(char *p);

//! MMB0 build test state retrieval function
/**
 * - returns the build test state in a single string as
 * programmed into the EEPROM
 *
 * \param *p array to be filled by this function
 * \retval status
 *          - 0 success
 *          - -1 no EEPROM found
*/
int eeprom_get_mmb0_state(char *p);

/************EVM EEPROM ACCESS*****************************/
//! EVM BOM retrieval function
/**
 * - returns the EVM bill of material number and revision in a
 *  single string as programmed into the EEPROM
 *
 * \param *p array to be filled by this function
 * \retval status
 *          - 0 success
 *          - -1 no EEPROM found
*/
int eeprom_get_evm_bom(char *p);

//! EVM build date retrieval function
/**
 * - returns the date code when the EVM was manufactured as
 * programmed into the EEPROM
 * \param *p array to be filled by the function
 * \retval status
 *          - 0 success
 *          - -1 no EEPROM found
*/
int eeprom_get_evm_date(char *p);

//! EVM name retrieval function
/**
 * - returns the name of the EVM as programmed into the EEPROM
 *
 * \param *p character array to be filled by this function
 * \retval status
 *          - 0 success
 *          - -1 no EEPROM found
*/
int eeprom_get_evm_name(char *p);

//! EVM PCA retrieval function
/**
 * - returns the PCA number and revision in a single string as
 * programmed into the EEPROM
 *
 * \param *p array to be filled by this function
 * \retval status
 *          - 0 success
 *          - -1 no EEPROM found
*/
int eeprom_get_evm_pca(char *p);

//! EVM PWB retrieval function
/**
 * - copies the pwb number and revision into  a single string as
 * programmed into the EEPROM
 *
 * \param *p array to be filled by this function
 * \retval status
 *          - 0 success
 *          - -1 no EEPROM found
*/
int eeprom_get_evm_pwb(char *p);

/************EEPROM BULK ACCESS*****************************/
//! EEPROM page length getter fucntion
/**
 * - returns the size of the current EEPROM page as defined in
 * EEPROM_LENGTH;
 *
 * \param *bus the mmb0 I2C bus the eeprom is connected to 
 * \param *f the pointer to the EEPROM data file (unused)
 * \retval the size of the data file
*/
u32 eeprom_len(u8 bus, estyx_file_t *f);

//! EEPROM read function for data file
/**
 * - reads the number of bytes defines in EEPROM_LENGTH from the
 * EEPROM page passed in parameter \p page and returns them in the
 * passed array. The lenght of this array must match EEPROM_LENGTH.
 *
 * \param *bus the mmb0 I2C bus the eeprom is connected to 
 * \param *p Array to be filled by this function
 * \param page The EEPROM page to read from
 * \retval status
 *          - 0 success
 *          - -1 no EEPROM found
*/
int eeprom_get_page(u8 bus, char *p, unsigned int page);

//! EEPROM data file read function
/*! The EEPROM read function. Reads the contents of the EEPROM
    into the data file for the EEPROM
    \param *bus the mmb0 I2C bus the eeprom is connected to 
    \param f File ID of the \p data file
    \param len Number of bytes to be read
    \param pos Position inside the \p data file. Unused parameter
    \param data Pointer to buffer data is to be read into
    \retval 0 Success
    \retval !0 Fail                                             */
int eeprom_read(u8 bus, estyx_fid_t* f, u16 len, u32 pos, char* data);


#endif
