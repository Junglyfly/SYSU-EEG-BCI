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
   
 
#if (_EMIFA_SUPPORT)
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

  int oldgie;

  oldgie = IRQ_globalDisable();

	  Config -> egcr = EMIF_RGET(EGCR);     
        Config -> ce01 = EMIF_RGET(CE01); 
        Config -> ce02 = EMIF_RGET(CE02);
        Config -> ce03 = EMIF_RGET(CE03);
        Config -> ce11 = EMIF_RGET(CE11); 
        Config -> ce12 = EMIF_RGET(CE12);
        Config -> ce13 = EMIF_RGET(CE13);
        Config -> ce21 = EMIF_RGET(CE21); 
        Config -> ce22 = EMIF_RGET(CE22);
        Config -> ce23 = EMIF_RGET(CE23);
        Config -> ce31 = EMIF_RGET(CE31); 
        Config -> ce32 = EMIF_RGET(CE32);
        Config -> ce33 = EMIF_RGET(CE33);
        Config -> sdc1 = EMIF_RGET(SDC1);
        Config -> sdper = EMIF_RGET(SDPER);
        Config -> init = EMIF_RGET(INIT);
        Config -> sdc2 = EMIF_RGET(SDC2);

  IRQ_globalRestore(oldgie);

}
/*----------------------------------------------------------------------------*/

#endif /* EMIF_SUPPORT */
/******************************************************************************\
* End of emif_gcfg.c
\******************************************************************************/
