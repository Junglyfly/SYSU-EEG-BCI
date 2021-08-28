/******************************************************************************\
*           Copyright (C) 1999 Texas Instruments Incorporated.
*                           All Rights Reserved
*------------------------------------------------------------------------------
* MODULE.NAME... I2C
* FILENAME...... i2c_enab.c
* DATE CREATED.. Sun 06/20/1999 
* PROJECT....... Chip Support Library
* COMPONENT..... 
* IMPORTS....... 
*------------------------------------------------------------------------------
* HISTORY:
*   CREATED:  08/02/2001
*------------------------------------------------------------------------------
* DESCRIPTION:  (Enable/mask one/or more I2C interrupts)
*
*
*
\******************************************************************************/
#define _I2C_MOD_

#include <csl_i2c.h>

#if (_I2C_SUPPORT)

#pragma CODE_SECTION(I2C_eventEnable,".text:i2C_eventEnable")

void I2C_eventEnable(Uint16 isrmask)  {

   Uint16 temp;
     temp = I2C_RGET(I2CIER) | (isrmask);
     I2C_RSET(I2CIER,temp);
     IRQ_enable(IRQ_EVT_I2C);


}

#endif

