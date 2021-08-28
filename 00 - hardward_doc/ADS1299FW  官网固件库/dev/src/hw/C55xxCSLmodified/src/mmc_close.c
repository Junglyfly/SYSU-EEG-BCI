/****************************************************************************\
*           Copyright (C) 1999-2000 Texas Instruments Incorporated.
*                           All Rights Reserved 
*------------------------------------------------------------------------------
* MODULE NAME... MMC
* FILENAME...... mmc_close.c
* DATE CREATED.. Thu May 24 14:48:09 2001
* PROJECT....... Chip Support Library
* COMPONENT..... Service Layer
* PREREQUISITS.. 
*------------------------------------------------------------------------------
*
* HISTORY:
*	 Created:		Thu May 24 14:48:09 2001 (Automatic generation)
*	 Modified:	Changed #include csl_ebselhal.h to csl_chiphal.h
*
*------------------------------------------------------------------------------
* DESCRIPTION: Closes specified MMC device, previously opened by call
*              to MMC_open() 
*
\*************************************************************************/
#define _MMC_MOD_
/*************************************************************************\
* Included Header Files                                                  *
\*************************************************************************/

#include <csl_mmc.h>


#if (_MMC_SUPPORT)

#include <csl_chiphal.h>

#pragma CODE_SECTION(MMC_close,".text:MMC_close")

/**************************************************************************
* MMC_close(MMC_Handle mmc)
*          mmc     - Handle of MMC device reserved by previous call to 
*                    MMC_open()
***************************************************************************/

void MMC_close(MMC_Handle mmc) {
  Uint16 old_intm;
  Uint16 dev = mmc->devNum;
  ioport MMC_MmcRegObj *regs = mmc->mmcRegs;

  MMC_ASSERT_HANDLE(mmc,return);

  old_intm = IRQ_globalDisable();

  // Place Controller in Reset //
  regs->mmcctl = 0x3u;

  // Clear all response registers *//
  MMC_clearResponse(mmc);
 

  // Free Allocated Port (for both MMC and MCBSP) //
  CSL_SYS_DATA.MmcAllocMask &= ~(1u << dev);
  CSL_SYS_DATA.McbspPortAllocMask &= ~(1u << (dev+1));

  // Set System Register to configure Port as MCBSP port //
  _XBSR &= ~(0x3u << (dev ? 4 : 2));

  IRQ_globalRestore(old_intm);

}
#endif
