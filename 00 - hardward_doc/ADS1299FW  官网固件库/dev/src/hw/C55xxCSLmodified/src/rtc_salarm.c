/******************************************************************************\
*           Copyright (C) 1999 Texas Instruments Incorporated.
*                           All Rights Reserved
*------------------------------------------------------------------------------
* MODULE.NAME... RTC
* FILENAME...... rtc_sAlarm.c
* DATE CREATED.. 7/09/01
* PROJECT....... RTC
* COMPONENT..... 
* IMPORTS....... 
*------------------------------------------------------------------------------
* HISTORY:
*   7/09/01 (IM) Created
*  22/02/02 (IM) Removed the call to _RESET_B which starts the RTC running
*		     Removed the call to _SET_AIE which enables the Alarm interrupt
*  27/02/02 (IM) Changed setting of hour alarm to set entire register, not 
*				 just HAR field, to accomodate 'Don't care' condition.
*  10 April 2003 added #pragma CODE_SECTION
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

#pragma CODE_SECTION(RTC_setAlarm,".text:RTC_setAlarm")

void RTC_setAlarm(RTC_Alarm *myAlarm) {
  int old_intm;
  old_intm = IRQ_globalDisable();  

/* Only a 24-hour mode is supported */

  _SET_B; 		 /* Set bit SET in Register B to 1 */  

  /* RTC_FSET(RTCINTEN, TM, 1); */
  RTC_FSET(RTCDAYW,DAEN, 1);
  RTC_FSET(RTCSECA,SAR,myAlarm->alsecond); 
  RTC_FSET(RTCMINA,MAR,myAlarm->alminute);
  //RTC_FSET(RTCHOURA,HAR,myAlarm->alhour);
  RTC_RSET(RTCHOURA,myAlarm->alhour);
  RTC_FSET(RTCDAYW,DAR,myAlarm->aldayw);

  IRQ_globalRestore(old_intm);   

}


/*----------------------------------------------------------------------------*/

#endif /* RTC_SUPPORT */
/******************************************************************************\
* End of rtc_setAlarm.c
\******************************************************************************/

