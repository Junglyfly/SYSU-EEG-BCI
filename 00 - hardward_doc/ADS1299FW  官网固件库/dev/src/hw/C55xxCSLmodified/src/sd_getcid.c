/****************************************************************************\
*           Copyright (C) 1999-2000 Texas Instruments Incorporated.
*                           All Rights Reserved 
*------------------------------------------------------------------------------
* MODULE NAME... MMC
* FILENAME...... sd_getcid.c
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
* DESCRIPTION: Parses CID information from MMC response registers. Assumes
*              either ALL_SEND_CID or SEND_CID command has already been
*              issued and response is complete.
\*************************************************************************/
#define _MMC_MOD_
/*************************************************************************\
* Included Header Files                                                  *
\*************************************************************************/

#include <csl_mmc.h>

#if (_MMC_SUPPORT)

#pragma CODE_SECTION(SD_getCardID,".text:SD_getCardID")

/**************************************************************************
* SD_getCardID(MMC_Handle sd, MMC_CardIDObj *cid)
*          sd     - Handle of MMC device reserved by previous call to 
*                    MMC_open()
*          cid     - pointer to card ID object
***************************************************************************/

void SD_getCardID(MMC_Handle sd, MMC_CardIdObj *cid) {
  Uint16 old_intm;
  Uint16 lo,hi,mid;
  ioport MMC_MmcRegObj *regs = sd->mmcRegs;

  MMC_ASSERT_HANDLE(sd,return);

  old_intm = IRQ_globalDisable();
 
  cid->monthCode      = (regs->mmcrsp.rsp0 >> 8 ) & 0xF;     // 11:8
 
  lo                  = (regs->mmcrsp.rsp0 >> 12) & 0xF;     // 12:15
  hi                  = (regs->mmcrsp.rsp1      ) & 0xF;     // 16:19
  cid->yearCode       = ((Uint32)hi << 4) | lo;
  
  lo                  = (regs->mmcrsp.rsp1  >> 8) & 0xFF;    // 24:31
  mid                 = (regs->mmcrsp.rsp2);                 // 32:47 
  hi                  = (regs->mmcrsp.rsp3)       & 0xFF;    // 48:55
  cid->serialNumber   = ((Uint32)hi << 24) | ((Uint32)mid << 8) | lo;               
  
  cid->fwRev          = (regs->mmcrsp.rsp3 >> 8 ) & 0xF;     // 48:51
  cid->hwRev          = (regs->mmcrsp.rsp3 >> 12) & 0xF;     // 52:55
  
  lo                  = (regs->mmcrsp.rsp6 >> 8 ) & 0xFF;    // 111:104
  hi                  = (regs->mmcrsp.rsp7);                 // 112:127
  cid->mfgId          = ((Uint32)hi << 16) | lo;   	

  cid->productName[0] = ((regs->mmcrsp.rsp4)       & 0xFF);
  cid->productName[1] = ((regs->mmcrsp.rsp4  >> 8) & 0xFF);
  cid->productName[2] = ((regs->mmcrsp.rsp5)       & 0xFF);
  cid->productName[3] = ((regs->mmcrsp.rsp5) >> 8) & 0xFF;
  cid->productName[4] = ((regs->mmcrsp.rsp6)       & 0xFF);

  IRQ_globalRestore(old_intm);
}
#endif
 
