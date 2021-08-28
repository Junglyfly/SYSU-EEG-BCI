/******************************************************************************\
*           Copyright (C) 1999 Texas Instruments Incorporated.
*                           All Rights Reserved
*------------------------------------------------------------------------------
* MODULE.NAME... EMIF
* FILENAME...... emif_cfga.c
* DATE CREATED.. Tues 02/12/2002 
* PROJECT....... CSL - Chip Support Library
* COMPONENT..... 
* IMPORTS....... 
*------------------------------------------------------------------------------
* HISTORY:
*   CREATED:  02/12/2002 for 5502 EMIF 
*   MODIFIED: 03/27/2003 Changed register names as per SPRU621A
*------------------------------------------------------------------------------
* DESCRIPTION:  (body file for the 5502 EMIF configuration function)
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

#pragma CODE_SECTION(EMIF_config,".text:EMIF_config")

/*----------------------------------------------------------------------------*/
void EMIF_config(EMIF_Config *Config) {
  ioport EMIF_RegObj *regPtr = (CSL_EMIF_DATA).regs;
  int oldgie;

  oldgie = IRQ_globalDisable();

	(regPtr->gblctl1) = Config->gblctl1;
	(regPtr->gblctl2) = Config->gblctl2;
	(regPtr->ce1ctl1) = Config->ce1ctl1;
	(regPtr->ce1ctl2) = Config->ce1ctl2;
	(regPtr->ce0ctl1) = Config->ce0ctl1;
	(regPtr->ce0ctl2) = Config->ce0ctl2;
	(regPtr->ce2ctl1) = Config->ce2ctl1;
	(regPtr->ce2ctl2) = Config->ce2ctl2;
	(regPtr->ce3ctl1) = Config->ce3ctl1;
	(regPtr->ce3ctl2) = Config->ce3ctl2;
	(regPtr->sdctl1)  = Config->sdctl1;
	(regPtr->sdctl2)  = Config->sdctl2;
	(regPtr->sdrfr1)  = Config->sdrfr1;
	(regPtr->sdrfr2)  = Config->sdrfr2;
	(regPtr->sdext1)  = Config->sdext1;
	(regPtr->sdext2)  = Config->sdext2;
	(regPtr->ce1sec1) = Config->ce1sec1;
	(regPtr->ce0sec1) = Config->ce0sec1;
	(regPtr->ce2sec1) = Config->ce2sec1;
	(regPtr->ce3sec1) = Config->ce3sec1;
      (regPtr->cescr)   = Config->cescr;

  IRQ_globalRestore(oldgie);

}
/*----------------------------------------------------------------------------*/

#endif /* EMIF_SUPPORT */
/******************************************************************************\
* End of emif_cfga.c
\******************************************************************************/
