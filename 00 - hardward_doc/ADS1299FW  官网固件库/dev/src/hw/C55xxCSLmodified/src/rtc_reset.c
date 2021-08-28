/******************************************************************************\
*           Copyright (C) 1999 Texas Instruments Incorporated.
*                           All Rights Reserved
*------------------------------------------------------------------------------
* MODULE NAME... RTC
* FILENAME...... rtc_reset.c
* DATE CREATED.. Tue 27/02/2002 
* PROJECT....... Chip Support Library
* COMPONENT..... service layer
* PREREQUISITS.. cslhal.h
*------------------------------------------------------------------------------
* HISTORY:
*   CREATED:		27/02/2002 (IM).  
*   LAST MODIFIED:	10 April 2003  added #pragam CODE_SECTION
*------------------------------------------------------------------------------
* DESCRIPTION:  (Resets the RTC registers to their default (power-on) values)
*
*
*
\******************************************************************************/
#define _RTC_MOD_

#include <csl_rtc.h>
#include <csl.h>


#if (_RTC_SUPPORT)

#pragma CODE_SECTION(RTC_reset,".text:RTC_reset")

/*----------------------------------------------------------------------------*/
void RTC_reset() {
  int oldgie;
    
  RTC_Config myConfig = {
     0x0,    /* Seconds */
     0x0,    /* Seconds Alarm */
     0x0,    /* Minutes */
     0x0,    /* Minutes Alarm */
     0x0,    /* Hour */
     0x0,    /* Hours Alarm */
     0x0,    /* Day of the week and day alarm */
     0x0,    /* Day of the month */
     0x0,    /* Month */
     0x0,    /* Year */
     0x0,    /* Register A - RTCPINTR */
     0x80,   /*Register B - RTCINTEN */
  };
  oldgie = IRQ_globalDisable(); 

  RTC_config(&myConfig); 

  IRQ_globalRestore(oldgie);

}
#endif /* RTC_SUPPORT */
/******************************************************************************\
* End of rtc_reset.c
\******************************************************************************/


