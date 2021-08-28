/******************************************************************************\
*           Copyright (C) 1999 Texas Instruments Incorporated.
*                           All Rights Reserved
*------------------------------------------------------------------------------
* MODULE NAME... GPIO
* FILENAME...... pgpio_pwrite.c
* DATE CREATED.. Tues 07/31/2001
* PROJECT....... CSL - Chip Support Library
* COMPONENT..... service layer
* PREREQUISITS.. 
*------------------------------------------------------------------------------
* HISTORY:
*   CREATED:  07/31/2001
*   MODIFIED: 01/22/2002 changed implementation to write a single pin
*                        rather than mask several
*------------------------------------------------------------------------------
* DESCRIPTION:  (interface file for the non power non-muxed GPIO module)
\******************************************************************************/
#define _GPIO_MOD_

#include <csl_gpio.h>

#if (_GPIO_PGPIO_SUPPORT)

#pragma CODE_SECTION(_PGPIO_pinWrite,".text:_PGPIO_pinWrite")

/*----------------------------------------------------------------------------*/
int _PGPIO_pinWrite(Uint32 pinId, Uint16 val) {
  Uint16 dev = ((pinId >> 16) & 0xffffu);
  Uint16 pin = (pinId & 0xffffu);
  Uint16 pin1,pin2;

   switch (dev) {
    case GPIO_GPIO: 
                        pin1 = (Uint16)(pin & 0xffu);
                        pin2 = (Uint16)((pin >> 8) & 0x3fu);
                        if (pin1) {
                            _GPIO_IODATA = (_GPIO_IODATA & (~pin1)) 
                                           | ((val) ? pin1:0);
                           return ((_GPIO_IODATA & pin1)? 1:0);
                        }
                        _GPIO_GPIODATA = (_GPIO_GPIODATA & (~(pin2)))
                                         | ((val)? pin2:0);
                        return ((_GPIO_GPIODATA & pin2)? 1:0);
    case GPIO_AGPIO:   
                        _GPIO_AGPIODATA = (_GPIO_AGPIODATA & ~pin) | 
                                         ((val) ? pin:0);
                        return  ((_GPIO_AGPIODATA & pin) ? 1:0);

    default:            return 0;
 }
}
#endif /* GPIO_SUPPORT */
/******************************************************************************\
* End of pgpio_pwrite.c
\******************************************************************************/
