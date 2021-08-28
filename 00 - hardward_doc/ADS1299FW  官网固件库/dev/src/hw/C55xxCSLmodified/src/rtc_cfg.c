/******************************************************************************\
*           Copyright (C) 1999 Texas Instruments Incorporated.
*                           All Rights Reserved
*------------------------------------------------------------------------------
* MODULE.NAME... RTC
* FILENAME...... rtc_cfg.c
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

#pragma CODE_SECTION(RTC_config,".text:RTC_config")

void RTC_config(RTC_Config *Config) {
  int old_intm;

  old_intm = IRQ_globalDisable();  

  RTC_RSET(RTCINTFL,0x70);			/* Clear flags in Register C */
  RTC_RSET(RTCINTEN,Config->rtcinten);	/* Set Register B - 12 or 24-hour mode */
  _SET_B;			/* Set bit SET in Register B to 1 */  

/* Set Data registers */
  RTC_RSET(RTCSEC,Config->rtcsec);
  RTC_RSET(RTCSECA,Config->rtcseca);
  RTC_RSET(RTCMIN,Config->rtcmin);
  RTC_RSET(RTCMINA,Config->rtcmina);
  RTC_RSET(RTCHOUR,Config->rtchour);
  RTC_RSET(RTCHOURA,Config->rtchoura);
  RTC_RSET(RTCDAYW,Config->rtcdayw);
  RTC_RSET(RTCDAYM,Config->rtcdaym);
  RTC_RSET(RTCMONTH,Config->rtcmonth);
  RTC_RSET(RTCYEAR,Config->rtcyear);    
  
  RTC_RSET(RTCPINTR,Config->rtcpintr);	/* Set Register A */

  IRQ_globalRestore(old_intm);   

}

/*----------------------------------------------------------------------------*/

#endif /* RTC_SUPPORT */
/******************************************************************************\
* End of rtc_cfg.c
\******************************************************************************/

