/****************************************************************************\
*           Copyright (C) 1999-2000 Texas Instruments Incorporated.
*                           All Rights Reserved 
*------------------------------------------------------------------------------
* MODULE NAME... MMC
* FILENAME...... mmc_rspdne.c
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
* DESCRIPTION: Checks status register for response complete condition.
*              If timeout occurs before reponse done flag is set, function
*              returns an error condition of 0xFFFF = MMC_RESPONSE_TIMEOUT
\*************************************************************************/
#define _MMC_MOD_
/*************************************************************************\
* Included Header Files                                                  *
\*************************************************************************/

#include <csl_mmc.h>

#if (_MMC_SUPPORT)

#pragma CODE_SECTION(MMC_responseDone,".text:MMC_responseDone")

/**************************************************************************
* MMC_setRca(MMC_Handle mmc, Uint16 rca)
*          mmc     - Handle of MMC device reserved by previous call to 
*                    MMC_open()
*          rca     - User assigned relative card address
***************************************************************************/

int MMC_responseDone(MMC_Handle mmc) {
  Uint16 old_intm;

  MMC_ASSERT_HANDLE(mmc, return);
  old_intm = IRQ_globalDisable();

  // Wait for response done

  MMC_getStatus(mmc,MMC_ST0_RSPDNE);
  while (!MMC_getStatus(mmc,MMC_USE_LAST_STATUS | MMC_ST0_RSPDNE)) {
    if (MMC_getStatus(mmc,MMC_USE_LAST_STATUS | MMC_ST0_TOUTRS ))
       return MMC_RESPONSE_TIMEOUT;
  }

  IRQ_globalRestore(old_intm);

  return 1;
}
#endif

  
