/******************************************************************************\
*           Copyright (C) 1999 Texas Instruments Incorporated.
*                           All Rights Reserved
*------------------------------------------------------------------------------
* MODULE NAME... GPIO
* FILENAME...... gpio_readA.c
* DATE CREATED.. Tues 07/31/2001
* PROJECT....... CSL - Chip Support Library
* COMPONENT..... service layer
* PREREQUISITS.. 
*------------------------------------------------------------------------------
* HISTORY:
*   CREATED:  04/23/2002
*   MODIFIED: 12/09/2002 added conditional compilation for non-5502 
*------------------------------------------------------------------------------
* DESCRIPTION:  (interface file for the non-power non-muxed GPIO module)
\******************************************************************************/
#define _GPIO_MOD_

#include <csl_chiphal.h>

#if (_GPIO_SUPPORT)

#include <csl_gpio.h>

#if (CHIP_5502_FAMILY)

#pragma CODE_SECTION(GPIO_pinReadAll,".text:GPIO_pinReadAll")

/*----------------------------------------------------------------------------*/
int GPIO_pinReadAll(GPIO_Handle hGpio, Uint32 pinMask){
  GPIO_PrivateObj *ptr = (GPIO_PrivateObj *)hGpio;  
  Uint16 pins = (Uint16)(pinMask & 0xFFFFu);
  Uint16 val;
  
  val =  ((*((ioport Uint16 *)(ptr->datReg))) & (ptr->pinsAllocated & pins));
  return (val);
}
#endif /* CHIP_5502_FAMILY */
#endif /* GPIO_SUPPORT */
/******************************************************************************\
* End of gpio_read.c
\******************************************************************************/
