/******************************************************************************\
*           Copyright (C) 1999 Texas Instruments Incorporated.
*                           All Rights Reserved
*------------------------------------------------------------------------------
* MODULE NAME... GPIO
* FILENAME...... pgpio_pdisab.c
* DATE CREATED.. Tues 07/31/2001
* PROJECT....... CSL - Chip Support Library
* COMPONENT..... service layer
* PREREQUISITS.. 
*------------------------------------------------------------------------------
* HISTORY:
*   CREATED:  07/31/2001
*------------------------------------------------------------------------------
* DESCRIPTION:  (interface file for the power non-muxed GPIO module)
\******************************************************************************/
#define _GPIO_MOD_

#include <csl_gpio.h>

#if (_GPIO_PGPIO_SUPPORT)

#pragma CODE_SECTION(_PGPIO_pinDisable,".text:_PGPIO_pinDisable")

/*----------------------------------------------------------------------------*/
int _PGPIO_pinDisable(Uint32 pinId) {
  Uint16 dev = ((pinId >> 16) & 0xffffu);
  Uint16 pin = (pinId & 0xffffu);
  Uint16 pin2;

   switch (dev) {
    case GPIO_GPIO: 
                        pin2 = ((pin >> 8) & 0x3fu);
                        _GPIO_GPIOEN &= ~(pin2);
                        return ((_GPIO_GPIOEN & pin2) ? 1:0);

    case GPIO_AGPIO:   
                        _GPIO_AGPIOEN &= ~(pin);
                        return  ((_GPIO_AGPIOEN & pin) ? 1:0);

 
    default:            return 0;
 }
}

#endif /* GPIO_SUPPORT */
/******************************************************************************\
* End of pgpio_pdisab.c
\******************************************************************************/
