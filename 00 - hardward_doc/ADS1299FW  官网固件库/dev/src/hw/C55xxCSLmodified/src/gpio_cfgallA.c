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
*          : 12/13/2002 merged 5502/5509/5510 support
*          : 12/02/2003 Fixed typo in '#if' statement
*          : 08/06/2004 Added 5501 support
*------------------------------------------------------------------------------
* DESCRIPTION:  (interface file for the non-power non-muxed GPIO module)
\******************************************************************************/
#define _GPIO_MOD_

#include <csl_chiphal.h>

#if (CHIP_5502_FAMILY)

   #if (_GPIO_SUPPORT)

   #include <csl_gpio.h>

   #pragma CODE_SECTION(GPIO_configAllArgs,".text:GPIO_configAllArgs")

   /*----------------------------------------------------------------------------*/
   #if (CHIP_5502) || (CHIP_5501)
          void GPIO_configAllArgs(GPIO_Handle hGpio, Uint16 iodir, Uint16 pgpioen0, Uint16 pgpiodir0,
          Uint16 pgpioen1, Uint16 pgpiodir1, Uint16 pgpioen2, Uint16 pgpiodir2) {
   #elif (CHIP_5509)
          void GPIO_configAllArgs(GPIO_Handle hGpio, Uint16 iodir, Uint16 pgpioen0, Uint16 pgpiodir0,
          Uint16 pgpioen1, Uint16 pgpiodir1) {
   #else
          void GPIO_configAllArgs(GPIO_Handle hGpio, Uint16 iodir) {
   #endif

     GPIO_RSET(IODIR,iodir);

      #if (_GPIO_PARALLEL_SUPPORT)
         GPIO_RSET(PGPIOEN0,pgpioen0);
         GPIO_RSET(PGPIODIR0,pgpiodir0);
         GPIO_RSET(PGPIOEN1,pgpioen1);
         GPIO_RSET(PGPIODIR1,pgpiodir1);
    #if (CHIP_5502) || (CHIP_5501)
         GPIO_RSET(PGPIOEN2,pgpioen2);
         GPIO_RSET(PGPIODIR2,pgpiodir2);
   #endif
   #endif
   }
   #endif /* GPIO_SUPPORT */
#endif
/******************************************************************************\
* End of gpio_cfgall.c
\******************************************************************************/
