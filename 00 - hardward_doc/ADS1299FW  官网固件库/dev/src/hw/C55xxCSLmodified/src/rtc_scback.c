/*****************************************************************************\
*           Copyright (C) 2000 Texas Instruments Incorporated.
*                           All Rights Reserved
*------------------------------------------------------------------------------
* MODULE.NAME... RTC
* FILENAME...... RTC_scback.c
* DATE CREATED.. 05/07/2001 
* PROJECT....... Chip Support Library
* COMPONENT..... 
* IMPORTS....... 
*------------------------------------------------------------------------------
* HISTORY:
*   05/03/2001	(IM) Created 
*   Modified	(IM) Removed 2nd and 3rd arguments
*	26/02/2002	(IM) Added manual clearing of interrupt flags in Reg C
*   12/02/2003       Added underscore '_' prefix for private API
*------------------------------------------------------------------------------
* DESCRIPTION:  (body file for the RTC module)
*
*
*
\*****************************************************************************/

/*----------------------------------------------------------------------------*/
/* INCLUDES                                                                   */
/*----------------------------------------------------------------------------*/
#include <csl_chiphal.h>

#if (_RTC_SUPPORT)

#include <csl_irq.h>
#include <csl_rtc.h>

#pragma CODE_SECTION(RTC_setCallback,".text:RTC_setCallback")

/*----------------------------------------------------------------------------*/
/* Function                                                                   */
/*----------------------------------------------------------------------------*/
void RTC_setCallback(RTC_IsrAddr *isrAddr)
{    
	int perTemp, alTemp, updateTemp;
	
	perTemp = (RTC_EVT_PERIODIC) >> 4;	// [4]
	alTemp = (RTC_EVT_ALARM) >> 4;   	// [2]
	updateTemp = (RTC_EVT_UPDATE) >> 4;	// [1]
	    
    (CSL_RTC_DATA).RTC_isrDispatchTable[perTemp] = (Uint32)(isrAddr->periodicAddr); 
    (CSL_RTC_DATA).RTC_isrDispatchTable[alTemp] = (Uint32)(isrAddr->alarmAddr); 
    (CSL_RTC_DATA).RTC_isrDispatchTable[updateTemp] = (Uint32) (isrAddr->updateAddr); 
  
    IRQ_plug(IRQ_EVT_RTC, _RTC_intrDispatch);
     
}

/* Private function */
interrupt void _RTC_intrDispatch(void)
{
      
    asm("\tNOP                       ;====> RTC interrupt routine");   
          
    if (RTC_FGET(RTCINTFL, PF)) {
	    ((void (*)(void))(CSL_RTC_DATA).RTC_isrDispatchTable[(RTC_EVT_PERIODIC) >> 4])();  
	    RTC_FSET(RTCINTFL, PF, 1);	
    }

    if (RTC_FGET(RTCINTFL, AF)) {
	    ((void (*)(void))(CSL_RTC_DATA).RTC_isrDispatchTable[(RTC_EVT_ALARM) >> 4])();	 
	    RTC_FSET(RTCINTFL, AF, 1);	
    }	    
	    
    if (RTC_FGET(RTCINTFL, UF)) {
	    ((void (*)(void))(CSL_RTC_DATA).RTC_isrDispatchTable[(RTC_EVT_UPDATE) >> 4])();	       
	    RTC_FSET(RTCINTFL, UF, 1);	
    }	    
	    
	/* Clear flags in Register C */
	//RTC_RSET(RTCINTFL, 0x70);	
}

#endif /* RTC_SUPPORT */
