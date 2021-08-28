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
* DESCRIPTION: Returns the total number of card found in the system
*              when in Native mode. Also returns in pointer arguments
*              the number of those cards that are currently active
*              and the number that ar inactive (off-line)
\*************************************************************************/
#define _MMC_MOD_
/*************************************************************************\
* Included Header Files                                                  *
\*************************************************************************/

#include <csl_mmc.h>

#if (_MMC_SUPPORT)

#pragma CODE_SECTION(MMC_getNumberOfCards,".text:MMC_getNumberOfCards")

/**************************************************************************
* MMC_getNumberOfCards(MMC_Handle mmc, Uint16 *active, Uint16 *inactive)
*          mmc      - Handle of MMC device reserved by previous call to 
*                     MMC_open()
*          active   - Pointer where to return number of active cards
*          inactive - Pointer to where to return number of inactive cards
* Returns - total number of cards in system (active + inactrive)
***************************************************************************/

int MMC_getNumberOfCards(MMC_Handle mmc, Uint16 *active, Uint16 *inactive) {

  MMC_ASSERT_HANDLE(mmc,return);

  *active = mmc->numCardsActive;
  *inactive = mmc->numCardsInactive;
  
  return mmc->numCards;
}
#endif
  
