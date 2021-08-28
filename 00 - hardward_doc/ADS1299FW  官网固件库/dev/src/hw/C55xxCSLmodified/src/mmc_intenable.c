/****************************************************************************\
*           Copyright (C) 1999-2000 Texas Instruments Incorporated.
*                           All Rights Reserved 
*------------------------------------------------------------------------------
* MODULE NAME... MMC
* FILENAME...... mmc_intenable.c
* DATE CREATED.. Thu Apr 29 14:48:09 2004
* PROJECT....... Chip Support Library
* COMPONENT..... Service Layer
* PREREQUISITS.. 
*------------------------------------------------------------------------------
*
* HISTORY:
*	 Created:		Thu Apr 29 14:48:09 2004
*	 Last Modified:	
*
*------------------------------------------------------------------------------
* DESCRIPTION: Sets the interrupt enable bits of the MMCIE
\*************************************************************************/
#define _MMC_MOD_
/*************************************************************************\
* Included Header Files                                                  *
\*************************************************************************/

#include <csl_mmc.h>

#if (_MMC_SUPPORT)

#pragma CODE_SECTION(MMC_intEnable,".text:MMC_intEnable")

/**************************************************************************
* MMC_intenable(MMC_Handle mmc, Uint16 enableMask)
*      mmc - Handle for MMC Device returned from call to MMC_open
*      enableMask - Sets the interrupt enable bits in the MMCIE
***************************************************************************/

 void MMC_intEnable(MMC_Handle mmc, Uint16 enableMask) {
      ioport MMC_MmcRegObj *regs = mmc->mmcRegs;
      Uint16 event;
      regs->mmcie = enableMask;
      event = mmc->eventId;
      IRQ_clear(event);
      IRQ_plug(event, (IRQ_IsrPtr)(mmc->mmcDispatch));
      IRQ_enable(event);
}

#endif
 
