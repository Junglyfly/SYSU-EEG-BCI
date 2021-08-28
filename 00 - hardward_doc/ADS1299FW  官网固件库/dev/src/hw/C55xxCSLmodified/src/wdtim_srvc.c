/******************************************************************************\
*           Copyright (C) 1999 Texas Instruments Incorporated.
*                           All Rights Reserved
*------------------------------------------------------------------------------
* MODULE NAME... WDTIMER
* FILENAME...... wdtim_srvc.c
* DATE CREATED.. Fri 07/28/2000 
* PROJECT....... Chip Support Library
* COMPONENT..... service layer
* PREREQUISITS.. cslhal.h
*------------------------------------------------------------------------------
* HISTORY:
*   CREATED:		07/28/2000.  
*   MODIFIED:	(IM) 01/02/2001 Modified for Amadeus
*   MODIFIED: 08/13/2002 fixed bit-field access problem
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

#pragma CODE_SECTION(WDTIM_service,".text:WDTIM_service")

/*----------------------------------------------------------------------------*/
void WDTIM_service() {
 Uint16 old_wdtcr2;
 int old_intm;

  old_intm = IRQ_globalDisable();

  old_wdtcr2 = WDTIM_RGET(WDTCR2);

  WDTIM_FSETS(WDTCR2,WDKEY,PREACTIVE);
  WDTIM_RSET(WDTCR2,
    ((old_wdtcr2 & _WDTIM_WDTCR2_WDKEY_CLR) | _WDTIM_WDTCR2_WDEN_MK(WDTIM_WDTCR2_WDEN_ENABLE) | 
      _WDTIM_WDTCR2_WDKEY_MK(WDTIM_WDTCR2_WDKEY_ACTIVE)));

  IRQ_globalRestore(old_intm);
}
#endif /* WDTIM_SUPPORT */
/******************************************************************************\
* End of wdtim_start.c
\******************************************************************************/

