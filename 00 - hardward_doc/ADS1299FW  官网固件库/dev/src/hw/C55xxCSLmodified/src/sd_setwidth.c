/****************************************************************************\
*           Copyright (C) 1999-2000 Texas Instruments Incorporated.
*                           All Rights Reserved 
*------------------------------------------------------------------------------
* MODULE NAME... MMC
* FILENAME...... sd_setwidth.c
* DATE CREATED.. Thu Nov 06 2003
* PROJECT....... Chip Support Library
* COMPONENT..... Service Layer
* PREREQUISITS.. 
* Note: This functionality is present only for SD card.
*------------------------------------------------------------------------------
*
* HISTORY:
*	 Created:	Thu Nov 06 2003	
*
*------------------------------------------------------------------------------
* DESCRIPTION: sets width to 4 or 1 as per SD width required
\*************************************************************************/

#define _MMC_MOD_
/*************************************************************************\
* Included Header Files                                                  *
\*************************************************************************/

#include <csl_mmc.h>
#include <csl_mmchal.h>
#include <csl_pwr.h>
#include <csl_chiphal.h>

#define OP_TIMEOUT 1024

#if (_MMC_SUPPORT)

#pragma CODE_SECTION(SD_setWidth,".text:SD_setWidth")

/**************************************************************************
* SD_setWidth(MMC_Handle sd, Uint16 width)
*              width is 1 or 4          
*          
***************************************************************************/
int SD_setWidth(MMC_Handle sd, Uint16 width){
   Uint16 old_intm;
   Uint16 old_mmcctl;
   Uint16 stat, rca;
   int count;
   ioport MMC_MmcRegObj *regs = sd->mmcRegs;

   MMC_ASSERT_HANDLE(sd, return);

   if (width == 0x1){
      width = 0; //set width field in MMCCTL to 0
      MMC_getStatus(sd, MMC_ST0_RSPDNE);
    
      rca = sd->cards->rca;
      MMC_clearResponse(sd);
      regs->mmcargh = rca;
      regs->mmcargl = MMC_STUFF_BITS;
      regs->mmccmd  = SD_APP_CMD;
    
      stat = 0;
      count = OP_TIMEOUT;
      while (!(stat & 0x4) && (count-- > 0))
         stat = MMC_getStatus(sd, MMC_ST0_RSPDNE);

      if (count <= 0)
         return -1;
    
      regs->mmcargh = MMC_STUFF_BITS;
      regs->mmcargl = 0x00;
      regs->mmccmd  = SD_SET_BUS_WIDTH;

      stat = 0;
      count = OP_TIMEOUT;

      while (!(stat & 0x4) && (count-- > 0))
         stat = MMC_getStatus(sd,MMC_ST0_RSPDNE);

      if (count <= 0)
         return -1;
    
   } else if (width == 0x4) {
      width = 1; //set width field in MMCCTL to 1
      MMC_getStatus(sd, MMC_ST0_RSPDNE);
    
      rca = sd->cards->rca;
      MMC_clearResponse(sd);
      regs->mmcargh = rca;
      regs->mmcargl = MMC_STUFF_BITS;
      regs->mmccmd  = SD_APP_CMD;
    
      stat = 0;
      count = OP_TIMEOUT;
      while (!(stat & 0x4) && (count-- > 0))
         stat = MMC_getStatus(sd, MMC_ST0_RSPDNE);

      if (count <= 0)
         return -1;
    
      regs->mmcargh = MMC_STUFF_BITS;
      regs->mmcargl = 0x2;
      regs->mmccmd  = SD_SET_BUS_WIDTH;

      stat = 0;
      count = OP_TIMEOUT;

      while (!(stat & 0x4) && (count-- > 0))
         stat = MMC_getStatus(sd,MMC_ST0_RSPDNE);

      if (count <= 0)
         return -1;
    
   } else {
      return -1; //this value is not acceptable
   }
   
   old_intm = IRQ_globalDisable();

   old_mmcctl   = regs->mmcctl;
   regs->mmcctl = 0x3; //reset it before writing new value
   old_mmcctl  |= _MMC_MMCCTL_WIDTH_MK(width);
   regs->mmcctl = old_mmcctl & 0xFFFC;
   
   IRQ_globalRestore(old_intm);
   return 1;
}
#endif
