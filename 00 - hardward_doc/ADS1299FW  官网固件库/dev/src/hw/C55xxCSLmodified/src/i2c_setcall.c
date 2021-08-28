/******************************************************************************\
*           Copyright (C) 1999 Texas Instruments Incorporated.
*                           All Rights Reserved
*------------------------------------------------------------------------------
* MODULE.NAME... I2C
* FILENAME...... i2c_setcall.c
* DATE CREATED.. Sun 06/20/1999 
* PROJECT....... Chip Support Library
* COMPONENT..... 
* IMPORTS....... 
*------------------------------------------------------------------------------
* HISTORY:
*   CREATED:  08/02/2001
*------------------------------------------------------------------------------
* DESCRIPTION:  (Sets call back function for I2C interrupts)
*
*
*
\******************************************************************************/
#define _I2C_MOD_

#include <csl_chiphal.h>

#if (_I2C_SUPPORT)

#include <csl_i2c.h>

#pragma CODE_SECTION(I2C_setCallback,".text:I2C_setCallBack")

void I2C_setCallback(I2C_IsrAddr *isrAddr) {

    (CSL_I2C_DATA).I2C_isrDispatchTable[I2C_EVT_AL] = (Uint32) (isrAddr->alAddr); 
    (CSL_I2C_DATA).I2C_isrDispatchTable[I2C_EVT_NACK] = (Uint32) (isrAddr->nackAddr); 
    (CSL_I2C_DATA).I2C_isrDispatchTable[3] = (Uint32) (isrAddr->ardyAddr); 
    (CSL_I2C_DATA).I2C_isrDispatchTable[4] = (Uint32) (isrAddr->rrdyAddr); 
    (CSL_I2C_DATA).I2C_isrDispatchTable[5] = (Uint32) (isrAddr->xrdyAddr); 
    
       
    IRQ_plug(IRQ_EVT_I2C, I2C_intrDispatch);         
}

#endif

