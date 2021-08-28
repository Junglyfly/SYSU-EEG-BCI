/******************************************************************************\
*           Copyright (C) 1999 Texas Instruments Incorporated.
*                           All Rights Reserved
*------------------------------------------------------------------------------
* MODULE.NAME... I2C
* FILENAME...... i2c_cfga.c
* DATE CREATED.. Sun 06/20/1999 
* PROJECT....... Chip Support Library
* COMPONENT..... 
* IMPORTS....... 
*------------------------------------------------------------------------------
* HISTORY:
*   CREATED:  08/02/2001
*------------------------------------------------------------------------------
* DESCRIPTION:  (I2C initialization)
*
*
*
\******************************************************************************/
#define _I2C_MOD_

#include <csl_chiphal.h>

#if (_I2C_SUPPORT)

#include <csl_i2c.h>

#pragma CODE_SECTION(I2C_configArgs,".text:I2C_configArgs")

IDEF void I2C_configArgs(Uint16 i2coar,Uint16 i2cier,Uint16 i2cstr,Uint16 i2cclkl,Uint16 i2cclkh,Uint16 i2ccnt,Uint16 i2csar,Uint16 i2cmdr,Uint16 i2cisrc,Uint16 i2cpsc) {

  int old_intm;

  old_intm = IRQ_globalDisable();  
  I2C_RSET(I2COAR,i2coar);
  I2C_RSET(I2CIER,i2cier);
  I2C_RSET(I2CPSC,i2cpsc);	
  I2C_RSET(I2CCLKL,i2cclkl);
  I2C_RSET(I2CCLKH,i2cclkh);
  I2C_RSET(I2CCNT,i2ccnt);
  I2C_RSET(I2CSAR,i2csar);				 
  I2C_RSET(I2CMDR,i2cmdr);
  I2C_RSET(I2CSTR,i2cstr);
  I2C_RSET(I2CISRC,i2cisrc);
  IRQ_globalRestore(old_intm);   

}
#endif


