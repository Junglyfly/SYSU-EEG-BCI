/******************************************************************************\
*           Copyright (C) 1999 Texas Instruments Incorporated.
*                           All Rights Reserved
*------------------------------------------------------------------------------
* MODULE NAME... GPIO
* FILENAME...... gpio_cfgall.c
* DATE CREATED.. Tues 04/23/2002
* PROJECT....... CSL - Chip Support Library
* COMPONENT..... service layer
* PREREQUISITS.. 
*------------------------------------------------------------------------------
* HISTORY:
*   CREATED: 04/23/2002
*   	   : 12/13/2002 merged 5502/5509/5510 support
*   	   : 12/02/2003 Fixed the typo in '#if' statement
*   	   : 08/06/2004 Added 5501 support
*------------------------------------------------------------------------------
* DESCRIPTION:  (interface file for the non-power non-muxed GPIO module)
\******************************************************************************/
#define _GPIO_MOD_

#include <csl_chiphal.h>

#if (CHIP_5502_FAMILY)
   #if (_GPIO_SUPPORT)

   #include <csl_gpio.h>

   #pragma CODE_SECTION(GPIO_configAll,".text:GPIO_configAll")

   /*----------------------------------------------------------------------------*/
   void GPIO_configAll(GPIO_ConfigAll *gCfg) {

        GPIO_RSET(IODIR,gCfg->iodir);
   #if (_GPIO_PARALLEL_SUPPORT)
         GPIO_RSET(PGPIOEN0,gCfg->pgpioen0);
         GPIO_RSET(PGPIODIR0,gCfg->pgpiodir0);
         GPIO_RSET(PGPIOEN1,gCfg->pgpioen1);
         GPIO_RSET(PGPIODIR1,gCfg->pgpiodir1);
   #if (CHIP_5502) || (CHIP_5501)
         GPIO_RSET(PGPIOEN2,gCfg->pgpioen2);
         GPIO_RSET(PGPIODIR2,gCfg->pgpiodir2);
   #endif
   #endif
   }
   #endif /* GPIO_SUPPORT */
#endif
/******************************************************************************\
* End of gpio_cfgall.c
\******************************************************************************/
