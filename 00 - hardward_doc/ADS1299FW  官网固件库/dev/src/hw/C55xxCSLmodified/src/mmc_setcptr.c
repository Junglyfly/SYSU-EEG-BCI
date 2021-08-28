/****************************************************************************\
*           Copyright (C) 1999-2000 Texas Instruments Incorporated.
*                           All Rights Reserved 
*------------------------------------------------------------------------------
* MODULE NAME... MMC
* FILENAME...... mmc_setcptr.c
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
* DESCRIPTION: Sets the card pointer in MMC global status table
*              This function must be used if the application performs
*              system/card initialization outside of MMC_initCard function.
\*************************************************************************/
#define _MMC_MOD_
/*************************************************************************\
* Included Header Files                                                  *
\*************************************************************************/

#include <csl_mmc.h>

#if (_MMC_SUPPORT)

#pragma CODE_SECTION(MMC_setCardPtr,".text:MMC_setCardPtr")

/**************************************************************************
* MMC_setCardPtr(MMC_Handle mmc, MMC_CardObj *cards)
*          mmc      - Handle of MMC device reserved by previous call to 
*                     MMC_open()
*          cards   - Pointer to set of card objects that define all cards
*                    connected to the MMC controller
***************************************************************************/

void MMC_setCardPtr(MMC_Handle mmc, MMC_CardObj *cards){

  MMC_ASSERT_HANDLE(mmc, return);

  mmc->cards = cards;
}
#endif
  
