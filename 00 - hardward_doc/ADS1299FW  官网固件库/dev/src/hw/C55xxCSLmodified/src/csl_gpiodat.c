/******************************************************************************\
*           Copyright (C) 1999 Texas Instruments Incorporated.
*                           All Rights Reserved
*------------------------------------------------------------------------------
* MODULE NAME... GPIO
* FILENAME...... csl_gpiodat.c
* DATE CREATED.. Thu 10 April 2003
* PROJECT....... CSL - Chip Support Library
* COMPONENT..... service layer
* PREREQUISITS.. 
*------------------------------------------------------------------------------
* HISTORY:
*   CREATED:  07/31/2001
*------------------------------------------------------------------------------
* DESCRIPTION:  (Data file for the non-muxed GPIO module)
\******************************************************************************/
#define _GPIO_MOD_

#include <csl_gpio.h>
#include <csl_gpiodat.h>

#if (_GPIO_SUPPORT)

#pragma DATA_SECTION(CSL_GpioData,".csldata:GPIO_data")

CSL_GpioDataObj   CSL_GpioData = CSL_GPIODATAINIT;


/*----------------------------------------------------------------------------*/
/*------------------------------------------------------*/

#endif /* GPIO_SUPPORT */
/******************************************************************************\
* End of csl_gpiodat.c
\******************************************************************************/
