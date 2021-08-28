/****************************************************************************\
*           Copyright (C) 1999-2000 Texas Instruments Incorporated.
*                           All Rights Reserved 
*------------------------------------------------------------------------------
* MODULE NAME... MMC
* FILENAME...... mmc_disp0.c
* DATE CREATED.. Thu May 24 14:48:09 2001
* PROJECT....... Chip Support Library
* COMPONENT..... Service Layer
* PREREQUISITS.. 
*------------------------------------------------------------------------------
*
* HISTORY:
*	 Created:		Thu May 24 14:48:09 2001 (Automatic generation)
*	 Last Modified:         Thu Nov 6
*	 Changes made:          Changed value of check for number of interrupts 
*	                        from 0x800 to 0x400 as the spierr interrupt has 
*	                        been removed.
*------------------------------------------------------------------------------
* DESCRIPTION: ISR dispatch function to service MMC0 isrs.
\*************************************************************************/
#define _MMC_MOD_
/*************************************************************************\
* Included Header Files                                                  *
\*************************************************************************/

#include <csl_mmc.h>

#if (_MMC_SUPPORT)

#pragma CODE_SECTION(MMC_dispatch0,".text:MMC_dispatch0")

/**************************************************************************
* MMC_dispatch0()
*         Default ISR dispatch handler for MMC DEV0
***************************************************************************/

 interrupt void MMC_dispatch0() {
 
  volatile Uint16 enabled;
  volatile Uint16 flagged;
  volatile Uint16 service;
  volatile Uint16 i;
  ioport MMC_MmcRegObj *regs;
  MMC_Handle mmc = MMC_hDev0;

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
