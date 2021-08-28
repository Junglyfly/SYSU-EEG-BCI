/****************************************************************************\
*           Copyright (C) 1999-2000 Texas Instruments Incorporated.
*                           All Rights Reserved 
*------------------------------------------------------------------------------
* MODULE NAME... MMC
* FILENAME...... mmc_cfga.c
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
*              arguments pased to function.
*
\*************************************************************************/
#define _MMC_MOD_
/*************************************************************************\
* Included Header Files                                                  *
\*************************************************************************/

#include <csl_mmc.h>

#if (_MMC_SUPPORT)

/**************************************************************************
* MMC_configArgs(MMC_Handle mmc, Uint16 mmcctl, Uint16 mmcfclk, Uint16 mmcclk,
*            Uint16 mmcie, Uint16 mmctor, Uint16 mmctod, Uint16 mmcblen,
*            Uint16 mmcnblk)
*          mmc     - Handle for MMC device previously returned from call
*                    to MMC_open
*          mmcctl  - Value to write to MMC control register MMCCTL
*          mmcfclk - Value to write to MMC function clock register MMCFCLK
*          mmcclk  - Value to write to MMC clock register MMCCLK
*          mmcie   - Value to write to MMC interrupt enable mask register MMCIE
*          mmctor  - Value to write to MMC response timeout register MMCTOR
*          mmctod  - Value to write to MMC data receive timeout reg MMCTOD
*          mmcblen - Value to write to MMC block length register MMCBLEN
*          mmcnblk - Value to write to MMC no blocks in transfer regsiter MMCNBLK
***************************************************************************/

#pragma CODE_SECTION(MMC_configArgs,".text:MMC_configArgs")

void MMC_configArgs(MMC_Handle mmc, Uint16 mmcctl, Uint16 mmcfclk, Uint16 mmcclk,
           Uint16 mmcie, Uint16 mmctor, Uint16 mmctod, Uint16 mmcblen,
           Uint16 mmcnblk) {

  Uint16 old_intm;
  Uint16 resetMask = _MMC_MMCCTL_CMDRST_MK(1) | _MMC_MMCCTL_DATRST_MK(1);
  ioport MMC_MmcRegObj *regs;


  MMC_ASSERT_HANDLE(mmc,return);

  old_intm = IRQ_globalDisable();
  // Perform Initialization //
  regs = mmc->mmcRegs;

  // Place controller in reset before changing other values
  
  regs->mmcctl = resetMask; 
  regs->mmcctl = mmcctl & (~(resetMask));

  regs->mmcfclk = mmcfclk;
  regs->mmcclk  = mmcclk;
  regs->mmcie   = mmcie;
  regs->mmctor  = mmctor;
  regs->mmctod  = mmctod;
  regs->mmcblen = mmcblen;
  regs->mmcnblk = mmcnblk;
  regs->mmcctl &= 0xFFFC;     // Take MMC out of reset //

  IRQ_globalRestore(old_intm);
}
#endif 
