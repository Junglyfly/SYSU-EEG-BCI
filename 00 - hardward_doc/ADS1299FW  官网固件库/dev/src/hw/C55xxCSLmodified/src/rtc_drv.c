#define  _RTC_MOD_    

#include <csl_chiphal.h>

#if (_RTC_SUPPORT)

#include <stdio.h>
#include <string.h>
#include <csl_time.h>  
#include <csl_rtc.h>


/****************************************************************************/
/* Functions:                                                               */
/*    RTCTIME()    -    Get the time from RTC                               */
/*                                                                          */
/****************************************************************************/

 /* extern _CODE_ACCESS time_t RTCtime(void);      */
  
#pragma CODE_SECTION(RTCtime,".text:RTCtime")

/****************************************************************************/
/* RTCTIME()  -  Get the time from RTC                                      */
/****************************************************************************/
_CODE_ACCESS time_t RTCtime(void)
{                     

_DATA_ACCESS struct RTC_tm temp;
   time_t rtctime;   
 
   
   temp.tm_sec = RTC_bcdToDec(RTC_RGET(RTCSEC));
   temp.tm_min = RTC_bcdToDec(RTC_RGET(RTCMIN));     
   temp.tm_hour = RTC_bcdToDec(RTC_FGET(RTCHOUR,HR));  
   temp.tm_mday = RTC_bcdToDec(RTC_RGET(RTCDAYM)); 
   temp.tm_mon = RTC_bcdToDec(RTC_RGET(RTCMONTH)) - 1;    //RTC month starts at 1     
   temp.tm_year = RTC_bcdToDec(RTC_RGET(RTCYEAR)) + 100;   //RTC starts at 2000   
    
   rtctime = RTC_mktime(&temp);                
 
   return rtctime;         

}
#endif
