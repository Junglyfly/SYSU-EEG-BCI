/******************************************************************************\
*           Copyright (C) 1999 Texas Instruments Incorporated.
*                           All Rights Reserved
*------------------------------------------------------------------------------
* MODULE.NAME... RTC
* FILENAME...... rtc_cfgA.c
* DATE CREATED.. 7/09/01
* PROJECT....... RTC
* COMPONENT..... 
* IMPORTS....... 
*------------------------------------------------------------------------------
* HISTORY:
*   7/09/01 (IM) Created
*
*------------------------------------------------------------------------------
* DESCRIPTION:  (body file for the RTC module)
*
*
*
\******************************************************************************/
#define _RTC_MOD_

/****************************************\
* include files
\****************************************/

#include <csl_rtc.h>

#if (_RTC_SUPPORT)
/******************************************************************************\
*                         L O C A L   S E C T I O N
\******************************************************************************/

/****************************************\
* RTC static macro declarations
\****************************************/

/****************************************\
* RTC static typedef declarations
\****************************************/

/****************************************\
* RTC static function declarations
\****************************************/

/****************************************\
* RTC static variable definitions
\****************************************/

/****************************************\
* RTC static function definitions
\****************************************/

/******************************************************************************\
*                        G L O B A L   S E C T I O N
\******************************************************************************/

/****************************************\
* RTC global variable definitions
\****************************************/

/****************************************\
* RTC global function definitions
\****************************************/

#pragma CODE_SECTION(RTC_configArgs,".text:RTC_configArgs")

void RTC_configArgs(Uint16 rtcsec,Uint16 rtcseca,Uint16 rtcmin,Uint16 rtcmina,Uint16 rtchour,Uint16 rtchoura,Uint16 rtcdayw,
				   Uint16 rtcdaym,Uint16 rtcmonth,Uint16 rtcyear,Uint16 rtcpintr,Uint16 rtcinten)
{
  int old_intm;

   old_intm = IRQ_globalDisable(); 
    
  RTC_RSET(RTCPINTR,rtcpintr);
  _SET_B; 		 /* Set bit SET in Register B to 1 */  

  RTC_RSET(RTCSEC,rtcsec);
  RTC_RSET(RTCSECA,rtcseca);
  RTC_RSET(RTCMIN,rtcmin);
  RTC_RSET(RTCMINA,rtcmina);
  RTC_RSET(RTCHOUR,rtchour);
  RTC_RSET(RTCHOURA,rtchoura);
  RTC_RSET(RTCDAYW,rtcdayw);
  RTC_RSET(RTCDAYM,rtcdaym);
  RTC_RSET(RTCMONTH,rtcmonth);
  RTC_RSET(RTCYEAR,rtcyear);    
  RTC_RSET(RTCINTEN,rtcinten);
  
  IRQ_globalRestore(old_intm);   
}
/*----------------------------------------------------------------------------*/

#endif /* RTC_SUPPORT */
/******************************************************************************\
* End of rtc_cfga.c
\******************************************************************************/

