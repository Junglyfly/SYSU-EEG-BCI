/******************************************************************************\
*           Copyright (C) 1999 Texas Instruments Incorporated.
*                           All Rights Reserved
*------------------------------------------------------------------------------
* MODULE.NAME... I2C
* FILENAME...... i2c_gcfg.c
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

#pragma CODE_SECTION(I2C_getConfig,".text:I2C_getConfig")

void I2C_getConfig(I2C_Config *Config) {

  int old_intm;

  old_intm = IRQ_globalDisable();
  Config->i2coar = I2C_RGET(I2COAR);
  Config->i2cier = I2C_RGET(I2CIER);
  Config->i2cpsc = I2C_RGET(I2CPSC);
  Config->i2cclkl = I2C_RGET(I2CCLKL);
  Config->i2cclkh = I2C_RGET(I2CCLKH);
  Config->i2ccnt = I2C_RGET(I2CCNT);
  Config->i2csar = I2C_RGET(I2CSAR);
  Config->i2cmdr = I2C_RGET(I2CMDR);
  Config->i2cstr = I2C_RGET(I2CSTR);
  Config->i2cisrc = I2C_RGET(I2CISRC);
  IRQ_globalRestore(old_intm);   



}
#endif

