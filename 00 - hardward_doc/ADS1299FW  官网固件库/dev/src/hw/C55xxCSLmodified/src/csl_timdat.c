/******************************************************************************\
*           Copyright (C) 1999 Texas Instruments Incorporated.
*                           All Rights Reserved
*------------------------------------------------------------------------------
* MODULE.NAME... TIMER
* FILENAME...... tim_cfg.c
* DATE CREATED.. Thu April 10 2003 
* PROJECT....... CSL - Chip Support Library
* COMPONENT..... 
* IMPORTS....... 
*------------------------------------------------------------------------------
* HISTORY:
*   CREATED:  10 April 2003 
*------------------------------------------------------------------------------
* DESCRIPTION:  (Data file for TIMER module)
*
*
*
\******************************************************************************/
#define _TIMER_MOD_

/****************************************\
* include files
\****************************************/    


#include <csl_chiphal.h>  
 
#if (_TIMER_SUPPORT)
#include <csl_timer.h>
/******************************************************************************\
*                         L O C A L   S E C T I O N
\******************************************************************************/

/****************************************\
* TIMER static macro declarations
\****************************************/
/* See timer.h Romability */
 
/****************************************\
* TIMER static typedef declarations
\****************************************/

/****************************************\
* TIMER static function declarations
\****************************************/

/****************************************\
* TIMER Variable definitions : ROMability 
\****************************************/

/* Definition Romability See timer.h */


             
/****************************************\
* TIMER static function definitions
\****************************************/

/*----------------------------------------------------------------------------*/

/******************************************************************************\
*                        G L O B A L   S E C T I O N
\******************************************************************************/

/****************************************\
* TIMER global variable definitions
\****************************************/
/* Definition See timer.h */
/*----------------------------------------------------------------------------*/
/* DSP/BIOS symbols that CSL needs to check to see what if any timer devices  */
/* are in use by BIOS. These symbols will e defined in the BIOS linker        */
/* command file generated by gconf.                                           */
/* See C6xx */
             
/****************************************\
* TIMER global function definitions
\****************************************/


#pragma DATA_SECTION(CSL_TimerData,".csldata:TIMER_data")

 CSL_TimerDataObj CSL_TimerData = CSL_TIMERDATAINIT;

/*----------------------------------------------------------------------------*/

#endif /* TIMER_SUPPORT */
/******************************************************************************\
* End of csl_timdat.c
\******************************************************************************/

