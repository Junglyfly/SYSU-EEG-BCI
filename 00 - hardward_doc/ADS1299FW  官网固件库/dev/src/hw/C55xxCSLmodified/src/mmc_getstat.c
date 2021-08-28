/****************************************************************************\
*           Copyright (C) 1999-2000 Texas Instruments Incorporated.
*                           All Rights Reserved 
*------------------------------------------------------------------------------
* MODULE NAME... MMC
* FILENAME...... mmc_getstat.c
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

#pragma CODE_SECTION(MMC_getStatus,".text:MMC_getStatus")

/**************************************************************************
* MMC_getStatus(MMC_Handle mmc, Uint32 lmask)
*          mmc      - Handle of MMC device reserved by previous call to 
*                     MMC_open()
*         lmask     - mask of status flags to check
*  Returns contents of MMC ST0 register.
***************************************************************************/

int MMC_getStatus(MMC_Handle mmc, Uint32 lmask) {
  Uint16 mask = (Uint16)(lmask & 0xFFFFu);
  Uint16 flag = (((lmask>>16) & 0x8000u)>>15);
  Uint16 old_intm;
  Uint16 old_status;
  Uint16 value;
  ioport MMC_MmcRegObj *regs = mmc->mmcRegs;

  MMC_ASSERT_HANDLE(mmc,return);
  old_intm = IRQ_globalDisable();
  old_status = mmc->status;
  mmc->status = regs->mmcst0;
  if (flag)
    value = (old_status & mask) | (mmc->status & mask);
  else
    value = mmc->status & mask;

  
  IRQ_globalRestore(old_intm);

  return value;
}
#endif 
