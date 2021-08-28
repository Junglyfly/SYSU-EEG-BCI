/****************************************************************************\
*           Copyright (C) 1999-2000 Texas Instruments Incorporated.
*                           All Rights Reserved 
*------------------------------------------------------------------------------
* MODULE NAME... MMC
* FILENAME...... mmc_stop.c
* DATE CREATED.. Thu May 24 14:48:09 2001
* PROJECT....... Chip Support Library
* COMPONENT..... Service Layer
* PREREQUISITS.. 
*------------------------------------------------------------------------------
*
* HISTORY:
*	 Created:		Thu May 24 14:48:09 2001 (Automatic generation)
*	 Last Modified:	
*
*------------------------------------------------------------------------------
* DESCRIPTION: Halts current data transfer.
\*************************************************************************/
#define _MMC_MOD_
/*************************************************************************\
* Included Header Files                                                  *
\*************************************************************************/

#include <csl_mmc.h>

#if (_MMC_SUPPORT)

#pragma CODE_SECTION(MMC_stop,".text:MMC_stop")

/**************************************************************************
* MMC_stop(MMC_Handle mmc)
*          mmc     - Handle of MMC device reserved by previous call to 
*                    MMC_open()
***************************************************************************/

 int MMC_stop(MMC_Handle mmc) {
  Uint16 old_intm;
  ioport MMC_MmcRegObj *regs = mmc->mmcRegs;

  MMC_ASSERT_HANDLE(mmc, return);
  old_intm = IRQ_globalDisable();

  // Clear out response registers

  MMC_clearResponse(mmc);

  // Format and send command

  regs->mmcargh = MMC_STUFF_BITS;
  regs->mmcargl = MMC_STUFF_BITS;
  regs->mmccmd = MMC_STOP_TRANSMISSION;

  // Wait for response done

  MMC_getStatus(mmc,MMC_ST0_RSPDNE);
  while (!MMC_getStatus(mmc,MMC_USE_LAST_STATUS | MMC_ST0_RSPDNE)) {
    if (MMC_getStatus(mmc,MMC_USE_LAST_STATUS | MMC_ST0_TOUTRS ))
       return 0;
  }

  IRQ_globalRestore(old_intm);

  return 1;
}
#endif

  
