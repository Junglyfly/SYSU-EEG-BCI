/******************************************************************************\
*           Copyright (C) 1999 Texas Instruments Incorporated.
*                           All Rights Reserved
*------------------------------------------------------------------------------
* MODULE NAME... GPIO
* FILENAME...... gpio_write.c
* DATE CREATED.. Tues 07/31/2001
* PROJECT....... CSL - Chip Support Library
* COMPONENT..... service layer
* PREREQUISITS.. 
*------------------------------------------------------------------------------
* HISTORY:
*   CREATED:  07/31/2001
*   MODIFIED: 01/22/2002 Fixed problem with write only to GPIO0
*   MODIFIED: 12/09/2002 merged 5502/5509/5510 support
*------------------------------------------------------------------------------
* DESCRIPTION:  (interface file for the non power non-muxed GPIO module)
\******************************************************************************/
#define _GPIO_MOD_

#include <csl_chiphal.h>

#if (CHIP_5502_FAMILY)
  #if (_GPIO_SUPPORT)

  #include <csl_gpio.h>

  #pragma CODE_SECTION(GPIO_pinWrite,".text:GPIO_pinWrite")

  /*------------------------------------------------------*/
  void GPIO_pinWrite(GPIO_Handle hGpio, Uint32 pinMask, Uint16 val){
    GPIO_PrivateObj *ptr = (GPIO_PrivateObj *)hGpio;  
    Uint16 pins = (Uint16)(pinMask & 0xFFFFu);
    Uint16 allocMask = ptr->pinsAllocated;
     *((ioport Uint16 *)(ptr->datReg)) = ((*((ioport Uint16 *)(ptr->datReg))) 
                                         & (~(allocMask & pins))) | ((val) ? pins:0x0000u);
  }
  #endif
#else
   #if ((_GPIO_SUPPORT) && (!_GPIO_PGPIO_SUPPORT))

   #include <csl_gpio.h>
   
   #pragma CODE_SECTION(_GPIO_pinWrite,".text:_GPIO_pinWrite")

   /*------------------------------------------------------*/
   int _GPIO_pinWrite(Uint32 pinId, Uint16 val){
     Uint16 pin = (Uint16)(pinId & 0xFFFFu);
     _GPIO_IODATA = (_GPIO_IODATA & (~pin)) | ((val)? pin:0);
     return ((_GPIO_IODATA & pin) ? 1:0);
   }
   #endif /* GPIO_SUPPORT */
#endif /* CHIP_5502_FAMILY */
/******************************************************************************\
* End of gpio_write.c
\******************************************************************************/
