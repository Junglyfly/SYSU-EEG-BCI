/******************************************************************************\
*           Copyright (C) 1999 Texas Instruments Incorporated.
*                           All Rights Reserved
*------------------------------------------------------------------------------
* MODULE NAME... GPIO
* FILENAME...... gpio_disab.c
* DATE CREATED.. Tues 07/31/2001
* PROJECT....... CSL - Chip Support Library
* COMPONENT..... service layer
* PREREQUISITS.. 
*------------------------------------------------------------------------------
* HISTORY:
*   CREATED:  07/31/2001
*   MODIFIED: 01/22/2002 changed to return single value
*   MODIFIED: 12/13/2002 merged 5502/5510/5509 support
*   MODIFIED: 10/07/2003 Fixed GPIO-pinDisable() bug.
*
*------------------------------------------------------------------------------
* DESCRIPTION:  (interface file for the power non-muxed GPIO module)
\******************************************************************************/
#define _GPIO_MOD_

#include <csl_gpio.h>

#if (CHIP_5502_FAMILY)
   #if (_GPIO_SUPPORT)

   #pragma CODE_SECTION(GPIO_pinDisable,".text:GPIO_pinDisable")

   /*------------------------------------------------------*/
   IDEF void GPIO_pinDisable(GPIO_Handle hGpio, Uint32 pinId){
     GPIO_PrivateObj *ptr = (GPIO_PrivateObj *)hGpio;
     Uint16 allocMask;
     Uint16 pin = (Uint16)(pinId & 0xFFFFu);

      allocMask = ptr->pinsAllocated;
      if (ptr->regIndx == _GPIO_NON_PARALLEL) {
        return;
      }
      *((ioport Uint16 *)(ptr->enableReg)) = (*((ioport Uint16 *)(ptr->enableReg))) 
                                              & (~(allocMask & pin));

   }
  #endif /* GPIO_SUPPORT */
#else
   #if ((_GPIO_SUPPORT) && (!_GPIO_PGPIO_SUPPORT))

   #pragma CODE_SECTION(_GPIO_pinDisable,".text:_GPIO_pinDisable")

   /*------------------------------------------------------*/
   int _GPIO_pinDisable(Uint32 pinId){
     return 1;
   }
   #endif /* GPIO_SUPPORT */
#endif   /* CHIP_5502 */

/******************************************************************************\
* End of gpio_disab.c
\******************************************************************************/
