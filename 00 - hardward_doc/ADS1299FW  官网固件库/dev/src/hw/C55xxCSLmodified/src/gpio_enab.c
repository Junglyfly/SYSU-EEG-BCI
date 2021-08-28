/******************************************************************************\
*           Copyright (C) 1999 Texas Instruments Incorporated.
*                           All Rights Reserved
*------------------------------------------------------------------------------
* MODULE NAME... GPIO
* FILENAME...... gpio_enab.c
* DATE CREATED.. Tues 07/31/2001
* PROJECT....... CSL - Chip Support Library
* COMPONENT..... service layer
* PREREQUISITS.. 
*------------------------------------------------------------------------------
* HISTORY:
*   CREATED:  07/31/2001
*   MODIFIED: 01/22/2002 changed to return single value
*   MODIFIED: 12/09/2002 added conditional compilation for non-5502
*------------------------------------------------------------------------------
* DESCRIPTION:  (interface file for the power non-muxed GPIO module)
*               Enables pin function during idle/power-down
\******************************************************************************/
#define _GPIO_MOD_

#include <csl_gpio.h>

#if ((_GPIO_SUPPORT) && (!_GPIO_PGPIO_SUPPORT))

#if (CHIP_5502_FAMILY)

  #pragma CODE_SECTION(GPIO_pinEnable,".text:GPIO_pinEnable")

  /*----------------------------------------------------------------------------*/
  void GPIO_pinEnable(GPIO_Handle hGpio, Uint32 pinId){
    GPIO_PrivateObj *ptr = (GPIO_PrivateObj *)hGpio;
    Uint16 allocMask;
    Uint16 pin = (Uint16)(pinId & 0xFFFFu);

     allocMask = ptr->pinsAllocated;
     if (ptr->regIndx == _GPIO_NON_PARALLEL) {
       return;
     }
     *((ioport Uint16 *)(ptr->enableReg)) = *((ioport Uint16 *)(ptr->enableReg)) 
                                            | (allocMask & pin);

     }
  #endif /* GPIO_SUPPORT */

 #else
   #if ((_GPIO_SUPPORT) && (!_GPIO_PGPIO_SUPPORT))

   #pragma CODE_SECTION(_GPIO_pinEnable,".text:_GPIO_pinEnable")

   /*------------------------------------------------------*/
   int _GPIO_pinEnable(Uint32 pinId){
     return 1;
   }
   #endif /* GPIO_SUPPORT */
 #endif
/******************************************************************************\
* End of gpio_enab.c
\******************************************************************************/
