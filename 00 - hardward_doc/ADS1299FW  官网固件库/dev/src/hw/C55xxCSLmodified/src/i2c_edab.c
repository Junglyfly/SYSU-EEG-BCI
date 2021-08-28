/******************************************************************************\
*           Copyright (C) 1999 Texas Instruments Incorporated.
*                           All Rights Reserved
*------------------------------------------------------------------------------
* MODULE.NAME... I2C
* FILENAME...... i2c_edab.c
* DATE CREATED.. Sun 06/20/1999 
* PROJECT....... Chip Support Library
* COMPONENT..... 
* IMPORTS....... 
*------------------------------------------------------------------------------
* HISTORY:
*   CREATED:  08/02/2001
*------------------------------------------------------------------------------
* DESCRIPTION:  (Disable one/or more I2C interrupts)
*
*
*
\******************************************************************************/
#define _I2C_MOD_

#include <csl_chiphal.h>

#if (_I2C_SUPPORT)

#include <csl_i2c.h>

#pragma CODE_SECTION(I2C_eventDisable,".text:I2C_eventDisable")

void I2C_eventDisable(Uint16 ierMask)  {
  
   Uint16 temp = I2C_RGET(I2CIER) & (~ierMask);
   I2C_RSET(I2CIER,temp);
   
     if ( !(I2C_RGET(I2CIER) & 0x001F) )
           IRQ_disable(IRQ_EVT_I2C);

}

#endif

