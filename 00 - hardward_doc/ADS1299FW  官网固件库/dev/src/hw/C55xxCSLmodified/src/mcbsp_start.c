/******************************************************************************\
*           Copyright (C) 1999 Texas Instruments Incorporated.
*                           All Rights Reserved
*------------------------------------------------------------------------------
* MODULE NAME... MCBSP
* FILENAME...... MCBSP_start.c
* DATE CREATED.. Fri 06/11/1999 
* PROJECT....... Chip Support Library
* COMPONENT..... 
* PREREQUISITS.. 
*------------------------------------------------------------------------------
* HISTORY:
*   CREATED:  06/11/1999  Created
*   MODIFIED: 11/18/1999  Modified for TMS320C54xx
*   MODIFIED: 02/15/2000  C54x Romability .
*   MODIFIED: 10/17/2000  Modified for TMS320C55xx
*   MODIFIED: 03/23/2001  Modified to use new CSL types to avoid DSPBIOS
*                         definition conflict
*   MODIFIED: 06/28/2001  added pragma for section per function
*   MODIFIED: 09/20/2004  Fixed MCBSP_start() issue
*------------------------------------------------------------------------------
* DESCRIPTION:  (interface file for the MCBSP module)
*
*
*
\******************************************************************************/
#define _MCBSP_MOD_

#include <csl_mcbsp.h> 
  
#if (_MCBSP_SUPPORT)

#pragma CODE_SECTION(MCBSP_start,".text:MCBSP_start")

/*----------------------------------------------------------------------------*/

CSLBool MCBSP_start(MCBSP_Handle hMcbsp, Uint16 StartMask, Uint32
SampleRateDelay) {

  /* compiler should not optimize delay loop under -o3 switch */
  volatile Uint32 count = SampleRateDelay;

  MCBSP_ASSERT_HANDLE(hMcbsp,return FALSE);
 
  if (StartMask & MCBSP_SRGR_START) {
    MCBSP_FSETH(hMcbsp,SPCR2,GRST,1);
    while (count > 0)  {
           --count;
     }
  }

  if (StartMask & MCBSP_RCV_START) {
    MCBSP_FSETH(hMcbsp,SPCR1,RRST,1);
  }

  if (StartMask & MCBSP_XMIT_START) {
    MCBSP_FSETH(hMcbsp,SPCR2,XRST,1);
  }
  
 if (StartMask & MCBSP_SRGR_FRAMESYNC) {
    MCBSP_FSETH(hMcbsp,SPCR2,FRST,1);
  }
 return TRUE;
}

#endif /* MCBSP_SUPPORT */
/******************************************************************************\
* End of MCBSP_start.c
\******************************************************************************/

