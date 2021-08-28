/****************************************************************************\
*           Copyright (C) 1999-2000 Texas Instruments Incorporated.
*                           All Rights Reserved 
*------------------------------------------------------------------------------
* MODULE NAME... MMC
* FILENAME...... mmc_xcmd.c
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
* DESCRIPTION: Sends a command to the currently selected card. Will optionally
*              wait for response
\*************************************************************************/
#define _MMC_MOD_
/*************************************************************************\
* Included Header Files                                                  *
\*************************************************************************/

#include <csl_mmc.h>

#if (_MMC_SUPPORT)

#pragma CODE_SECTION(MMC_sendCmd,".text:MMC_sendCmd")

/**************************************************************************
* MMC_sendCmd(MMC_Handle mmc, Uint16 cmd, Uint16 argh, Uint16 argl, Uint16 waitForRsp)
*          mmc         - Handle of MMC device reserved by previous call to 
*                        MMC_open()
*          cmd         - Command to send to card
*          argh        - High part of command argument
*          argl        - Low part of command argument
*          waitForRsp  - If true, wait for respsonse before returning
***************************************************************************/

int MMC_sendCmd(MMC_Handle mmc, Uint16 cmd, Uint16 argh, 
                Uint16 argl, Uint16 waitForRsp) {

  Uint16 old_intm;
  ioport MMC_MmcRegObj *regs = mmc->mmcRegs;

  MMC_ASSERT_HANDLE(mmc, return);
  old_intm = IRQ_globalDisable();

  // Clear out response registers

  MMC_clearResponse(mmc);

  // Format and send command

  regs->mmcargh = argh;
  regs->mmcargl = argl;
  regs->mmccmd = cmd;

  IRQ_globalRestore(old_intm);

  // Wait for response done

  if (waitForRsp) {
    MMC_waitForFlag(mmc, MMC_ST0_RSPDNE | MMC_ST0_TOUTRS);
    if (mmc->status & MMC_ST0_TOUTRS)
       return 0;
  }

  return 1;
}
#endif

  
