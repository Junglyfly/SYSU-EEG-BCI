/*****************************************************************************\
*           Copyright (C) 2000 Texas Instruments Incorporated.
*                           All Rights Reserved
*------------------------------------------------------------------------------
* MODULE.NAME... RTC
* FILENAME...... RTC_enab.c
* DATE CREATED.. 05/03/2001 
* PROJECT....... Chip Support Library
* COMPONENT..... 
* IMPORTS....... 
*------------------------------------------------------------------------------
* HISTORY:
*   05/03/2001   (IM) Created 
*------------------------------------------------------------------------------
* DESCRIPTION:  (body file for the RTC module)
*
*
*
\*****************************************************************************/
#define _RTC_MOD_
/*----------------------------------------------------------------------------*/
/* INCLUDES                                                                   */
/*----------------------------------------------------------------------------*/
#include <csl_chiphal.h>

#if (_RTC_SUPPORT)

#include <csl_rtc.h>

/*----------------------------------------------------------------------------*/
/* Function                                                                   */
/*----------------------------------------------------------------------------*/
void RTC_eventEnable(Uint16 isrMask)
{     Uint16 temp;
	temp = RTC_RGET(RTCINTEN)|(isrMask);
        RTC_RSET(RTCINTEN, temp); 
        IRQ_enable(IRQ_EVT_RTC);
}

#endif /* RTC_SUPPORT */

