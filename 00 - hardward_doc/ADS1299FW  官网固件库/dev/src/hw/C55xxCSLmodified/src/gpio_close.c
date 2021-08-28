/******************************************************************************\
*           Copyright (C) 1999 Texas Instruments Incorporated.
*                           All Rights Reserved
*------------------------------------------------------------------------------
* MODULE NAME... GPIO
* FILENAME...... gpio_close.c
* DATE CREATED.. Tues 04/23/2002
* PROJECT....... CSL - Chip Support Library
* COMPONENT..... service layer
* PREREQUISITS.. 
*------------------------------------------------------------------------------
* HISTORY:
*   CREATED : 04/23/2002
*   MODIFIED: 12/13/2002 merge 5502/5509/5510 support
*   MODIFIED: 10/07/2003 Fixed GPIO_close() bug.
*
*------------------------------------------------------------------------------
* DESCRIPTION:  (interface file for the non-power non-muxed GPIO module)
\******************************************************************************/
#define _GPIO_MOD_

#include <csl_chiphal.h>

#if (CHIP_5502_FAMILY)

   #if (_GPIO_SUPPORT)

   #include <csl_gpio.h>

   #pragma CODE_SECTION(GPIO_close,".text:GPIO_close")

   /*----------------------------------------------------------------------------*/
   IDEF void GPIO_close(GPIO_Handle hGpio) {
      GPIO_PrivateObj *ptr = (GPIO_PrivateObj *)hGpio;
      Uint16 regIndx = (ptr->regIndx);

      (CSL_SYS_DATA).GpioAllocMask[regIndx] = 0;
      ptr->pinsAllocated = 0x0000;
   }
  
   #endif /* GPIO_SUPPORT */
#endif
/******************************************************************************\
* End of gpio_open.c
\******************************************************************************/
