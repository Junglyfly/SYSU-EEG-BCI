/******************************************************************************\
*           Copyright (C) 1999 Texas Instruments Incorporated.
*                           All Rights Reserved
*------------------------------------------------------------------------------
* MODULE.NAME... RTC
* FILENAME...... rtc_setTime.c
* DATE CREATED.. 7/09/01
* PROJECT....... RTC
* COMPONENT..... 
* IMPORTS....... 
*------------------------------------------------------------------------------
* HISTORY:
*   7/09/01 (IM) Created
*  22/02/02 (IM) Removed the call to _RESET_B which starts the RTC running
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

#pragma CODE_SECTION(RTC_setTime,".text:RTC_setTime")

void RTC_setTime(RTC_Time *myTime) {
  /* time is set for the default mode of 24-hour */

  int old_intm;
  old_intm = IRQ_globalDisable();  

  _SET_B; 		 /* Set bit SET in Register B to 1 */  

  RTC_FSET(RTCINTEN, TM, 1);
  RTC_RSET(RTCSEC,myTime->second); 
  RTC_RSET(RTCMIN,myTime->minute);
  RTC_FSET(RTCHOUR,HR,myTime->hour);
	
  IRQ_globalRestore(old_intm);   

}

/*----------------------------------------------------------------------------*/

#endif /* RTC_SUPPORT */
/******************************************************************************\
* End of rtc_setTime.c
\******************************************************************************/

