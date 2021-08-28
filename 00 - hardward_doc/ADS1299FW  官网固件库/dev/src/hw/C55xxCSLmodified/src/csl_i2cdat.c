/******************************************************************************\
*           Copyright (C) 1999 Texas Instruments Incorporated.
*                           All Rights Reserved
*------------------------------------------------------------------------------
* MODULE.NAME... I2C
* FILENAME...... csl_i2cdat.c
* DATE CREATED.. Thu 10 April 2003 
* PROJECT....... Chip Support Library
* COMPONENT..... 
* IMPORTS....... 
*------------------------------------------------------------------------------
* HISTORY:
*   CREATED:  10 Apri 2003 added reference for CSL_I2cData for .csldata size fix
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

#pragma DATA_SECTION(CSL_I2cData,".csldata:I2C_data")

CSL_I2cDataObj  CSL_I2cData = CSL_I2CDATAINIT;

   




#endif

