/******************************************************************************\
*           Copyright (C) 1999 Texas Instruments Incorporated.
*                           All Rights Reserved
*------------------------------------------------------------------------------
* MODULE.NAME... EMIF
* FILENAME...... emif_gcfg.c
* DATE CREATED.. Thu  05/10/2001 
* PROJECT....... Chip Support Library
* COMPONENT..... 
* IMPORTS....... 
*------------------------------------------------------------------------------
* HISTORY:
*   CREATED:  05/10/2001
*   MODIFIED: 05/16/2001 renamed typedef EMIF_CONFIG->EMIF_Config
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

#pragma CODE_SECTION(EMIF_getConfig,".text:EMIF_getConfig")

/*----------------------------------------------------------------------------*/
void EMIF_getConfig(EMIF_Config *Config) {

  ioport EMIF_RegObj *regPtr = (CSL_EMIF_DATA).regs;
  int oldgie;

  oldgie = IRQ_globalDisable();

	Config->gblctl1 = (regPtr->gblctl1);
	Config->gblctl2 = (regPtr->gblctl2);
	Config->ce1ctl1 = (regPtr->ce1ctl1);
	Config->ce1ctl2 = (regPtr->ce1ctl2);
	Config->ce0ctl1 = (regPtr->ce0ctl1);
	Config->ce0ctl2 = (regPtr->ce0ctl2);
	Config->ce2ctl1 = (regPtr->ce2ctl1);
	Config->ce2ctl2 = (regPtr->ce2ctl2);
	Config->ce3ctl1 = (regPtr->ce3ctl1);
	Config->ce3ctl2 = (regPtr->ce3ctl2);
	Config->sdctl1 = (regPtr->sdctl1);
	Config->sdctl2 = (regPtr->sdctl2);
	Config->sdrfr1 = (regPtr->sdrfr1);
	Config->sdrfr2 = (regPtr->sdrfr2);
	Config->sdext1 = (regPtr->sdext1);
	Config->sdext2 = (regPtr->sdext2);
	Config->ce1sec1 = (regPtr->ce1sec1);
	Config->ce0sec1 = (regPtr->ce0sec1);
	Config->ce2sec1 = (regPtr->ce2sec1);
	Config->ce3sec1 = (regPtr->ce3sec1);
      Config->cescr = (regPtr->cescr);

  IRQ_globalRestore(oldgie);

}
/*----------------------------------------------------------------------------*/

#endif /* EMIF_SUPPORT */
/******************************************************************************\
* End of emif_gcfg.c
\******************************************************************************/
