/****************************************************************************\
*           Copyright (C) 1999-2000 Texas Instruments Incorporated.
*                           All Rights Reserved 
*------------------------------------------------------------------------------
* MODULE NAME... MMC/SD
* FILENAME...... sd_allcid.c
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
* DESCRIPTION: Sends the broadcast SEND_ALL_CID command to identify
*              attached MMC/SD cards. Function returns the CID information
*              for a responding card in the specified card ID object
\*************************************************************************/
#define _MMC_MOD_
/*************************************************************************\
* Included Header Files                                                  *
\*************************************************************************/

#include <csl_mmc.h>


#if (_MMC_SUPPORT)

#pragma CODE_SECTION(SD_sendAllCID,".text:SD_sendALLCID")

/**************************************************************************
* SD_sendAllCID(MMC_Handle sd, MMC_CardIdObj *cid)
*          sd     - Handle of SD device reserved by previous call to 
*                    MMC_open()
*          cid     - pointer to card ID object
***************************************************************************/

int SD_sendAllCID(MMC_Handle sd, MMC_CardIdObj *card) {
  
  Uint16 old_intm;
  Uint16 stat;
  ioport MMC_MmcRegObj *regs = sd->mmcRegs;
  
  MMC_ASSERT_HANDLE(sd,return);
  old_intm = IRQ_globalDisable();

  // Clear out response registers

  MMC_clearResponse(sd);
  MMC_getStatus(sd,MMC_ST0_RSPDNE);

  // Format and send command
  
  regs->mmcargh = MMC_STUFF_BITS;
  regs->mmcargl = MMC_STUFF_BITS;
  regs->mmccmd  = MMC_ALL_SEND_CID;

  // Wait for response done
  while ((stat=MMC_responseDone(sd))==0)
                   ;

  if (stat==MMC_RESPONSE_TIMEOUT)
    return 0;

  sd->numCards++;

  SD_getCardID(sd, card);

  IRQ_globalRestore(old_intm);

  return 1;
}
#endif
  
