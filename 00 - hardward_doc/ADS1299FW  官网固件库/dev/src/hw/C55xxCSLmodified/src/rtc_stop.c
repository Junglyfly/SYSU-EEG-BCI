/******************************************************************************\
*           Copyright (C) 1999 Texas Instruments Incorporated.
*                           All Rights Reserved
*------------------------------------------------------------------------------
* MODULE NAME... RTC
* FILENAME...... rtc_stop.c
* DATE CREATED.. Tue 27/02/2002 
* PROJECT....... Chip Support Library
* COMPONENT..... service layer
* PREREQUISITS.. cslhal.h
*------------------------------------------------------------------------------
* HISTORY:
*   CREATED:		27/02/2002 (IM).  
*   MODIFIED: 10 April 2003  added #pragma CODE_SECTION	
*------------------------------------------------------------------------------
* DESCRIPTION:  (Stops the RTC)
*
*
*
\******************************************************************************/
#define _RTC_MOD_

#include <csl_rtc.h>
#include <csl.h>


#if (_RTC_SUPPORT)

#pragma CODE_SECTION(RTC_stop,".text:RTC_stop")

/*----------------------------------------------------------------------------*/
void RTC_stop() {
  int oldgie;

  oldgie = IRQ_globalDisable(); 
    
  _SET_B;		 /* Set bit SET in Register B to 1 */

  IRQ_globalRestore(oldgie);

}
#endif /* RTC_SUPPORT */
/******************************************************************************\
* End of rtc_stop.c
\******************************************************************************/

