/******************************************************************************\
*           Copyright (C) 1999 Texas Instruments Incorporated.
*                           All Rights Reserved
*------------------------------------------------------------------------------
* MODULE NAME... GPIO
* FILENAME...... pgpio_pread.c
* DATE CREATED.. Tues 07/31/2001
* PROJECT....... CSL - Chip Support Library
* COMPONENT..... service layer
* PREREQUISITS.. 
*------------------------------------------------------------------------------
* HISTORY:
*   CREATED:  07/31/2001
*   MODIFIED: 01/22/2002 changed implenentation to read single pin
*                        rather than several
*------------------------------------------------------------------------------
* DESCRIPTION:  (interface file for the power non-muxed GPIO module)
\******************************************************************************/
#define _GPIO_MOD_

#include <csl_gpio.h>

#if (_GPIO_PGPIO_SUPPORT)

#pragma CODE_SECTION(_PGPIO_pinRead,".text:_PGPIO_pinRead")

/*----------------------------------------------------------------------------*/
int _PGPIO_pinRead(Uint32 pinId) {
  Uint16 dev = ((pinId >> 16) & 0xffffu);
  Uint16 pin = (pinId & 0xffffu);
  Uint16 pin1,pin2,val;

   switch (dev) {
    case GPIO_GPIO: 
                       pin1 = (pin & 0xffu);
                       pin2 = ((pin >> 8) & 0x3fu);
                       if (pin1) {
                          val = (_GPIO_IODATA & (pin1)) & (0xffu);
                       }
                       else {
                          val = ((_GPIO_GPIODATA & (pin2)) & 0x3fu);
                       }
                       return ((val) ? 1:0);
    case GPIO_AGPIO:   
                       return  ((_GPIO_AGPIODATA & pin) ? 1:0);

 
    default:           return 0;
  }
}
#endif /* GPIO_SUPPORT */
/******************************************************************************\
* End of pgpio_pread.c
\******************************************************************************/
