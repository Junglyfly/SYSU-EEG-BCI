/****************************************************************************/
/*  RTC_difftime v1.82                                                      */
/*  Copyright (c) 1996-2001 Texas Instruments Incorporated                  */
/****************************************************************************/
#define _RTC_MOD_

#include <csl_chiphal.h>

#if (_RTC_SUPPORT)

#undef _INLINE
#include <csl_time.h>
#include <csl_rtc.h> 

#pragma CODE_SECTION(RTC_difftime,".text:RTC_difftime")

_CODE_ACCESS double RTC_difftime(time_t time1, time_t time0)
{
    /*----------------------------------------------------------------------*/
    /* THE FOLLOWING CONVERSION INSURES THAT ANY POSSIBLE DIFFERENCE IN THE */
    /* RANGE OF UNSIGNED LONG IS PROPERLY REPRESENTED IN THE RESULT.        */
    /*----------------------------------------------------------------------*/
    return ((time1 > time0) ?  (double)(time1 - time0)
	                    : -(double)(time0 - time1));
}
#endif
