/******************************************************************************\
*           Copyright (C) 1999 Texas Instruments Incorporated.
*                           All Rights Reserved
*------------------------------------------------------------------------------
* MODULE NAME... GPIO
* FILENAME...... gpio_read.c
* DATE CREATED.. Tues 07/31/2001
* PROJECT....... CSL - Chip Support Library
* COMPONENT..... service layer
* PREREQUISITS.. 
*------------------------------------------------------------------------------
* HISTORY:
*   CREATED:  07/31/2001
*   MODIFIED: 01/22/2002 changed to return 1 pin value instead of
*                        several
*   MODIFIED: 12/13/2002 merged 5502/5509/5510 support
*------------------------------------------------------------------------------
* DESCRIPTION:  (interface file for the non-power non-muxed GPIO module)
\******************************************************************************/
#define _GPIO_MOD_

#include <csl_chiphal.h>

#include <csl_gpio.h>

#if (CHIP_5502_FAMILY)
  #if (_GPIO_SUPPORT)

  #pragma CODE_SECTION(GPIO_pinRead,".text:GPIO_pinRead")

  /*----------------------------------------------------------------------------*/
  int GPIO_pinRead(GPIO_Handle hGpio, Uint32 pinId){
    GPIO_PrivateObj *ptr = (GPIO_PrivateObj *)hGpio;  
    Uint16 pins = (Uint16)(pinId & 0xFFFFu);
    Uint16 val;
  
    val =  ((*((ioport Uint16 *)(ptr->datReg))) & (ptr->pinsAllocated & pins));
    return (val ? 1:0);
  }
#else
   #if ((_GPIO_SUPPORT) && (!_GPIO_PGPIO_SUPPORT))

   #pragma CODE_SECTION(_GPIO_pinRead,".text:_GPIO_pinRead")

   /*----------------------------------------------------------------------------*/
    int _GPIO_pinRead(Uint32 pinId){
     Uint16 pin = (Uint16)(pinId & 0xFFFFu);
     return ((_GPIO_IODATA & pin) ? 1:0);
   }
   #endif /* GPIO_SUPPORT */
#endif

#endif /* GPIO_SUPPORT */
/******************************************************************************\
* End of gpio_read.c
\******************************************************************************/
