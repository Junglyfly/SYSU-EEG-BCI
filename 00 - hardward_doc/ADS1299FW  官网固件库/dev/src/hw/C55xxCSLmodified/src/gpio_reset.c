/******************************************************************************\
*           Copyright (C) 1999 Texas Instruments Incorporated.
*                           All Rights Reserved
*------------------------------------------------------------------------------
* MODULE NAME... GPIO
* FILENAME...... gpio_reset.c
* DATE CREATED.. Tues 04/23/2002
* PROJECT....... CSL - Chip Support Library
* COMPONENT..... service layer
* PREREQUISITS.. 
*------------------------------------------------------------------------------
* HISTORY:
*   CREATED:  04/23/2002
*   MODIFIED: 12/09/2002 added conditional compile for non-5502 devices
*------------------------------------------------------------------------------
* DESCRIPTION:  (interface file for the non-power non-muxed GPIO module)
\******************************************************************************/
#define _GPIO_MOD_

#include <csl_chiphal.h>

#if (_GPIO_SUPPORT)

#if (CHIP_5502_FAMILY)

#include <csl_gpio.h>

#pragma CODE_SECTION(GPIO_pinReset,".text:GPIO_pinReset")

/*----------------------------------------------------------------------------*/
void GPIO_pinReset(GPIO_Handle hGpio, Uint32 pinMask) {
  GPIO_PrivateObj *ptr = (GPIO_PrivateObj *)hGpio;
  Uint16 allocMask = ptr->pinsAllocated;

#if (_GPIO_PARALLEL_SUPPORT) || (_GPIO_AGPIO_SUPPORT)
  *((ioport Uint16 *)(ptr->enableReg)) = *(ioport Uint16 *)(ptr->enableReg)
                                       & (~(allocMask & pinMask));
#endif
  *((ioport Uint16 *)(ptr->dirReg)) = (*((ioport Uint16 *)(ptr->dirReg)) &
                                    (~(allocMask & pinMask)));
}
#endif /* CHIP_5502_FAMILY */

#endif /* GPIO_SUPPORT */
/******************************************************************************\
* End of gpio_reset.c
\******************************************************************************/
