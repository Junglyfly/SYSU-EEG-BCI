/****************************************************************************\
*           Copyright (C) 1999-2000 Texas Instruments Incorporated.
*                           All Rights Reserved 
*------------------------------------------------------------------------------
* MODULE NAME... MMC
* FILENAME...... mmc_xop.c
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
* DESCRIPTION: Sends braodcast request for cards to repond with their
*              operating voltage requirements Vdd. Returns the number
*              of cards that do not match the Vdd range specified in
*              the Host Vdd argument
\*************************************************************************/
#define _MMC_MOD_
/*************************************************************************\
* Included Header Files                                                  *
\*************************************************************************/
#include <stdarg.h>
#include <csl_mmc.h>

#if (_MMC_SUPPORT) && 0

#pragma CODE_SECTION(MMC_sendOpCond,".text:MMC_sendOpCond")

/**************************************************************************
* MMC_sendOpCond(MMC_Handle mmc, Uint16 hVddMask, Uint16 flag, ...)
*          mmc         - Handle of MMC device reserved by previous call to 
*                        MMC_open()
*          hVddMask    - Mask of acceptable host voltage ranges for data xfr
*          flag        - One of:
*                         1. MMC_VOLTAGE_RANGE_ONLY -  (sends broadcast command
*                           with hVddMask of 0 to determine card ranges only
*                         2. MMC_FORCE_CARDS_INACTIVE - (sends broadcast msg
*                            with hVddMask set to force all cards that
*                            cannot operate in host voltage range to go 
*                            inactive
*          *VddCards   - When flags = MMC_VOLTAGE_RANGE_ONLY, this argument is used
*                       to return the logical OR of all the valid voltage
*                       ranges returned by the connected cards. This allows
*                       the application to adjust/choose a voltage range
*                       that is acceptable, if voltage is adjustable.
*                        
***************************************************************************/

int MMC_sendOpCond(MMC_Handle mmc, Uint16 hVddMask, Uint16 flag, ...) {

  va_list argList;
  Uint16 done=FALSE;
  Uint16 rspDone;
  Uint16 noCardsOutOfRange=0;
  Uint16 *VddCards;
  Uint16 VddCard=0x0000u;
  ioport MMC_MmcRegObj *regs = mmc->mmcRegs;

  MMC_ASSERT_HANDLE(mmc, return);

  while(!done) {
    // Clear out response registers

    MMC_clearResponse(mmc);

    // Format and send command
    if (flag == MMC_VOLTAGE_RANGE_ONLY) {
      regs->mmcargh = MMC_STUFF_BITS;
      regs->mmcargl = MMC_STUFF_BITS;
    } 
    else {
      regs->mmcargl = (hVddMask << 8 )& 0x00FF;
      regs->mmcargh = (hVddMask >> 8 )& 0x00FF;
    }
      
    
    regs->mmccmd = MMC_SEND_OP_COND;

   // Wait for Response

     rspDone = FALSE;
     while (!rspDone) {
       MMC_waitForFlag(mmc, MMC_ST0_RSPDNE | MMC_ST0_TOUTRS);
       if (mmc->status & MMC_ST0_TOUTRS) {
         rspDone = TRUE;
       }
     }

     if (!done) {
       VddCard |= regs->mmcrsp.rsp6;
       if ((VddCard & hVddMask) == 0x0000)
         ++noCardsOutOfRange;
   

       if (flag == MMC_VOLTAGE_RANGE_ONLY) {
          va_start(argList,flag);
          VddCards = va_arg(argList, Uint16 *);
          va_end(argList);
          *VddCards |= VddCard;
       }
     } /* If !done */
  } /* while !done */
  
  return noCardsOutOfRange;
}
#endif
  
