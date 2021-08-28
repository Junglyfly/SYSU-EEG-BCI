/****************************************************************************\
*           Copyright (C) 1999-2000 Texas Instruments Incorporated.
*                           All Rights Reserved 
*------------------------------------------------------------------------------
* MODULE NAME... MMC
* FILENAME...... mmc_cfg.c
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
* DESCRIPTION: Writes values to MMC control registers specified in
*              MMC_Config structure passed as argument to call.
*
\*************************************************************************/
#define _MMC_MOD_
/*************************************************************************\
* Included Header Files                                                  *
\*************************************************************************/

#include <csl_mmc.h>
#include <csl_emif.h>

#if (_MMC_SUPPORT)

#pragma CODE_SECTION(MMC_config,".text:MMC_config")

/**************************************************************************
* MMC_config(MMC_Handle mmc, MMC_Config *cfg)
*          mmc     - Handle for MMC device previously returned from call
*                    to MMC_open
*          cfg     - MMC Configuration structure containing value to be
*                    assigned to MMC control registers.
***************************************************************************/

void MMC_config(MMC_Handle mmc, MMC_Config *cfg){

  Uint16 old_intm;
  Uint16 resetMask = _MMC_MMCCTL_CMDRST_MK(1) | _MMC_MMCCTL_DATRST_MK(1);
  ioport MMC_MmcRegObj *regs;

  MMC_ASSERT_HANDLE(mmc,return);

  old_intm = IRQ_globalDisable();
  // Perform Initialization //
  regs = mmc->mmcRegs;

  // Place controller in reset before changing other values
  
  regs->mmcctl = resetMask; 
  regs->mmcctl = cfg->mmcctl & (~(resetMask));

  regs->mmcfclk = cfg->mmcfclk;
  regs->mmcclk  =  cfg->mmcclk;
  regs->mmcie   = cfg->mmcie;
  regs->mmctor  = cfg->mmctor;
  regs->mmctod  = cfg->mmctod;
  regs->mmcblen = cfg->mmcblen;
  regs->mmcnblk = cfg->mmcnblk;
  regs->mmcctl &= 0xFFFC;     // Take MMC out of reset //

  IRQ_globalRestore(old_intm);
}
#endif

  
