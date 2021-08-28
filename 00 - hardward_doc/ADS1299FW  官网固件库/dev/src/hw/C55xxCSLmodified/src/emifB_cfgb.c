/******************************************************************************\
*           Copyright (C) 1999 Texas Instruments Incorporated.
*                           All Rights Reserved
*------------------------------------------------------------------------------
* MODULE.NAME... EMIF
* FILENAME...... emif_cfgb.c
* DATE CREATED.. Sun 06/20/1999 
* PROJECT....... Chip Support Library
* COMPONENT..... 
* IMPORTS....... 
*------------------------------------------------------------------------------
* HISTORY:
*   CREATED:  06/20/1999 
*   MODIFIED: 12/06/2000 Modification for C54x.
*   MODIFIED: 05/18/2000 Modified for C55xx.
*   MODIFIED: 06/29/2001 added pragma for section per function
*   MODIFIED: 03/27/2003 Updated register names as per SPRU621A
*------------------------------------------------------------------------------
* DESCRIPTION:  (body file for the Cache module)
*
*
*
\******************************************************************************/
#define _EMIF_MOD_

/****************************************\
* include files
\****************************************/    

#include <csl_emif.h> 

#if (CHIP_5509)
  #include <csl_chip.h>
#endif
 
#if (_EMIFB_SUPPORT)
/******************************************************************************\
*                         L O C A L   S E C T I O N
\******************************************************************************/

/****************************************\
* EMIF static macro declarations
\****************************************/
/* See cache.h Romability */
 
 
/****************************************\
* EMIF static typedef declarations
\****************************************/

/****************************************\
* EMIF static function declarations
\****************************************/

/****************************************\
* EMIF Variable definitions : ROMability 
\****************************************/

/* Definition Romability See emif.h */


             
/****************************************\
* EMIF static function definitions
\****************************************/

/*----------------------------------------------------------------------------*/

/******************************************************************************\
*                        G L O B A L   S E C T I O N
\******************************************************************************/

/****************************************\
* EMIF global variable definitions
\****************************************/
/* Definition See emif.h */
/*----------------------------------------------------------------------------*/

             
/****************************************\
* EMIF global function definitions
\****************************************/

#pragma CODE_SECTION(EMIF_configArgs,".text:EMIF_configArgs")

/*----------------------------------------------------------------------------*/


void EMIF_configArgs(Uint16 gblctl1,Uint16 gblctl2,Uint16 ce1ctl1,
                     Uint16 ce1ctl2,Uint16 ce0ctl1,Uint16 ce0ctl2,
                     Uint16 ce2ctl1,Uint16 ce2ctl2,Uint16 ce3ctl1,
                     Uint16 ce3ctl2,Uint16 sdctl1,Uint16 sdctl2,
                     Uint16 sdrfr1,Uint16 sdrfr2,Uint16 sdext1,
                     Uint16 sdext2,Uint16 ce1sec1,Uint16 ce0sec1,
                     Uint16 ce2sec1,Uint16 ce3sec1, Uint16 cescr){

  ioport EMIF_RegObj *regPtr = (CSL_EMIF_DATA).regs;
  int oldgie;

  oldgie = IRQ_globalDisable();

	(regPtr->gblctl1) = gblctl1;
	(regPtr->gblctl2) = gblctl2;
	(regPtr->ce1ctl1) = ce1ctl1;
	(regPtr->ce1ctl2) = ce1ctl2;
	(regPtr->ce0ctl1) = ce0ctl1;
	(regPtr->ce0ctl2) = ce0ctl2;
	(regPtr->ce2ctl1) = ce2ctl1;
	(regPtr->ce2ctl2) = ce2ctl2;
	(regPtr->ce3ctl1) = ce3ctl1;
	(regPtr->ce3ctl2) = ce3ctl2;
	(regPtr->sdctl1)  = sdctl1;
	(regPtr->sdctl2)  = sdctl2;
	(regPtr->sdrfr1)  = sdrfr1;
	(regPtr->sdrfr2)  = sdrfr2;
	(regPtr->sdext1)  = sdext1;
	(regPtr->sdext2)  = sdext2;
	(regPtr->ce1sec1) = ce1sec1;
	(regPtr->ce0sec1) = ce0sec1;
	(regPtr->ce2sec1) = ce2sec1;
	(regPtr->ce3sec1) = ce3sec1;
      (regPtr->cescr)   = cescr;

  IRQ_globalRestore(oldgie);

}
/*----------------------------------------------------------------------------*/

#endif /* EMIF_SUPPORT */
/******************************************************************************\
* End of emif_cfgb.c
\******************************************************************************/
