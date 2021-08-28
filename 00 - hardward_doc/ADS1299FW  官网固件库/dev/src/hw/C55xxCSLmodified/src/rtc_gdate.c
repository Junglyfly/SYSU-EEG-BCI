/******************************************************************************\
*           Copyright (C) 1999 Texas Instruments Incorporated.
*                           All Rights Reserved
*------------------------------------------------------------------------------
* MODULE.NAME... RTC
* FILENAME...... rtc_gdate.c
* DATE CREATED.. 7/09/01
* PROJECT....... RTC
* COMPONENT..... 
* IMPORTS....... 
*------------------------------------------------------------------------------
* HISTORY:
*   7/09/01 (IM) Created
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
#include <csl_chiphal.h>

#if (_RTC_SUPPORT)

#include <csl_rtc.h>

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

#pragma CODE_SECTION(RTC_getDate,".text:RTC_getDate")

void RTC_getDate(RTC_Date *myDate) {
  int old_intm, old_set;
  old_intm = IRQ_globalDisable();  
  old_set = RTC_FGET(RTCINTEN, SET);

  while (RTC_FGET(RTCPINTR,UIP) != 0);
  _SET_B;		 /* Set bit SET in Register B to 1 */

  myDate->year = RTC_RGET(RTCYEAR);
  myDate->month = RTC_FGET(RTCMONTH,MONTH);
  myDate->daym = RTC_FGET(RTCDAYM,DATE);
  myDate->dayw = RTC_FGET(RTCDAYW,DAY);

  RTC_FSET(RTCINTEN, SET, old_set);

  IRQ_globalRestore(old_intm);   

}

/*----------------------------------------------------------------------------*/

#endif /* RTC_SUPPORT */
/******************************************************************************\
* End of rtc_getDate.c
\******************************************************************************/

