/******************************************************************************\
*           Copyright (C) 1999 Texas Instruments Incorporated.
*                           All Rights Reserved
*------------------------------------------------------------------------------
* MODULE NAME... RTC
* FILENAME...... csl_rtcdat.c
* DATE CREATED.. Tue 27/02/2002 
* PROJECT....... Chip Support Library
* COMPONENT..... service layer
* PREREQUISITS.. cslhal.h
*------------------------------------------------------------------------------
* HISTORY:
*   CREATED:  10 April 2003   	
*------------------------------------------------------------------------------
* DESCRIPTION:  (Data file for RTC Module)
*
*
*
\******************************************************************************/
#define _RTC_MOD_

#include <csl_rtc.h>
#include <csl.h>


#if (_RTC_SUPPORT)

#pragma DATA_SECTION(CSL_RtcData,".csldata:RTC_data")

CSL_RtcDataObj CSL_RtcData = CSL_RTCDATAINIT;

#endif /* RTC_SUPPORT */
/******************************************************************************\
* End of csl_rtcdat.c
\******************************************************************************/

