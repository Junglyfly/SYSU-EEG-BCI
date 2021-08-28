/****************************************************************************/
/*  RTC_mktime v1.82                                                        */
/*  Copyright (c) 1993-2001  Texas Instruments Incorporated                 */
/****************************************************************************/
#define  _RTC_MOD_          

#include <csl_chiphal.h>

#if (_RTC_SUPPORT)

#include <csl_time.h>
#include <csl_rtc.h>
#include <limits.h>      
#include <stdio.h>
 
 

#define JAN11900    (time_t)1                   
#define DAYS_IN_YR  (time_t)365     
#define SECS_IN_MIN (time_t)60 
#define MINS_IN_HR  (time_t)60 
#define HRS_IN_DAY  (time_t)24 
#define SECS_IN_HR  (SECS_IN_MIN * MINS_IN_HR)
#define SECS_IN_DAY (SECS_IN_HR  * HRS_IN_DAY)
#define SECS_IN_YR  (DAYS_IN_YR  * SECS_IN_DAY)

/* THE FOLLOWING MACRO DETERMINES IF ADDING k TO x CAUSES OVERFLOW */

#define OVERFLOW(x,k) (((x) > 0) == ((k) > 0) &&          \
                       ((unsigned)(x)       <= INT_MAX) != \
		       ((unsigned)((x)+(k)) <= INT_MAX))

#define LEAPYEAR(y)   ( ((y + 1900) % 4 == 0) &&      \
                       (((y + 1900) % 100 != 0) || ((y + 1900) % 400 == 0)) )

#pragma CODE_SECTION(RTC_mktime,".text:RTC_mktime")

_CODE_ACCESS time_t RTC_mktime(register struct RTC_tm *tptr)
{
   time_t   result;
   time_t   daycount;
   int      mdays;
   int      adjust;
    
   /*-----------------------------------------------------------------*/
   /* HANDLE SECONDS.  IF TOO MANY OR TOO FEW, MODIFY MINUTES.        */
   /*-----------------------------------------------------------------*/
   adjust = (tptr->tm_sec < 0) ? -((unsigned)(-tptr->tm_sec + 59) / 60) 
			       : (tptr->tm_sec / 60);

   if (OVERFLOW(tptr->tm_min, adjust)) return((time_t) -1);
   tptr->tm_min += adjust;
   tptr->tm_sec -= adjust * 60;

   /*-----------------------------------------------------------------*/
   /* HANDLE MINUTES.  IF TOO MANY OR TOO FEW, MODIFY HOURS           */
   /*-----------------------------------------------------------------*/
  
   adjust = (tptr->tm_min < 0) ? -((unsigned)(-tptr->tm_min + 59) / 60) 
			       : (tptr->tm_min / 60);

   if (OVERFLOW(tptr->tm_hour, adjust)) return((time_t) -1);
   tptr->tm_hour += adjust;
   tptr->tm_min  -= adjust * 60;
 


   /*-----------------------------------------------------------------*/
   /* HANDLE HOURS.    IF TOO MANY OR TOO FEW, MODIFY DAYS            */
   /*-----------------------------------------------------------------*/
   adjust = (tptr->tm_hour < 0) ? -((unsigned)(-tptr->tm_hour + 23) / 24) 
			        : (tptr->tm_hour / 24);

   if (OVERFLOW(tptr->tm_mday, adjust)) return((time_t) -1);
   tptr->tm_mday += adjust;
   tptr->tm_hour -= adjust * 24;

   for (;;)
   {
      /*-----------------------------------------------------------------*/
      /* HANDLE MONTHS.   IF TOO MANY OR TOO FEW, MODIFY YEARS           */
      /*-----------------------------------------------------------------*/ 

      adjust = (tptr->tm_mon < 0) ? -((unsigned)(-tptr->tm_mon + 11) / 12) 
				  : (tptr->tm_mon / 12);

      if (OVERFLOW(tptr->tm_year, adjust)) return((time_t) -1);
      tptr->tm_year += adjust;            

      tptr->tm_mon  -= adjust * 12;

      /*-----------------------------------------------------------------*/
      /* HANDLE DAYS.     IF TOO MANY OR TOO FEW, MODIFY MONTHS          */
      /*-----------------------------------------------------------------*/
      if (tptr->tm_mday <= 0)
      { 
	   tptr->tm_mon--;  
           tptr->tm_mday += CSL_RTC_DATA.mon_len[tptr->tm_mon < 0 ? 11 : tptr->tm_mon] + 
		        ((tptr->tm_mon == 1) && ((tptr->tm_year % 4) == 0));
	   continue;
      }

      /*-----------------------------------------------------------------*/
      /* CALCULATE NUMBER OF DAYS IN THIS MONTH.                         */
      /*-----------------------------------------------------------------*/
      mdays = CSL_RTC_DATA.mon_len[tptr->tm_mon] + 
		( (tptr->tm_mon == 1) && LEAPYEAR(tptr->tm_year) );

      if (tptr->tm_mday > mdays) 
	{ tptr->tm_mday -= mdays; tptr->tm_mon++; }
      else break;

   }	/* end for */

   /*--------------------------------------------------------------------*/
   /* WE CAN NOW BE SURE THAT ALL FIELDS IN THE TIME STRUCTURE ARE RIGHT */
   /*--------------------------------------------------------------------*/ 

   tptr->tm_yday = CSL_RTC_DATA.mon_day[tptr->tm_mon]  + tptr->tm_mday - 1 +
                   ( (tptr->tm_mon > 1) && LEAPYEAR(tptr->tm_year) );

   daycount      = tptr->tm_year * DAYS_IN_YR  + 
	           ((tptr->tm_year - 1) / 4)   + tptr->tm_yday;

   result        = daycount      * SECS_IN_DAY + 
	           tptr->tm_hour * SECS_IN_HR  +
	           tptr->tm_min  * SECS_IN_MIN + tptr->tm_sec;
   
   tptr->tm_wday  = (JAN11900 + daycount) % 7; 
   tptr->tm_isdst = CSL_RTC_DATA._tz.daylight;
   

   return result;
}
#endif

