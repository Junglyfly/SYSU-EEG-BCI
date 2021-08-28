/******************************************************************************\
*           Copyright (C) 1999 Texas Instruments Incorporated.
*                           All Rights Reserved
*------------------------------------------------------------------------------
* MODULE.NAME... I2C
* FILENAME...... i2c_dispatch.c
* DATE CREATED.. Sun 06/20/1999 
* PROJECT....... Chip Support Library
* COMPONENT..... 
* IMPORTS....... 
*------------------------------------------------------------------------------
* HISTORY:
*   CREATED:  08/02/2001
*   MODIFIED: 09/23/2004  Fixed the implementation error in I2C_intrDispatch
*------------------------------------------------------------------------------
* DESCRIPTION:  (Default Interrupt Service Routine for processing I2C ints)
*
*
*
\******************************************************************************/
#define _I2C_MOD_

#include <csl_chiphal.h>

#if (_I2C_SUPPORT)

#include <csl_i2c.h>

#pragma CODE_SECTION(I2C_intrDispatch,".text:I2C_intrDispatch")

interrupt void I2C_intrDispatch(void)   {

  if (I2C_FGET(I2CISRC,INTCODE) == I2C_I2CISRC_INTCODE_AL) {
  	((void (*)(void))(CSL_I2C_DATA).I2C_isrDispatchTable[I2C_EVT_AL])();
        I2C_FSET(I2CSTR,AL,1);  // AL bit is cleared
  } 
 
  if (I2C_FGET(I2CISRC,INTCODE) == I2C_I2CISRC_INTCODE_NACK){ 
  	((void (*)(void))(CSL_I2C_DATA).I2C_isrDispatchTable[I2C_EVT_NACK])();
        I2C_FSET(I2CSTR,NACK,1); // NACK bit is cleared
  }
  
  if (I2C_FGET(I2CISRC,INTCODE) == I2C_I2CISRC_INTCODE_ARDY) {
  	((void (*)(void))(CSL_I2C_DATA).I2C_isrDispatchTable[3])();
        I2C_FSET(I2CSTR,ARDY,1); // ARDY bit is cleared
  }
  if (I2C_FGET(I2CISRC,INTCODE) == I2C_I2CISRC_INTCODE_RRDY) {
  	((void (*)(void))(CSL_I2C_DATA).I2C_isrDispatchTable[4])();		
        I2C_FSET(I2CSTR,RRDY,1); // RRDY bit is cleared
  }
  	
  if (I2C_FGET(I2CISRC,INTCODE) == I2C_I2CISRC_INTCODE_XRDY) {
  	((void (*)(void))(CSL_I2C_DATA).I2C_isrDispatchTable[5])();
        I2C_FSET(I2CSTR,XRDY,1); // XRDY bit is cleared
  }
}
#endif

