/******************************************************************************\
*           Copyright (C) 1999 Texas Instruments Incorporated.
*                           All Rights Reserved
*------------------------------------------------------------------------------
* MODULE NAME... GPIO
* FILENAME...... gpio_cfg.c
* DATE CREATED.. Tues 04/23/2002
* PROJECT....... CSL - Chip Support Library
* COMPONENT..... service layer
* PREREQUISITS.. 
*------------------------------------------------------------------------------
* HISTORY:
*   CREATED:  04/23/2002
*------------------------------------------------------------------------------
* DESCRIPTION:  (interface file for the non-power non-muxed GPIO module)
\******************************************************************************/
#define _GPIO_MOD_

#include <csl_chiphal.h>

#if (CHIP_5502_FAMILY)
   #if (_GPIO_SUPPORT)

   #include <csl_gpio.h>

   #pragma CODE_SECTION(GPIO_config,".text:GPIO_config")

   /*----------------------------------------------------------------------------*/
   void GPIO_config(GPIO_Handle hGpio, GPIO_Config *cfg) {
      GPIO_PrivateObj *ptr = (GPIO_PrivateObj *)hGpio;

      if (ptr->regIndx == _GPIO_NON_PARALLEL) {
         *((ioport Uint16 *)(ptr->dirReg)) = cfg->iodir;
         return;
      }
   #if (_GPIO_PARALLEL_SUPPORT)
      *((ioport Uint16 *)(ptr->enableReg)) = cfg->ioen & (ptr->pinsAllocated);
      *((ioport Uint16 *)(ptr->dirReg)) = cfg->iodir;
   #endif
   }
   #endif /* GPIO_SUPPORT */
#endif
/******************************************************************************\
* End of gpio_cfg.c
\******************************************************************************/
