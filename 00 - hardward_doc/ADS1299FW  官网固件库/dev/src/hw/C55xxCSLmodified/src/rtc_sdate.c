/******************************************************************************\
*           Copyright (C) 1999 Texas Instruments Incorporated.
*                           All Rights Reserved
*------------------------------------------------------------------------------
* MODULE.NAME... RTC
* FILENAME...... rtc_setDate.c
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

#pragma CODE_SECTION(RTC_setDate,".text:RTC_setDate")

void RTC_setDate(RTC_Date *myDate) {
  int old_intm;

  old_intm = IRQ_globalDisable();  

  _SET_B; 		 /* Set bit SET in Register B to 1 */  

  RTC_RSET(RTCYEAR,myDate->year);
  RTC_FSET(RTCMONTH,MONTH,myDate->month);
  RTC_FSET(RTCDAYM,DATE,myDate->daym);
  RTC_FSET(RTCDAYW,DAEN, 0);
  RTC_FSET(RTCDAYW,DAY,myDate->dayw);

  IRQ_globalRestore(old_intm);   

}

/*----------------------------------------------------------------------------*/

#endif /* RTC_SUPPORT */
/******************************************************************************\
* End of rtc_setDate.c
\******************************************************************************/

