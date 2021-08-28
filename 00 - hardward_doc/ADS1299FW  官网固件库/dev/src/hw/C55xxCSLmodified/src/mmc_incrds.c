/****************************************************************************\
*           Copyright (C) 1999-2000 Texas Instruments Incorporated.
*                           All Rights Reserved 
*------------------------------------------------------------------------------
* MODULE NAME... MMC
* FILENAME...... mmc_initcrd.c
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
* DESCRIPTION: Sends all broadcast commands in the Card initialization
*              sequence to get all CID/CSD information from all attached
*              cards.
\*************************************************************************/
#define _MMC_MOD_
/*************************************************************************\
* Included Header Files                                                  *
\*************************************************************************/

#include <csl_mmc.h>

#if (_MMC_SUPPORT)

#pragma CODE_SECTION(MMC_initCards,".text:MMC_initCards")

/**************************************************************************
* MMC_initCards(MMC_Handle mmc, MMC_CardObj *cards, 
*                Uint16 maxCards, Uint16 hVddMask, Uint116 startRca)
*          mmc         - Handle of MMC device reserved by previous call to 
*                        MMC_open()
*          cards       - Pointer to an array of card objcts to initialize
*          hVddMask    - Mask to get voltage range only or to force cards
*                        outside of voltage range to go offline.
*          startRca    - Starting value for first assigned relative card
*                        address.
***************************************************************************/

int MMC_initCards(MMC_Handle mmc, MMC_CardObj *cards, Uint16 hVddMask, Uint16 startRca) {

  Uint16 old_intm;
  Uint16 rca = startRca;

  MMC_ASSERT_HANDLE(mmc,return);

  old_intm = IRQ_globalDisable();

  //---------------------------------------------------------
  // Send Broadcast Command to Place All Cards in Idle State
  //---------------------------------------------------------
  MMC_sendGoIdle(mmc);

  //---------------------------------------------------------
  // Determine if there are any cards which cannot
  // communicate in acceptable voltage range for host
  //---------------------------------------------------------
  mmc->numCardsInactive = MMC_sendOpCond(mmc,hVddMask);
  
  //---------------------------------------------------------
  // Place all unacceptable cards in Inactive state
  //---------------------------------------------------------
  MMC_sendOpCond(mmc,hVddMask);

  //---------------------------------------------------------  
  // Get CID for all remaining Cards and Set Their
  // Relative Card Addresses
  //---------------------------------------------------------
  mmc->numCardsActive = 0;
  
  while(MMC_sendAllCID(mmc,&(cards[mmc->numCards].cid))) {
    MMC_setRca(mmc,&(cards[mmc->numCards-1]),rca);
    ++rca;
  }

  IRQ_globalRestore(old_intm);

  return mmc->numCards;
}
#endif
 
