/************************************************************************\
*           Copyright (C) 1999-2000 Texas Instruments Incorporated.
*                           All Rights Reserved 
*------------------------------------------------------------------------
* MODULE NAME... MMC
* FILENAME...... csl_sendrca.h
* DATE CREATED.. Fri Nov 7 2003
* PROJECT....... Chip Support Library
* COMPONENT..... Service Layer
* PREREQUISITS.. 
*------------------------------------------------------------------------
*
* HISTORY:
*	 Created:	Fri Nov 7 2003
*------------------------------------------------------------------------
* DESCRIPTION: Asks SD card to send its RCA after initialization
\*************************************************************************/

//#ifdef _MMC_SUPPORT

#include <csl_mmc.h>

#if (_MMC_SUPPORT)

#pragma CODE_SECTION(SD_sendRca,".text:SD_sendRca")
int SD_sendRca(MMC_Handle sd, MMC_CardObj *card) {
   Uint16 old_intm;
   Uint16 stat;
   Uint16 rca;
   ioport MMC_MmcRegObj *regs = sd->mmcRegs;
 
   MMC_ASSERT_HANDLE(sd, return);
   old_intm = IRQ_globalDisable();

   // Clear out response registers

   MMC_clearResponse(sd);
   MMC_getStatus(sd, MMC_ST0_RSPDNE);

   regs->mmcargh = MMC_STUFF_BITS;
   regs->mmcargl = MMC_STUFF_BITS;
   regs->mmccmd  = SD_SEND_RELATIVE_ADDR;

   while ((stat = MMC_responseDone(sd)) == 0)
                       ;
   /* Response is R6. Bits 8:23 are status. Bits 24:39 are new RCA. Response is split across
      registers MMCRSP5, 6, 7 and MMCCIDX. We need to read registers 6 and 7 for rca and status */
   
   stat = MMC_RGET_H(sd,MMCRSP6);
   rca  = MMC_RGET_H(sd,MMCRSP7);
 
   card->rca = rca;
   sd->cards = card;
   IRQ_globalRestore(old_intm);
   return rca;
}

#endif
