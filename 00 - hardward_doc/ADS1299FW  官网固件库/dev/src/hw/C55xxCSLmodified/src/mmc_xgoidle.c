/****************************************************************************\
*           Copyright (C) 1999-2000 Texas Instruments Incorporated.
*                           All Rights Reserved 
*------------------------------------------------------------------------------
* MODULE NAME... MMC
* FILENAME...... mmc_xgoidle.c
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
* DESCRIPTION: Sends broadcast GO_IDLE command
\*************************************************************************/
#define _MMC_MOD_
/*************************************************************************\
* Included Header Files                                                  *
\*************************************************************************/

#include <csl_mmc.h>

#if (_MMC_SUPPORT)

#pragma CODE_SECTION(MMC_sendGoIdle,".text:MMC_sendGoIdle")

/**************************************************************************
* MMC_sendGoIdle(MMC_Handle mmc)
*          mmc         - Handle of MMC device reserved by previous call to 
*                        MMC_open()
***************************************************************************/

void MMC_sendGoIdle(MMC_Handle mmc) {

  Uint16 old_intm;
  ioport MMC_MmcRegObj *regs = mmc->mmcRegs;

  MMC_ASSERT_HANDLE(mmc, return);

  old_intm = IRQ_globalDisable();

  // Clear out response registers

  MMC_clearResponse(mmc);

  // Format and send command
  regs->mmcargh = MMC_STUFF_BITS;
  regs->mmcargl = MMC_STUFF_BITS;
  regs->mmccmd = MMC_GO_IDLE_STATE;
  
  IRQ_globalRestore(old_intm);
}

#endif

  
