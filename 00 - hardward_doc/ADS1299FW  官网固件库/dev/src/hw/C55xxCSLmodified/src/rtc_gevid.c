/******************************************************************************\
*           Copyright (C) 1999 Texas Instruments Incorporated.
*                           All Rights Reserved
*------------------------------------------------------------------------------
* MODULE.NAME... RTC
* FILENAME...... rtc_gevid.c
* DATE CREATED.. Sun 06/20/1999 
* PROJECT....... Chip Support Library
* COMPONENT..... 
* IMPORTS....... 
*------------------------------------------------------------------------------
* HISTORY:
*   CREATED:  08/02/2001
*------------------------------------------------------------------------------
* DESCRIPTION:  (Returns IRQ Event Id for RTC)
*
*
*
\******************************************************************************/
#define _RTC_MOD_

#include <csl_rtc.h>

#if (_RTC_SUPPORT)

#pragma CODE_SECTION(RTC_getEventId,".text:RTC_getEventId")

int RTC_getEventId(void) {

  return IRQ_EVT_RTC;
  
}
#endif
