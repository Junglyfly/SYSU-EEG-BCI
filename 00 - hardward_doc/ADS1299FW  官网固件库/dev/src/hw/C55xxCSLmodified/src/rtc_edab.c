/*****************************************************************************\
*           Copyright (C) 2000 Texas Instruments Incorporated.
*                           All Rights Reserved
*------------------------------------------------------------------------------
* MODULE.NAME... RTC
* FILENAME...... RTC_edab.c
* DATE CREATED.. Sun 05/07/2001 
* PROJECT....... Chip Support Library
* COMPONENT..... 
* IMPORTS....... 
*------------------------------------------------------------------------------
* HISTORY:
*   05/06/2001   (IM) Created 
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
void RTC_eventDisable(Uint16 ierMask)
{     
	Uint16 temp = RTC_RGET(RTCINTEN) & (~ierMask);
	RTC_RSET(RTCINTEN, temp);

        if ( !(RTC_RGET(RTCINTEN) & 0x70) )
            IRQ_disable(IRQ_EVT_RTC);
}
#endif /* RTC_SUPPORT */

