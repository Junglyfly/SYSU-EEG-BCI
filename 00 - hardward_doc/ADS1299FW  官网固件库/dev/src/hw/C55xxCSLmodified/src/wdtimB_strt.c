/******************************************************************************\
*           Copyright (C) 1999 Texas Instruments Incorporated.
*                           All Rights Reserved
*------------------------------------------------------------------------------
* MODULE NAME... WDTIMER
* FILENAME...... wdtim_strt.c
* DATE CREATED.. Fri 07/28/2000 
* PROJECT....... Chip Support Library
* COMPONENT..... service layer
*------------------------------------------------------------------------------
* HISTORY:
*   CREATED:		07/28/2000.  
*   LAST MODIFIED:	10/28/2003 Corrected the sequence to start watchdog timer
*					01/02/2001 Modified for Amadeus
*------------------------------------------------------------------------------
* DESCRIPTION:  (Start sequence for Watch Dog Timer, assumes preconfig)
*
*
*
\******************************************************************************/
#define _WDTIM_MOD_

#include <csl_chiphal.h>

#if (_WDTIMB_SUPPORT)

#include <csl_wdtim.h>

#pragma CODE_SECTION(WDTIM_wdStart,".text:WDTIM_wdStart")

/*----------------------------------------------------------------------------*/
void WDTIM_wdStart(WDTIM_Handle hWdt) {
 int old_intm;
  ioport WDTIM_RegObj *regPtr =((CSL_WdtimDataObj *)(hWdt))->regs;

  old_intm = IRQ_globalDisable();

  regPtr->wdtwctl1 |= _WDTIM_WDTWCTL1_WDEN_MK(1);
  regPtr->wdtwctl2 = WDTIM_WDTWCTL2_WDKEY_PREACTIVE;
  regPtr->wdtwctl2 = WDTIM_WDTWCTL2_WDKEY_ACTIVE;  
  IRQ_globalRestore(old_intm);
}
#endif /* _WDTIMB_SUPPORT */
/******************************************************************************\
* End of wdtimB_strt.c
\******************************************************************************/

