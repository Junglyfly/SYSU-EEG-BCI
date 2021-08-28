/****************************************************************************\
*           Copyright (C) 1999-2000 Texas Instruments Incorporated.
*                           All Rights Reserved 
*------------------------------------------------------------------------------
* MODULE NAME... MMC
* FILENAME...... mmc_init.c
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
* DESCRIPTION: Changes control register settings based on control
*              parameters
\*************************************************************************/
#define _MMC_MOD_
/*************************************************************************\
* Included Header Files                                                  *
\*************************************************************************/

#include <csl_mmc.h>
#include <csl_pwr.h>

#if (_MMC_SUPPORT)

#pragma CODE_SECTION(MMC_init,".text:MMC_init")

/**************************************************************************
*  MMC_init(
*    MMC_Handle mmc,           // Handle returned from call to MMC_open   
*    Uint16 dmaEnable,         // Enable/disable DMA for data read/write 
*    Uint16 dat3EdgeDetection, // Set level of edge detection for DAT3 pin 
*    Uint16 goIdle,            // Determines if MMC goes IDLE during IDLE instr 
*    Uint16 enableClkPin,      // Memory clk reflected on CLK Pin 
*    Uint32 fdiv,              // CPU CLK to MMC function clk divide down 
*    Uint32 cdiv,              // MMC function clk to memory clk divide down 
*    Uint16 rspTimeout,        // No. memory clks to wait before response timeout 
*    Uint16 dataTimeout,       // No. memory clks to wait before data timeout 
*    Uint16 blenOrCrc          // Sets Block Length for Native Mode or
*                                 Enables/disables CRC checking in SPI mode 
*
***************************************************************************/

int MMC_init(
    MMC_Handle mmc,           // Handle returned from call to MMC_open   
    Uint16 dmaEnable,         // Enable/disable DMA for data read/write 
    Uint16 dat3EdgeDetection, // Set level of edge detection for DAT3 pin 
    Uint16 goIdle,            // Determines if MMC goes IDLE during IDLE instr 
    Uint16 enableClkPin,      // Memory clk reflected on CLK Pin 
    Uint32 fdiv,              // CPU CLK to MMC function clk divide down 
    Uint32 cdiv,              // MMC function clk to memory clk divide down 
    Uint16 rspTimeout,        // No. memory clks to wait before response timeout 
    Uint16 dataTimeout,       // No. memory clks to wait before data timeout 
    Uint16 blenOrCrc          // Sets Block Length for Native Mode
){

  ioport MMC_MmcRegObj *regs;
  Uint16 old_intm;

  MMC_ASSERT_HANDLE(mmc, return FALSE );

  old_intm = IRQ_globalDisable();

  // Perform Initialization //
  regs = mmc->mmcRegs;



  regs->mmcctl |= 0x3;          // Place controller in reset
  regs->mmcctl |= _MMC_MMCCTL_DMAEN_MK(dmaEnable) |
                  _MMC_MMCCTL_DATEG_MK(dat3EdgeDetection);

  regs->mmcfclk = _MMC_MMCFCLK_FDIV_MK(fdiv);
  regs->mmcclk  = _MMC_MMCCLK_CDIV_MK(cdiv);

  if (goIdle) {
    regs->mmcfclk |= _MMC_MMCFCLK_IDLEEN_MK(1);
    _ICR |= 0x1u;
  }

  regs->mmctor = _MMC_MMCTOR_TOR_MK(rspTimeout);
  regs->mmctod = _MMC_MMCTOD_TOD_MK(dataTimeout);
  

  
  regs->mmcblen = blenOrCrc;
  regs->mmcctl &= 0xFFFC;     // Take MMC out of reset //
  
  if (enableClkPin)
    regs->mmcclk |= _MMC_MMCCLK_CLKEN_MK(1);

  IRQ_globalRestore(old_intm);

  return mmc->numCards;
}
#endif
 
