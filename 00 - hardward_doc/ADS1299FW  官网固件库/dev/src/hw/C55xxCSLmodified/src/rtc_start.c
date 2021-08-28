/******************************************************************************\
*           Copyright (C) 1999 Texas Instruments Incorporated.
*                           All Rights Reserved
*------------------------------------------------------------------------------
* MODULE NAME... RTC
* FILENAME...... rtc_start.c
* DATE CREATED.. Mo 09/10/2001
* PROJECT....... Chip Support Library
* COMPONENT..... service layer
* PREREQUISITS.. cslhal.h
*------------------------------------------------------------------------------
* HISTORY:
*   CREATED:		09/10/2001 (IM).  
*   LAST MODIFIED:	10 April 2003  added #pragma CODE_SECTION
*------------------------------------------------------------------------------
* DESCRIPTION:  (Starts the RTC)
*
*
*
\******************************************************************************/
#define _RTC_MOD_

#include <csl_rtc.h>
#include <csl.h>


#if (_RTC_SUPPORT)

#pragma CODE_SECTION(RTC_start,".text:RTC_start")

/*----------------------------------------------------------------------------*/
void RTC_start() {
  int oldgie;

  oldgie = IRQ_globalDisable(); 
    
  _RESET_B;		 /* Set bit SET in Register B to 0 */

  IRQ_globalRestore(oldgie);

}
#endif /* RTC_SUPPORT */
/******************************************************************************\
* End of rtc_start.c
\******************************************************************************/

