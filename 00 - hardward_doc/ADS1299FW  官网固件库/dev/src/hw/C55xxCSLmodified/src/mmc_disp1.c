/****************************************************************************\
*           Copyright (C) 1999-2000 Texas Instruments Incorporated.
*                           All Rights Reserved 
*------------------------------------------------------------------------------
* MODULE NAME... MMC
* FILENAME...... mmc_disp1.c
* DATE CREATED.. Thu May 24 14:48:09 2001
* PROJECT....... Chip Support Library
* COMPONENT..... Service Layer
* PREREQUISITS.. 
*------------------------------------------------------------------------------
*
* HISTORY:
*	 Created:		Thu May 24 14:48:09 2001 (Automatic generation)
*	 Last Modified:	        Thu Nov 6
*	 Changes made:          See mmc_disp0.c
*
*------------------------------------------------------------------------------
* DESCRIPTION: ISR disptach function for MMC1 callback functions.
\*************************************************************************/
#define _MMC_MOD_
/*************************************************************************\
* Included Header Files                                                  *
\*************************************************************************/

#include <csl_mmc.h>

#if (_MMC_SUPPORT)

#pragma CODE_SECTION(MMC_dispatch1,".text:MMC_dispatch1")

/**************************************************************************
* MMC_dispatch0()
*         Default ISR dispatch handler for MMC DEV1
***************************************************************************/

interrupt void MMC_dispatch1() {

  volatile Uint16 enabled;
  volatile Uint16 flagged;
  volatile Uint16 service;
  volatile int i;
  ioport MMC_MmcRegObj *regs;
  MMC_Handle mmc = MMC_hDev1;
  
  regs = mmc->mmcRegs;

  enabled = regs->mmcie;
  flagged = regs->mmcst0;
  service = enabled & flagged & 0x0EFF;
  i = 0;  
  
  if (!service)
    return;

  while (service){
    if (service & 0x1)
      if (mmc->callBackTbl->isr[i])
        mmc->callBackTbl->isr[i]();
    ++i;
    service >>= 1;
  }
}

#endif
