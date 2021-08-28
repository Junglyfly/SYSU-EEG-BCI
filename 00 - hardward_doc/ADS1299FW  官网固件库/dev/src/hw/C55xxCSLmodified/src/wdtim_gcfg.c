/******************************************************************************\
*           Copyright (C) 1999 Texas Instruments Incorporated.
*                           All Rights Reserved
*------------------------------------------------------------------------------
* MODULE.NAME... WDTIMER
* FILENAME...... wdtim_gcfg.c
* DATE CREATED.. Fri 07/28/2000 
* PROJECT....... Chip Support Library
* COMPONENT..... 
* IMPORTS....... 
*------------------------------------------------------------------------------
* HISTORY:
*  CREATED:		07/28/2000
*  LAST MODIFIED:	(IM) 01/02/2001 Modified for Amadeus
*------------------------------------------------------------------------------
* DESCRIPTION:  (body file for the WDTIMER module)
*
*
*
\******************************************************************************/
#define _WDTIM_MOD_

/****************************************\
* include files
\****************************************/  
#include <csl_chiphal.h>
 
#if (_WDTIMA_SUPPORT)

#include <csl_wdtim.h> 

/******************************************************************************\
*                         L O C A L   S E C T I O N
\******************************************************************************/

/****************************************\
* WDTIM static macro declarations
\****************************************/
/* See csl_wdtim.h Romability */
 
 
/****************************************\
* WDTIMER static typedef declarations
\****************************************/

/****************************************\
* WDTIMER static function declarations
\****************************************/

/****************************************\
* WDTIMER Variable definitions : ROMability 
\****************************************/

/* Definition Romability See wdtimer.h */


             
/****************************************\
* WDTIMER static function definitions
\****************************************/

/*----------------------------------------------------------------------------*/

/******************************************************************************\
*                        G L O B A L   S E C T I O N
\******************************************************************************/

/****************************************\
* WDTIMER global variable definitions
\****************************************/
/* Definition See csl_wdtim.h */
/*----------------------------------------------------------------------------*/
/* DSP/BIOS symbols that CSL needs to check to see what if any timer devices  */
/* are in use by BIOS. These symbols will be defined in the BIOS linker        */
/* command file generated by gconf.                                           */
/* See C6xx */
             
/****************************************\
* WDTIMER global function definitions
\****************************************/

#pragma CODE_SECTION(WDTIM_getConfig,".text:WDTIM_getConfig")

/*----------------------------------------------------------------------------*/
void WDTIM_getConfig(WDTIM_Config *Config) {
  int old_intm;
  old_intm = IRQ_globalDisable();  
  Config->wdtcr2 = WDTIM_RGET(WDTCR2);
  Config->wdprd = WDTIM_RGET(WDPRD);
  Config->wdtcr = WDTIM_RGET(WDTCR);   
  IRQ_globalRestore(old_intm);   
}
/*----------------------------------------------------------------------------*/

#endif /* WDTIM_SUPPORT */
/******************************************************************************\
* End of wdtim_gcfg.c
\******************************************************************************/

