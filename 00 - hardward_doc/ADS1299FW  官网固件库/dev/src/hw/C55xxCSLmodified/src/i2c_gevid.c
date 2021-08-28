/******************************************************************************\
*           Copyright (C) 1999 Texas Instruments Incorporated.
*                           All Rights Reserved
*------------------------------------------------------------------------------
* MODULE.NAME... I2C
* FILENAME...... i2c_getevid.c
* DATE CREATED.. Sun 06/20/1999 
* PROJECT....... Chip Support Library
* COMPONENT..... 
* IMPORTS....... 
*------------------------------------------------------------------------------
* HISTORY:
*   CREATED:  08/02/2001
*------------------------------------------------------------------------------
* DESCRIPTION:  (Returns IRQ Event Id for I2C)
*
*
*
\******************************************************************************/
#define _I2C_MOD_

#include <csl_i2c.h>

#if (_I2C_SUPPORT)

#pragma CODE_SECTION(I2C_getEventId,".text:I2C_getEventId")

int I2C_getEventId(void) {

  return IRQ_EVT_I2C;
  
}
#endif
