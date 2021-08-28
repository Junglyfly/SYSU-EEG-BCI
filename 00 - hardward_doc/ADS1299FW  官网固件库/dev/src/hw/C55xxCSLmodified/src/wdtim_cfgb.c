/******************************************************************************\
*           Copyright (C) 1999 Texas Instruments Incorporated.
*                           All Rights Reserved
*------------------------------------------------------------------------------
* MODULE.NAME... WDTIMER
* FILENAME...... wdtim_cfgb.c
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

/* Definition Romability See timer.h */


             
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
/* DSP/BIOS symbols that CSL needs to check to see which if any timer devices  */
/* are in use by BIOS. These symbols will be defined in the BIOS linker        */
/* command file generated by gconf.                                           */
/* See C6xx */
             
/****************************************\
* WDTIMER global function definitions
\****************************************/

#pragma CODE_SECTION(WDTIM_configArgs,".text:WDTIM_configArgs")

/*----------------------------------------------------------------------------*/
void WDTIM_configArgs(Uint16 wdtcr, Uint16 wdtcr2, Uint16 wdprd) {
  int old_intm;
  Uint16 tcr2_val = wdtcr2 & (_WDTIM_WDTCR2_WDEN_CLR & _WDTIM_WDTCR2_WDKEY_CLR);

   old_intm = IRQ_globalDisable(); 
    
   WDTIM_RSET(WDPRD,wdprd);
   WDTIM_RSET(WDTCR,wdtcr); 
   WDTIM_RSET(WDTCR2,tcr2_val);
  
  IRQ_globalRestore(old_intm);   
}
/*----------------------------------------------------------------------------*/

#endif /* WDTIM_SUPPORT */
/******************************************************************************\
* End of wdtim_cfgb.c
\******************************************************************************/

