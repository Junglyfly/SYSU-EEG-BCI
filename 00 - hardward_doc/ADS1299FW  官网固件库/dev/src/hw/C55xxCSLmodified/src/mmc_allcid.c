/****************************************************************************\
*           Copyright (C) 1999-2000 Texas Instruments Incorporated.
*                           All Rights Reserved 
*------------------------------------------------------------------------------
* MODULE NAME... MMC
* FILENAME...... mmc_allcid.c
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
*              attached MMC cards. Function returns the CID information
*              for a responding card in the specified card ID object
\*************************************************************************/
#define _MMC_MOD_
/*************************************************************************\
* Included Header Files                                                  *
\*************************************************************************/

#include <csl_mmc.h>


#if (_MMC_SUPPORT)

#pragma CODE_SECTION(MMC_sendAllCID,".text:MMC_sendALLCID")

/**************************************************************************
* MMC_sendAllCID(MMC_Handle mmc, MMC_CardIdObj *cid)
*          mmc     - Handle of MMC device reserved by previous call to 
*                    MMC_open()
*          cid     - pointer to card ID object
***************************************************************************/

int MMC_sendAllCID(MMC_Handle mmc, MMC_CardIdObj *cid) {
  
  Uint16 old_intm;
  Uint16 stat;
  ioport MMC_MmcRegObj *regs = mmc->mmcRegs;
  
  MMC_ASSERT_HANDLE(mmc,return);
  old_intm = IRQ_globalDisable();

  // Clear out response registers

  MMC_clearResponse(mmc);
  MMC_getStatus(mmc,MMC_ST0_RSPDNE);

  // Format and send command
  
  regs->mmcargh = MMC_STUFF_BITS;
  regs->mmcargl = MMC_STUFF_BITS;
  regs->mmccmd  = MMC_ALL_SEND_CID;

  // Wait for response done
  while ((stat=MMC_responseDone(mmc))==0)
                   ;

  if (stat==MMC_RESPONSE_TIMEOUT)
    return 0;

  mmc->numCards++;

  MMC_getCardID(mmc, cid);

  IRQ_globalRestore(old_intm);

  return 1;
}
#endif
  
