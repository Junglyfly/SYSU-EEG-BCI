/****************************************************************************\
*           Copyright (C) 1999-2000 Texas Instruments Incorporated.
*                           All Rights Reserved 
*------------------------------------------------------------------------------
* MODULE NAME... MMC
* FILENAME...... mmc_setrca.c
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
* DESCRIPTION: Sets the Relative Card Address for current card. Assumes
*              ALL_SEND_CID command has already been issued and response
*              is complete.
\*************************************************************************/
#define _MMC_MOD_
/*************************************************************************\
* Included Header Files                                                  *
\*************************************************************************/

#include <csl_mmc.h>

#if (_MMC_SUPPORT)

#pragma CODE_SECTION(MMC_setRca,".text:MMC_setRca")

/**************************************************************************
* MMC_setRca(MMC_Handle mmc, Uint16 rca)
*          mmc     - Handle of MMC device reserved by previous call to 
*                    MMC_open()
*          rca     - User assigned relative card address
***************************************************************************/

int MMC_setRca(MMC_Handle mmc, MMC_CardObj *card, Uint16 rca) {
  Uint16 old_intm, stat;
  ioport MMC_MmcRegObj *regs = mmc->mmcRegs;

  MMC_ASSERT_HANDLE(mmc, return);
  old_intm = IRQ_globalDisable();

  // Clear out response registers

  MMC_clearResponse(mmc);

  // Format and send command

  if(rca == 0) return 0;          // Zero is invalid, it is reserved

  regs->mmcargh = rca;
  regs->mmcargl = MMC_STUFF_BITS;
  regs->mmccmd = MMC_SET_RELATIVE_ADDR;
  
  // Wait for response done
  MMC_getStatus(mmc,MMC_ST0_RSPDNE);
  while (!MMC_getStatus(mmc,MMC_USE_LAST_STATUS | MMC_ST0_RSPDNE)) {
    if (MMC_getStatus(mmc,MMC_USE_LAST_STATUS | MMC_ST0_TOUTRS ))
       return 0;
  }

  IRQ_globalRestore(old_intm);

  card->rca = rca; 
  card->cardIndex = mmc->nextIndex;
  ++(mmc->nextIndex);

  //associating card with handle here. 
  mmc->cards = card;
  //intialize data in the card
  mmc->cards->lastAddrRead = 0;
  mmc->cards->lastAddrWritten = 0;
  //fields like card capacity and taac are not initialized but can be filled by the user

  return 1;
}
#endif

  
