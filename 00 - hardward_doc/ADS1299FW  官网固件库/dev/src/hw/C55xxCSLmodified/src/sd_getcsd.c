/****************************************************************************\
*           Copyright (C) 1999-2000 Texas Instruments Incorporated.
*                           All Rights Reserved 
*------------------------------------------------------------------------------
* MODULE NAME... MMC
* FILENAME...... sd_getcsd.c
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

#pragma CODE_SECTION(SD_getCardCsd,".text:SD_getCardcsd")

/**************************************************************************
* SD_getCardCSD(MMC_Handle sd, SD_CardCsdObj *csd)
*          sd     - Handle of MMC device reserved by previous call to 
*                    MMC_open()
*          csd     - pointer to card CSD object
***************************************************************************/

void SD_getCardCsd(MMC_Handle sd, SD_CardCsdObj *csd) {
  Uint16 old_intm;
  ioport Uint16 *regs;
  Uint16 lo;

  MMC_ASSERT_HANDLE(sd,return);
  
  old_intm = IRQ_globalDisable();
  
  regs = (Uint16 *)&(sd->mmcRegs->mmcrsp);

  csd->crc                = (regs[0] >>   1 ) & 0x7F;      // 1:7
  csd->fileFmt            = (regs[0] >>  10 ) & 0x3;         // 10:11
  csd->tmpWriteProtect    = (regs[0] >>  12 ) & 0x1;         // 12:12
  csd->permWriteProtect   = (regs[0] >>  13 ) & 0x1;         // 13:13
  csd->copy               = (regs[0] >>  14 ) & 0x1;         // 14:14
  csd->fileFmtGrp         = (regs[0] >>  15 ) & 0x1;         // 15:15
  
  csd->writeBlPartial     = (regs[1] >>   5 ) & 0x1;         // 21:21 
  csd->writeBlLen         = (regs[1] >>   6 ) & 0xF;       // 25:22
  csd->r2wFactor          = (regs[1] >>  10 ) & 0x7;       // 28:26
  csd->wpGrpEnable        = (regs[1] >>  15 ) & 0x1;         // 31:31
  
  csd->wpGrpSize          = (regs[2] & 0x7F );             // 32:38
  csd->sectorSize         = (regs[2] >>  7  ) & 0x7F;      // 39:45
  csd->eraseBlkEn         = (regs[2] >>  14 ) & 0x1;       // 46:46
  
      /* read the MSB of resp2 and 2 LSB of resp3 for this value */
  lo                      = (regs[2] >>  15 ) & 0x1;         // 49:47 
  csd->cSizeMult          = ((regs[3] &  3  ) << 1) | lo;
      
  csd->vddWCurrMax        = (regs[3]  >> 2  ) & 0x7;       // 52:50
  csd->vddWCurrMin        = (regs[3]  >> 5  ) & 0x7;       // 55:53
  csd->vddRCurrMax        = (regs[3]  >> 8  ) & 0x7;       // 58:56
  csd->vddRCurrMin        = (regs[3]  >> 11 ) & 0x7;       // 61:59
  
      /* read the 2 MSB of resp3 and 10 LSB of resp4 for this value */
  lo                      = (regs[3]  >> 14 ) & 0x3;      // 73:62
  csd->cSize              = ((regs[4]  & 0x3FF) << 2) | lo; 
      
  csd->dsrImp             = (regs[4]  >> 12  ) & 0x1;        // 76:76
  csd->readBlkMisalign    = (regs[4]  >> 13  ) & 0x1;        // 77:77
  csd->writeBlkMisalign   = (regs[4]  >> 14  ) & 0x1;        // 78:78
  csd->readBlPartial      = (regs[4]  >> 15  ) & 0x1;        // 79:79
  
  csd->readBlLen          = (regs[5]) & 0xF;               // 83:80
  csd->ccc                = (regs[5] >>  4 ) & 0xFFF;      // 95:84
  csd->tranSpeed          = (regs[6]) & 0xFF;              // 103:96
  csd->nsac               = (regs[6]  >>  8 ) & 0xFF;      // 111:104
  
  csd->taac               = (regs[7]) & 0xFF;              // 119:112
  csd->csdStructure       = (regs[7]  >> 14 ) & 0x3; 

  IRQ_globalRestore(old_intm);
}
#endif

  
