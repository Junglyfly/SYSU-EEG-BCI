/******************************************************************************\
*           Copyright (C) 1999 Texas Instruments Incorporated.
*                           All Rights Reserved
*------------------------------------------------------------------------------
* MODULE NAME... WDTIMER
* FILENAME...... wdtim_start.c
* DATE CREATED.. Tu 09/11/2001
* PROJECT....... Chip Support Library
* COMPONENT..... service layer
* PREREQUISITS.. cslhal.h
*------------------------------------------------------------------------------
* HISTORY:
*   CREATED:		09/11/2001.  
*   LAST MODIFIED:	(IM) 01/02/2001 Modified for Amadeus
*------------------------------------------------------------------------------
* DESCRIPTION:  (Start sequence for Watch Dog Timer, assumes preconfig)
*
*
*
\******************************************************************************/
#define _WDTIM_MOD_

#include <csl_chiphal.h>

#if (_WDTIMA_SUPPORT)

#include <csl_wdtim.h>

#pragma CODE_SECTION(WDTIM_start,".text:WDTIM_start")

/*----------------------------------------------------------------------------*/
void WDTIM_start() {
 Uint16 old_wdtcr2;
 int old_intm;

  old_intm = IRQ_globalDisable();

  old_wdtcr2 = WDTIM_RGET(WDTCR2);

  WDTIM_FSETS(WDTCR2,WDKEY,PREACTIVE);
  WDTIM_RSET(WDTCR2,
    ((old_wdtcr2 & _WDTIM_WDTCR2_WDKEY_CLR) | WDTIM_WDTCR2_WDEN_ENABLE | WDTIM_WDTCR2_WDKEY_ACTIVE));

  IRQ_globalRestore(old_intm);
}
#endif /* WDTIM_SUPPORT */
/******************************************************************************\
* End of wdtim_start.c
\******************************************************************************/


