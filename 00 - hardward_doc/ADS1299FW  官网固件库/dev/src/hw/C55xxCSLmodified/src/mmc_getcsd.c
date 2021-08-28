/****************************************************************************\
*           Copyright (C) 1999-2000 Texas Instruments Incorporated.
*                           All Rights Reserved 
*------------------------------------------------------------------------------
* MODULE NAME... MMC
* FILENAME...... mmc_getcsd.c
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
* DESCRIPTION: Parses CSD information from MMC response registers. Assumes
*              either SEND_CSD command has already been issued and response
*              is complete.
\*************************************************************************/
#define _MMC_MOD_
/*************************************************************************\
* Included Header Files                                                  *
\*************************************************************************/

#include <csl_mmc.h>


#if (_MMC_SUPPORT)

#pragma CODE_SECTION(MMC_getCardCsd,".text:MMC_getCardcsd")

/**************************************************************************
* MMC_getCardCSD(MMC_Handle mmc, MMC_CardCsdObj *csd)
*          mmc     - Handle of MMC device reserved by previous call to 
*                    MMC_open()
*          csd     - pointer to card CSD object
***************************************************************************/

void MMC_getCardCsd(MMC_Handle mmc, MMC_CardCsdObj *csd) {
  Uint16 old_intm;
  ioport Uint16 *regs;
  Uint16 lo;

  MMC_ASSERT_HANDLE(mmc,return);
  
  old_intm = IRQ_globalDisable();
  
  regs = (Uint16 *)&(mmc->mmcRegs->mmcrsp);

  csd->crc                = (regs[0] >>  1  ) & 0x7F;     // 7:1
  csd->ecc                = (regs[0] >>  8  ) & 0x3;      // 9:8
  csd->fileFmt            = (regs[0] >>  10 ) & 0x3;      // 11:10
  csd->tmpWriteProtect    = (regs[0] >>  12 ) & 0x1;      // 12:12
  csd->permWriteProtect   = (regs[0] >>  13 ) & 0x1;      // 13:13
  csd->copy               = (regs[0] >>  14 ) & 0x1;      // 14:14
  csd->fileFmtGrp         = (regs[0] >>  15 ) & 0x1;      // 15:15
  csd->writeBlPartial     = (regs[1] >>   5 ) & 0x1;      // 21:21 
  csd->writeBlLen         = (regs[1] >>   6 ) & 0xF;      // 25:22
  csd->r2wFactor          = (regs[1] >>  10 ) & 0x7;      // 28:26
  csd->defaultEcc         = (regs[1] >>  13 ) & 0x3;      // 30:29 
  csd->wpGrpEnable        = (regs[1] >>  15 ) & 0x1;      // 31:31
	
  csd->wpGrpSize          = (regs[2] & 0x1F);             // 36:32
  csd->eraseGrpMult       = (regs[2]  >>  5 ) & 0x1F;     // 41:37
  csd->eraseGrpSize       = (regs[2]  >> 10 ) & 0x1F;     // 46:42
  lo                      = (regs[2]  >> 15 ) & 1;        // 49:47
  csd->cSizeMult          = ((regs[3] &  3  ) << 1) | lo;
	
  csd->vddWCurrMax        = (regs[3]  >> 2  ) & 0x7;      // 52:50
  csd->vddWCurrMin        = (regs[3]  >> 5  ) & 0x7;      // 55:53
  csd->vddRCurrMax        = (regs[3]  >> 8  ) & 0x7;      // 58:56
  csd->vddRCurrMin        = (regs[3]  >> 11 ) & 0x7;      // 61:59
  lo                      = (regs[3]  >> 14 ) & 0x3;      // 73:62
  csd->cSize              = ((regs[4]  & 0x3FF) << 2) | lo; 
  csd->dsrImp             = (regs[4] >> 12  ) & 1;        // 76:76
  csd->readBlkMisalign    = (regs[4]  >> 13  ) & 1;       // 77:77
  csd->writeBlkMisalign   = (regs[4]  >> 14  ) & 1;       // 78:78
  csd->readBlPartial      = (regs[4]  >> 15  ) & 1;       // 79:79
	
  csd->readBlLen          = (regs[5]) & 0xF;              // 83:80
  csd->ccc                = (regs[5] >>  4 ) & 0xFFF;     // 95:84
  csd->tranSpeed          = (regs[6]) & 0xFF;             // 103:96
  csd->nsac               = (regs[6]  >>  8 ) & 0xFF;     // 111:104
	
  csd->taac               = (regs[7]) & 0xFF;             // 119:112
  csd->mmcProt            = (regs[7]  >> 10 ) & 0xF;      // 125:122
  csd->csdStructure       = (regs[7]  >> 14 ) & 0x3;      // 127:126

  IRQ_globalRestore(old_intm);
}
#endif

  
