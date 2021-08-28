/****************************************************************************\
*           Copyright (C) 1999-2000 Texas Instruments Incorporated.
*                           All Rights Reserved 
*------------------------------------------------------------------------------
* MODULE NAME... MMC
* FILENAME...... mmc_setnat.c
* DATE CREATED.. Thu May 24 14:48:09 2001
* PROJECT....... Chip Support Library
* COMPONENT..... Service Layer
* PREREQUISITS.. 
*------------------------------------------------------------------------------
*
* HISTORY:
*	 Created:		Thu May 24 14:48:09 2001 (Automatic generation)
*	 Modified:	04/04/2002 Renamed function to MMC_setupNative
*      Modified:  05/07/2002 changed #include csl_ebselhal.h to csl_chiphal.h
*------------------------------------------------------------------------------
* DESCRIPTION: Initializes the MMC Controller for Native Mode 
*
\*************************************************************************/
#define _MMC_MOD_
/*************************************************************************\
* Included Header Files                                                  *
\*************************************************************************/

#include <csl_mmc.h>
#include <csl_pwr.h>
#include <csl_chiphal.h>

#if (_MMC_SUPPORT)

#pragma CODE_SECTION(MMC_setupNative,".text:MMC_setupNative")

/**************************************************************************
* MMC_setupNative(Uint16 dev, MMC_SetupNative *mmcInit)
*          mmc     - MMC Handle
* 
*          mmcInit - pointer to structure contianing initial register
*                    configuration information
***************************************************************************/

void MMC_setupNative(MMC_Handle mmc, MMC_SetupNative *mmcInit) {
  ioport MMC_MmcRegObj *regs;
  Uint16 old_intm;
  

  MMC_SetupNative *init = (MMC_SetupNative*)mmcInit;
  
  MMC_ASSERT_HANDLE(mmc, return FALSE );

  old_intm = IRQ_globalDisable();
  

  // Perform Initialization //
  regs = mmc->mmcRegs;

  regs->mmcctl |= 0x3;          // Place controller in reset
  regs->mmcctl |= _MMC_MMCCTL_DMAEN_MK(init->dmaEnable) |
                 _MMC_MMCCTL_DATEG_MK(init->dat3EdgeDetection);
                
  regs->mmcfclk = _MMC_MMCFCLK_FDIV_MK(init->fdiv);
  regs->mmcclk  = _MMC_MMCCLK_CDIV_MK(init->cdiv);

  if (init->goIdle) {
    regs->mmcfclk |= _MMC_MMCFCLK_IDLEEN_MK(1);
    _ICR |= 0x1u;
  }

  regs->mmctor = _MMC_MMCTOR_TOR_MK(init->rspTimeout);
  regs->mmctod = _MMC_MMCTOD_TOD_MK(init->dataTimeout);
  regs->mmcblen = _MMC_MMCBLEN_BLEN_MK(init->blockLen);

  regs->mmcctl &= 0xFFFC;     // Take MMC out of reset //
  if (init->enableClkPin)
    regs->mmcclk |= _MMC_MMCCLK_CLKEN_MK(1);

  IRQ_globalRestore(old_intm);

}
#endif  
