/******************************************************************************\
*           Copyright (C) 1999 Texas Instruments Incorporated.
*                           All Rights Reserved
*------------------------------------------------------------------------------
* MODULE NAME... GPIO
* FILENAME...... gpio_cfga.c
* DATE CREATED.. Tues 04/23/2002
* PROJECT....... CSL - Chip Support Library
* COMPONENT..... service layer
* PREREQUISITS.. 
*------------------------------------------------------------------------------
* HISTORY:
*   CREATED:  04/23/2002
*   MODIFIED: 12/13/2002 merged 5502/5509/5510 support
*------------------------------------------------------------------------------
* DESCRIPTION:  (interface file for the non-power non-muxed GPIO module)
\******************************************************************************/
#define _GPIO_MOD_

#include <csl_chiphal.h>

#if (CHIP_5502_FAMILY)
   #if (_GPIO_SUPPORT)

   #include <csl_gpio.h>

   #pragma CODE_SECTION(GPIO_configArgs,".text:GPIO_configArgs")

   /*----------------------------------------------------------------------------*/
   #if (_GPIO_PARALLEL_SUPPORT)
    void GPIO_configArgs(GPIO_Handle hGpio, Uint32 enabMask, Uint16 direction) {
   #else
    void GPIO_configArgs(GPIO_Handle hGpio, Uint16 direction) {
   #endif
       GPIO_PrivateObj *ptr = (GPIO_PrivateObj *)hGpio;
 

       if (ptr->regIndx == _GPIO_NON_PARALLEL) {
         *(ioport Uint16 *)(ptr->dirReg) = direction;
         return;
       }

   #if (_GPIO_PARALLEL_SUPPORT)
       *(ioport Uint16 *)(ptr->enableReg) = enabMask & (ptr->pinsAllocated);
   #endif    
 
    }
   #endif /* GPIO_SUPPORT */
#endif
/******************************************************************************\
* End of gpio_cfg.c
\******************************************************************************/
