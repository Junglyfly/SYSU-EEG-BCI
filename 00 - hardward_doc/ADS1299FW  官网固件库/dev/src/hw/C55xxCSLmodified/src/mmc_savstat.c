/****************************************************************************\
*           Copyright (C) 1999-2000 Texas Instruments Incorporated.
*                           All Rights Reserved 
*------------------------------------------------------------------------------
* MODULE NAME... MMC
* FILENAME...... mmc_savstat.c
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
* DESCRIPTION: Saves current status of MMC
\*************************************************************************/
#define _MMC_MOD_
/*************************************************************************\
* Included Header Files                                                  *
\*************************************************************************/

#include <csl_mmc.h>

#if (_MMC_SUPPORT)

#pragma CODE_SECTION(MMC_saveStatus,".text:MMC_saveStatus")

/**************************************************************************
* MMC_saveStatus(MMC_Handle mmc)
*          mmc      - Handle of MMC device reserved by previous call to 
*                     MMC_open()
*  Saves current contents of MMC ST0 register and saves it in MMC Handle
***************************************************************************/

int MMC_saveStatus(MMC_Handle mmc) {
  Uint16 old_intm;
  ioport MMC_MmcRegObj *regs = mmc->mmcRegs;

  MMC_ASSERT_HANDLE(mmc,return);

  old_intm = IRQ_globalDisable();

  mmc->status = regs->mmcst0;

  IRQ_globalRestore(old_intm);

  return mmc->status;
}
#endif
 
