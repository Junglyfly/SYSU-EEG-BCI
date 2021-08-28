/*****************************************************************************/
/*  TIME.C v1.82                                                            */
/*  Copyright (c) 1995-2001 Texas Instruments Incorporated                   */
/*****************************************************************************/
#define  _RTC_MOD_    

#include <csl_chiphal.h>

#if (_RTC_SUPPORT)

#include <csl_time.h> 
#include <csl_rtc.h> 
#include <stdio.h>

extern _CODE_ACCESS time_t RTCtime();

#pragma CODE_SECTION(RTC_time,".text:RTC_time")

_CODE_ACCESS time_t RTC_time(time_t *timer) 
{
   time_t result;

   result = (time_t)RTCtime();   
   if(timer) *timer = result;   

   return(result);
}
#endif

