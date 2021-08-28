/******************************************************************************\
*           Copyright (C) 1999 Texas Instruments Incorporated.
*                           All Rights Reserved
*------------------------------------------------------------------------------
* MODULE NAME... GPIO
* FILENAME...... gpio_pdir.c
* DATE CREATED.. Tues 07/31/2001
* PROJECT....... CSL - Chip Support Library
* COMPONENT..... service layer
* PREREQUISITS.. 
*------------------------------------------------------------------------------
* HISTORY:
*   CREATED:  07/31/2001
*   MODIFIED: 01/22/2002 Fixed problem with setting only to GPIO0
*   MODIFIED: 12/13/2002 merged 5502/5510/5509 support
*   MODIFIED: 08/06/2004 Added 5501 support
*------------------------------------------------------------------------------
* DESCRIPTION:  (interface file for the power non-muxed GPIO module)
\******************************************************************************/
#define _GPIO_MOD_

#include <csl_gpio.h>

#if (CHIP_5502) || (CHIP_5501)

   #if (_GPIO_SUPPORT)

   #pragma CODE_SECTION(GPIO_pinDirection,".text:GPIO_pinDirection")

   /*------------------------------------------------------*/
   void GPIO_pinDirection(GPIO_Handle hGpio, Uint32 pinMask, Uint16 direction){
      GPIO_PrivateObj *ptr = (GPIO_PrivateObj *)hGpio;
      Uint16 pins = (Uint16)(pinMask & 0xFFFFu);
      Uint16 allocMask = ptr->pinsAllocated;
      *((ioport Uint16 *)(ptr->dirReg)) = ((*((ioport Uint16 *)(ptr->dirReg))) 
                                          & (~(allocMask & pins))) | ((direction) ? pins:0x0000u);
   }
   #endif /* GPIO_SUPPORT */
#else 
   #if ((_GPIO_SUPPORT) && (!_GPIO_PGPIO_SUPPORT))

   #pragma CODE_SECTION(_GPIO_pinDirection,".text:_GPIO_pinDirection")

   /*------------------------------------------------------*/
   int _GPIO_pinDirection(Uint32 pinId, Uint16 direction){
      Uint16 pin = (Uint16)(pinId & 0xFFFFu);
      _GPIO_IODIR = (_GPIO_IODIR & ~(pin)) | ((direction) ? pin : 0);
      return ((_GPIO_IODIR & pin) ? 1:0);
   }
   #endif /* GPIO_SUPPORT */
#endif     /* CHIP_5502  || CHIP_5501*/
/******************************************************************************\
* End of gpio_pdir.c
\******************************************************************************/
