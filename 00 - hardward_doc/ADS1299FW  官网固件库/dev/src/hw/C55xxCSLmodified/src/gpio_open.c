/******************************************************************************\
*           Copyright (C) 1999 Texas Instruments Incorporated.
*                           All Rights Reserved
*------------------------------------------------------------------------------
* MODULE NAME... GPIO
* FILENAME...... gpio_open.c
* DATE CREATED.. Tues 04/23/2002
* PROJECT....... CSL - Chip Support Library
* COMPONENT..... service layer
* PREREQUISITS.. 
*------------------------------------------------------------------------------
* HISTORY:
*   CREATED :   04/23/2002
*   MODIFIED:   10/06/2003 Fixed GPIO_open() bug.

*------------------------------------------------------------------------------
* DESCRIPTION:  (interface file for the non-power non-muxed GPIO module)
\******************************************************************************/
#define _GPIO_MOD_

#include <csl_chiphal.h>

#if (_GPIO_SUPPORT)

#include <csl_gpio.h>

#if (CHIP_5502_FAMILY)

  #pragma CODE_SECTION(GPIO_open,".text:GPIO_open")

  /*----------------------------------------------------------------------------*/
  GPIO_Handle GPIO_open(Uint32 allocMask, Uint32 flags){
    Uint16 regIndx = (Uint16)((allocMask >> 16) & 0xFFFFu);
    Uint16 pinMask = (Uint16)(allocMask & 0xFFFFu); 
    Uint16 old_intm;
    GPIO_PrivateObj *ptr = INV;

    old_intm = IRQ_globalDisable();

     if (!((CSL_SYS_DATA).GpioAllocMask[regIndx] )) {
      (CSL_SYS_DATA).GpioAllocMask[regIndx] = pinMask; /*allocated;*/
      ptr = (GPIO_PrivateObj *)(&((CSL_GPIO_DATA).pinObj[regIndx]));
      ptr->regIndx = regIndx;
      ptr->pinsAllocated = pinMask;
    }

    IRQ_globalRestore(old_intm);

    return (GPIO_Handle)(ptr);
  }
  #endif /* CHIP_5502_FAMILY */
#endif /* GPIO_SUPPORT */
/******************************************************************************\
* End of gpio_open.c
\******************************************************************************/
