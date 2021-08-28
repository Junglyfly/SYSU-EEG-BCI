/******************************************************************************\
*           Copyright (C) 1999 Texas Instruments Incorporated.
*                           All Rights Reserved
*------------------------------------------------------------------------------
* MODULE NAME... GPIO
* FILENAME...... gpio_writeA.c
* DATE CREATED.. Tues 07/31/2001
* PROJECT....... CSL - Chip Support Library
* COMPONENT..... service layer
* PREREQUISITS.. 
*------------------------------------------------------------------------------
* HISTORY:
*   CREATED:  04/23/2002
*   MODIFIED: 12/09/2002 made conditional compile for non-5502 devices
*------------------------------------------------------------------------------
* DESCRIPTION:  (interface file for the non power non-muxed GPIO module)
\******************************************************************************/
#define _GPIO_MOD_

#include <csl_gpio.h>

#if (CHIP_5502_FAMILY)

  #if (_GPIO_SUPPORT)

  #pragma CODE_SECTION(GPIO_pinWriteAll,".text:GPIO_pinWriteAll")

  /*------------------------------------------------------*/
  void GPIO_pinWriteAll(GPIO_Handle hGpio, Uint32 pinMask, Uint16 val){
    GPIO_PrivateObj *ptr = (GPIO_PrivateObj *)hGpio;  
    Uint16 pins = (Uint16)(pinMask & 0xFFFFu);
    Uint16 allocMask = ptr->pinsAllocated;
     *((ioport Uint16 *)(ptr->datReg)) = ((*((ioport Uint16 *)(ptr->datReg))) 
                                         & (~(allocMask & pins))) | val;
  }
  #endif /* GPIO_SUPPORT */
#endif /* CHIP_5502_FAMILY */
/******************************************************************************\
* End of gpio_write.c
\******************************************************************************/
