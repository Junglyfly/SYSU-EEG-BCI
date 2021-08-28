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

#include "mmb0i2c.h"
#include "mmb0eeprom.h"
#include "estyx.h"
#include "utilfiles.h"
#include <string.h>
#include <std.h>
#include <swi.h>

/* \def defines the lenght of a 40 char arryay                  */
#define MAXREAD  (41)
/* \def defines the lenght of a 32 char arryay                  */
#define STR32MAX (32)
/* \def defines the lenght of a 16 char array                   */
#define STR16MAX (16)
/* \def read address for the EEPROM on the EVM                  */
#define EEPROM_READ (0xA1)
/* \def write address for the EEPROM on the EVM                 */
#define EEPROM_WRITE (0xA0)
/* \def length of the EEPROM to read                            */

/**********MMB0 DATA OFFSETS*************************************/
#define MMB0_EEPROM_NAME_LENGTH   STR32MAX 
#define MMB0_EEPROM_NAME_OFFSET   (0x0000)
#define MMB0_EEPROM_DATE_LENGTH   STR16MAX 
#define MMB0_EEPROM_DATE_OFFSET   (0x0020)
#define MMB0_EEPROM_REV_LENGTH    STR16MAX
#define MMB0_EEPROM_REV_OFFSET    (0x0030)
#define MMB0_EEPROM_STATE_LENGTH  STR32MAX
#define MMB0_EEPROM_STATE_OFFSET  (0x0040)

/**********EVM  DATA OFFSETS*************************************/
#define EVM_EEPROM_NAME_LENGTH    STR32MAX 
#define EVM_EEPROM_NAME_OFFSET    (0x0000)
#define EVM_EEPROM_DATE_LENGTH    STR16MAX
#define EVM_EEPROM_DATE_OFFSET    (0x0050)
#define EVM_EEPROM_PWB_LENGTH     STR16MAX
#define EVM_EEPROM_PWB_OFFSET     (0x0020)
#define EVM_EEPROM_PCA_LENGTH     STR16MAX
#define EVM_EEPROM_PCA_OFFSET     (0x0030)
#define EVM_EEPROM_BOM_LENGTH     STR16MAX
#define EVM_EEPROM_BOM_OFFSET     (0x0040)

/****************************************************************/
/* eeprom_get_data()                                             */
/** EVM EEPROM read utility. Used to read various segments of
 * the EEPROM into the given buffer
 * - Writes "Bad eeprom wr" to the buffer on failed writes
 * - Writes "Bad eeprom rd" to the buffer on failed read
 * - Writes "NULL" to the buffer if a valid string is not found
 * 
 * \param bus MMB0 bus the EEPROM is attached to
 * \param address[] data address/offset into the EEPROM
 * \param length data length to be read from the eeprom
 * \param *pBuffer data buffer where EEPROM data is be be placed
 * \retval status
 *          - 0 success
 *          - -1 no EEPROM found
*/
/****************************************************************/
int eeprom_get_data(u8 bus, u8 address[], int length, char *pBuffer)
{
	int status;
    char rdBuff[MAXREAD];

    status = i2c_wr(bus, EEPROM_WRITE, address, 2);
    if (status != 0)
    {
       strcpy(pBuffer, "Bad eeprom wr\0");
       return -1;
    }
    
    status = i2c_rd(bus, EEPROM_READ, (u8 *)rdBuff, length);
    if (status != 0)
    {
       strcpy(pBuffer, "Bad eeprom rd\0");
       return -1;
    }
    
    if((strlen((const char *)rdBuff) > length-1) || (strlen((const char *)rdBuff) == 0))
    {
    	strcpy(pBuffer, "NULL\0");
    }
    else
    {
    	strcpy(pBuffer, rdBuff);
    }
    
    return 0;
}

/****************************************************************/
/**********MMB0 ACCESS*******************************************/
/****************************************************************/
int eeprom_get_mmb0_name(char *p)
{
    u8 address[2];
    address[0] = (u8)((int)MMB0_EEPROM_NAME_OFFSET >> 8);
    address[1] = (u8)MMB0_EEPROM_NAME_OFFSET;
    return eeprom_get_data(EEPROM_MMB0, address, 
            MMB0_EEPROM_NAME_LENGTH, p);
}

/****************************************************************/
int eeprom_get_mmb0_date(char *p)
{
    u8 address[2];

    address[0] = (u8)((int)MMB0_EEPROM_DATE_OFFSET >> 8);
    address[1] = (u8)MMB0_EEPROM_DATE_OFFSET;
    return eeprom_get_data(EEPROM_MMB0, address, 
            MMB0_EEPROM_DATE_LENGTH, p);
}

