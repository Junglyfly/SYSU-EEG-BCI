/****************************************************************************/
/*  RTC_gmtime v1.82                                                        */
/*  Copyright (c) 1993-2001  Texas Instruments Incorporated                 */
/****************************************************************************/
#define _RTC_MOD_

#if (_RTC_SUPPORT)

#include <csl_time.h>
#include <csl_rtc.h>


#pragma CODE_SECTION(RTC_gmtime,".text:RTC_gmtime")

_CODE_ACCESS struct tm *RTC_gmtime(const time_t *timer)
{
    time_t gtime = CSL_RTC_DATA._tz.timezone; /* DIFFERENCE BETWEEN CURRENT TIME ZONE    */
				 /* AND GMT IN SECONDS                      */

    if (timer) gtime += *timer;
    return (RTC_localtime(&gtime));
}
#endif

