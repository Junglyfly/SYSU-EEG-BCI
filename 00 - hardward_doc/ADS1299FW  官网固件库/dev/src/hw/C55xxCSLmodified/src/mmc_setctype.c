/****************************************************************************\
*           Copyright (C) 1999-2000 Texas Instruments Incorporated.
*                           All Rights Reserved 
*------------------------------------------------------------------------------
* MODULE NAME... MMC
* FILENAME...... mmc_setctype.c
* DATE CREATED.. Tue Dec 08 2003 
* PROJECT....... Chip Support Library
* COMPONENT..... Service Layer
* PREREQUISITS.. 
*------------------------------------------------------------------------------
*
* HISTORY:
*	 Created:		Tue 08 Dec 2003
*	 Last Modified:	
*
*------------------------------------------------------------------------------
* DESCRIPTION: Sets the card type to either MMC or SD based on return value
*              of send_op conditions
\*************************************************************************/
#define _MMC_MOD_
/*************************************************************************\
* Included Header Files                                                  *
\*************************************************************************/

#include <csl_mmc.h>


#if (_MMC_SUPPORT)

#pragma CODE_SECTION(MMC_setCardType,".text:MMC_setCardType")

/**************************************************************************
* MMC_setCardType(MMC_CardObj *cid, Uint16 type)
*          card     - pointer to card object
*          type     - MMC or SD card type
***************************************************************************/

void MMC_setCardType(MMC_CardObj *card, Uint16 type) {
    card->cardType = type;
}
#endif
  