/****************************************************************/
int eeprom_get_mmb0_rev(char *p)
{
    u8 address[2];
    address[0] = (u8)((int)MMB0_EEPROM_REV_OFFSET >> 8);
    address[1] = (u8)MMB0_EEPROM_REV_OFFSET;
    return eeprom_get_data(EEPROM_MMB0, address, 
            MMB0_EEPROM_REV_LENGTH, p);
}

/****************************************************************/
int eeprom_get_mmb0_state(char *p)
{
    u8 address[2];
    address[0] = (u8)((int)MMB0_EEPROM_STATE_OFFSET >> 8);
    address[1] = (u8)MMB0_EEPROM_STATE_OFFSET;
    return eeprom_get_data(EEPROM_MMB0, address, 
            MMB0_EEPROM_STATE_LENGTH, p);
}

/****************************************************************/
/**********EVM ACCESS********************************************/
/****************************************************************/
int eeprom_get_evm_name(char *p)
{
    u8 address[2];
    address[0] = (u8)((int)EVM_EEPROM_NAME_OFFSET >> 8);
    address[1] = (u8)EVM_EEPROM_NAME_OFFSET;
    return eeprom_get_data(EEPROM_EVM, address, 
            EVM_EEPROM_NAME_LENGTH, p);
}

/****************************************************************/
int eeprom_get_evm_date(char *p)
{
    u8 address[2];
    address[0] = (u8)((int)EVM_EEPROM_DATE_OFFSET >> 8);
    address[1] = (u8)EVM_EEPROM_DATE_OFFSET;
    return eeprom_get_data(EEPROM_EVM, address, 
            EVM_EEPROM_DATE_LENGTH, p);
}

/****************************************************************/
int eeprom_get_evm_pwb(char *p)
{
    u8 address[2];
    address[0] = (u8)((int)EVM_EEPROM_PWB_OFFSET >> 8);
    address[1] = (u8)EVM_EEPROM_PWB_OFFSET;
    return eeprom_get_data(EEPROM_EVM, address, 
            EVM_EEPROM_PWB_LENGTH, p);
}

/****************************************************************/
int eeprom_get_evm_pca(char *p)
{
    u8 address[2];
    address[0] = (u8)((int)EVM_EEPROM_PCA_OFFSET >> 8);
    address[1] = (u8)EVM_EEPROM_PCA_OFFSET & 0xFF;
    return eeprom_get_data(EEPROM_EVM, address, EVM_EEPROM_PCA_LENGTH, p);
}

/****************************************************************/
int eeprom_get_evm_bom(char *p)
{
    u8 address[2];
    address[0] = (u8)((int)EVM_EEPROM_BOM_OFFSET >> 8);
    address[1] = (u8)EVM_EEPROM_BOM_OFFSET;
    return eeprom_get_data(EEPROM_EVM, address, 
            EVM_EEPROM_BOM_LENGTH, p);
}

/****************************************************************/
/* eeprom_get_page()                                            */
/****************************************************************/
int eeprom_get_page(u8 bus, char *pBuffer, unsigned int page)
{
    int status;
    u8 data[2];

    pBuffer[EEPROM_LENGTH]=0;
    data[0] = ((0x0400u * page) >> 8) & 0xFFu;  /* high address */
    data[1] = (0x0400u * page) & 0xFFu;         /* low address  */
    status = i2c_wr(bus, EEPROM_WRITE, data, 2);
    if (status != 0)
    {
       strcpy(pBuffer, "Bad eeprom wr\0");
       return -1;
    }
    
    status = i2c_rd(bus, EEPROM_READ, (u8 *)pBuffer, EEPROM_LENGTH);
    if (status != 0)
    {
       strcpy(pBuffer, "Bad eeprom wr\0");
       return -1;
    }
	return 0;
}

/****************************************************************/
/* eeprom_len()                                                 */
/****************************************************************/
u32 eeprom_len(u8 bus, estyx_file_t *f)
{
    return EEPROM_LENGTH;
}

/****************************************************************/
/* eeprom_read()                                                */
/****************************************************************/
int eeprom_read(u8 bus, estyx_fid_t* f, u16 len, u32 pos, char* data)
{
    int status;
    int length=len;
    data[EEPROM_LENGTH]=0;
    SWI_disable();
    if (!len)
        length = 0;
    if (len > EEPROM_LENGTH)
        length = EEPROM_LENGTH >> 1;
    status = estyx_rread55(f, length, (u8*)(data));

    SWI_enable();
    return status;
}

