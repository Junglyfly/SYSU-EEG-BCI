/****************************************************************************\
*           Copyright (C) 1999-2000 Texas Instruments Incorporated.
*                           All Rights Reserved 
*------------------------------------------------------------------------------
* MODULE NAME... MMC
* FILENAME...... mmc_getcid.c
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

#pragma CODE_SECTION(MMC_getCardID,".text:MMC_getCardID")

/**************************************************************************
* MMC_getCardID(MMC_Handle mmc, MMC_CardIDObj *cid)
*          mmc     - Handle of MMC device reserved by previous call to 
*                    MMC_open()
*          cid     - pointer to card ID object
***************************************************************************/

void MMC_getCardID(MMC_Handle mmc, MMC_CardIdObj *cid) {
  Uint16 old_intm;
  Uint16 lo,hi;
  ioport MMC_MmcRegObj *regs = mmc->mmcRegs;

  MMC_ASSERT_HANDLE(mmc,return);

  old_intm = IRQ_globalDisable();

  cid->yearCode       = (regs->mmcrsp.rsp0 >> 8 ) & 0xF;     // 11:8
  cid->monthCode      = (regs->mmcrsp.rsp0 >> 12) & 0xF;     // 12:15
     
  lo                  = (regs->mmcrsp.rsp1);                 // 16:31
  hi                  = (regs->mmcrsp.rsp2);                 // 32:47 
  cid->serialNumber   = ((Uint32)hi << 16) | lo;               
     
  cid->fwRev          = (regs->mmcrsp.rsp3) & 0x000F;        // 48:51
  cid->hwRev          = (regs->mmcrsp.rsp3 >> 4) & 0x000F;   // 52:55
     
  lo                  = (regs->mmcrsp.rsp6 >> 8) & 0x00FF;   // 111:104
  hi                  = (regs->mmcrsp.rsp7);                 // 112:127
  cid->mfgId          = ((Uint32)hi << 16) | lo;   	

  cid->productName[0] = ((regs->mmcrsp.rsp3) >> 8) & 0xFF;
  cid->productName[1] = ((regs->mmcrsp.rsp4)       & 0xFF);
  cid->productName[2] = ((regs->mmcrsp.rsp4  >> 8) & 0xFF);
  cid->productName[3] = ((regs->mmcrsp.rsp5)       & 0xFF);
  cid->productName[4] = ((regs->mmcrsp.rsp5) >> 8) & 0xFF;
  cid->productName[5] = ((regs->mmcrsp.rsp6)       & 0xFF);
     
  IRQ_globalRestore(old_intm);
}
#endif
 
