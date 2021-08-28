/****************************************************************************\
*           Copyright (C) 1999-2000 Texas Instruments Incorporated.
*                           All Rights Reserved 
*------------------------------------------------------------------------------
* MODULE NAME... MMC
* FILENAME...... mmc_waitfor.c
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

#pragma CODE_SECTION(MMC_waitForFlag,".text:MMC_waitForFlag")

/**************************************************************************
* MMC_waitForFlag(MMC_Handle mmc, Uint16 mask)
*          mmc      - Handle of MMC device reserved by previous call to 
*                     MMC_open()
*          mask     - Mask of status flags to wait for (ST0)
*  Returns contents of MMC ST0 register.
***************************************************************************/

int MMC_waitForFlag(MMC_Handle mmc, Uint16 mask) {
  Uint16 done;
  ioport MMC_MmcRegObj *regs = mmc->mmcRegs;

  MMC_ASSERT_HANDLE(mmc,return);

 done = 0;
  while (!done) {
    mmc->status = regs->mmcst0;
    if (!((mmc->status & mask)==0))
       done = 1;
  }
  return done;
}
  
#endif
