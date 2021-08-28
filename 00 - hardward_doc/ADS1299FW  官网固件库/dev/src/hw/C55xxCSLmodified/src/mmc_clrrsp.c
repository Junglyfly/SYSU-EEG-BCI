/****************************************************************************\
*           Copyright (C) 1999-2000 Texas Instruments Incorporated.
*                           All Rights Reserved 
*------------------------------------------------------------------------------
* MODULE NAME... MMC
* FILENAME...... mmc_clrrsp.c
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
* DESCRIPTION: Clears all response registers associated with specified
*              MMC controller. 
*
\*************************************************************************/
#define _MMC_MOD_
/*************************************************************************\
* Included Header Files                                                  *
\*************************************************************************/

#include <csl_mmc.h>

#if (_MMC_SUPPORT)

#pragma CODE_SECTION(MMC_clearResponse,".text:MMC_clearResponse")

/**************************************************************************
* MMC_clearResponse(MMC_Handle mmc)
*          mmc     - Handle of MMC device reserved by previous call to 
*                    MMC_open()
***************************************************************************/

void MMC_clearResponse(MMC_Handle mmc) {
  Uint16 old_intm;
  ioport Uint16 *rsp;
  Uint16 i;

  MMC_ASSERT_HANDLE(mmc,return);

  old_intm = IRQ_globalDisable();
  for (i=0; i<=20; ++i)
      asm("   NOP");
        
  rsp = (Uint16 *)&(mmc->mmcRegs->mmcrsp);

  // Clear all response registers //
  for (i=0; i<= 7; i++)
      rsp[i] = 0;

  IRQ_globalRestore(old_intm);
}
#endif
 
