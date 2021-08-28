 /****************************************************************************/
/*  asctime v1.82                                                          */
/*  Copyright (c) 1993-2001  Texas Instruments Incorporated                 */
/****************************************************************************/
#define _RTC_MOD_

#if (_RTC_SUPPORT)

#include <csl_chiphal.h>
#include <csl_time.h> 
#include <csl_rtc.h>

extern _CODE_ACCESS int sprintf(char *_string, const char *_format, ...);

#pragma CODE_SECTION(RTC_asctime,".text:RTC_asctime")

_CODE_ACCESS char *RTC_asctime(const struct tm *timeptr)
{
    struct tm   tptr = *timeptr;
    static _DATA_ACCESS char result[26];

    RTC_mktime(&tptr);      /* MAKE SURE THIS A SANE TIME */

    sprintf(result, "%s %s%3d %02d:%02d:%02d %d\n",
		    CSL_RTC_DATA.day[tptr.tm_wday], 
                CSL_RTC_DATA.mon[tptr.tm_mon], 
                tptr.tm_mday, 
		    tptr.tm_hour, 
                tptr.tm_min, 
                tptr.tm_sec,
		    tptr.tm_year + 1900);

    return (result);
}
#endif
