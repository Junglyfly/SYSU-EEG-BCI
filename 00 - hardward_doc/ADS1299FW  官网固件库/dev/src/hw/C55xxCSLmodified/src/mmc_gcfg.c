/****************************************************************************\
*           Copyright (C) 1999-2000 Texas Instruments Incorporated.
*                           All Rights Reserved 
*------------------------------------------------------------------------------
* MODULE NAME... MMC
* FILENAME...... mmc_gcfg.c
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


#if (_MMC_SUPPORT)

#pragma CODE_SECTION(MMC_getConfig,".text:MMC_getConfig")

/**************************************************************************
* MMC_getConfig(MMC_Handle mmc, MMC_Config *cfg)
*          mmc     - Handle for MMC device previously returned from call
*                    to MMC_open
*          cfg     - MMC Configuration structure containing values of the 
*                    MMC control registers returned by the function.
***************************************************************************/

void MMC_getConfig(MMC_Handle mmc, MMC_Config *cfg){

  Uint16 old_intm;
  ioport MMC_MmcRegObj *regs;

  MMC_ASSERT_HANDLE(mmc,return);

  old_intm = IRQ_globalDisable();
  regs = mmc->mmcRegs;

  
  cfg->mmcctl = regs->mmcctl; 
  cfg->mmcfclk = regs->mmcfclk;
  cfg->mmcclk = regs->mmcclk;
  cfg->mmcie = regs->mmcie;
  cfg->mmctor = regs->mmctor;
  cfg->mmctod = regs->mmctod;
  cfg->mmcblen = regs->mmcblen;
  cfg->mmcnblk = regs->mmcnblk;

  IRQ_globalRestore(old_intm);
}
#endif

  
