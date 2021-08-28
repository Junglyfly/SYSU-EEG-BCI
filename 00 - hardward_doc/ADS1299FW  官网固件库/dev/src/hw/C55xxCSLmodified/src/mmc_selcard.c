/****************************************************************************\
*           Copyright (C) 1999-2000 Texas Instruments Incorporated.
*                           All Rights Reserved 
*------------------------------------------------------------------------------
* MODULE NAME... MMC
* FILENAME...... mmc_selcard.c
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
* DESCRIPTION: Selects the given card 
\*************************************************************************/
#define _MMC_MOD_
/*************************************************************************\
* Included Header Files                                                  *
\*************************************************************************/

#include <csl_mmc.h>
#include <csl_gpio.h>

#if (_MMC_SUPPORT)

#pragma CODE_SECTION(MMC_selectCard,".text:MMC_selectCard")

/**************************************************************************
* MMC_selectCard(MMC_Handle mmc, MMC_CardObj *card)
*          mmc     - Handle of MMC device reserved by previous call to 
*                    MMC_open()
*          card    - Card to be selected
***************************************************************************/

int MMC_selectCard(MMC_Handle mmc, MMC_CardObj *card) {
  Uint16 old_intm;
  Uint16 rca;

  ioport MMC_MmcRegObj *regs = mmc->mmcRegs;

  MMC_ASSERT_HANDLE(mmc,return);

  old_intm = IRQ_globalDisable();

  // Clear out response registers

  MMC_clearResponse(mmc);

  // Format and send command
  rca = (Uint16)(card->rca & 0xFFFFu);
  
  if (rca == 0) return 0;          // Zero is invalid, it is reserved

  regs->mmcargh = rca;  
  regs->mmcargl = MMC_STUFF_BITS;
  regs->mmccmd  = MMC_SELECT_CARD;

  IRQ_globalRestore(old_intm);

    // Wait for response done

  MMC_waitForFlag(mmc, MMC_ST0_RSPDNE | MMC_ST0_TOUTRS);
  if (mmc->status & MMC_ST0_TOUTRS)
     return 0;

  return 1;
}

#endif

