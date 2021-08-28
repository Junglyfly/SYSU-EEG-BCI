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
*   MODIFIED: 01/22/2002 changed implementation to access only 1 pin
*                        rather than mask several
*------------------------------------------------------------------------------
* DESCRIPTION:  (interface file for the power non-muxed GPIO module)
\******************************************************************************/
#define _GPIO_MOD_

#include <csl_gpio.h>

#if (_GPIO_PGPIO_SUPPORT)

#pragma CODE_SECTION(_PGPIO_pinDirection,".text:_PGPIO_pinDirection")

/*----------------------------------------------------------------------------*/
int _PGPIO_pinDirection(Uint32 pinId, Uint16 direction) {
  Uint16 dev = ((pinId >> 16) & 0xffffu);
  Uint16 pin = (Uint16)(pinId & 0xffffu);
  Uint16 pin1, pin2;
   switch (dev) {
    case GPIO_GPIO: 
                         pin1 = pin & 0xffu;
                         pin2 = (pin >> 8) & 0x3fu;
                        if (pin1) {
                           _GPIO_IODIR = (_GPIO_IODIR & (~(pin1)))
                                         | ((direction) ? pin1:0);
                           return ((_GPIO_IODIR & pin1) ? 1:0);
                        }
                        _GPIO_GPIODIR = (_GPIO_GPIODIR & (~(pin2)))
                                        | ((direction)?  pin2:0);
                        return ((_GPIO_GPIODIR & pin2)? 1:0);

    case GPIO_AGPIO:   
                        _GPIO_AGPIODIR = (_GPIO_AGPIODIR & (~(pin))) 
                                          | ((direction)? pin:0);
                        return ((_GPIO_AGPIODIR & pin) ? 1:0);

    default:            return 0;
  }
}
#endif /* GPIO_SUPPORT */
/******************************************************************************\
* End of pgpio_pdir.c
\******************************************************************************/
