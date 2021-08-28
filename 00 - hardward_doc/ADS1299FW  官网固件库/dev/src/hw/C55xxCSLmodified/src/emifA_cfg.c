/******************************************************************************\
*           Copyright (C) 1999 Texas Instruments Incorporated.
*                           All Rights Reserved
*------------------------------------------------------------------------------
* MODULE.NAME... EMIF
* FILENAME...... emif_cfga.c
* DATE CREATED.. Sun 06/20/1999 
* PROJECT....... CSL - Chip Support Library
* COMPONENT..... 
* IMPORTS....... 
*------------------------------------------------------------------------------
* HISTORY:
*   CREATED:  06/20/1999 
*   MODIFIED: 12/06/2000 Modification for C54x.
*   MODIFIED: 05/18/2000 Modified for C55xx.
*   MODIFIED: 05/16/2001 renamed typedef EMIF_CONFIG -> EMIF_Config
*   MODIFIED: 06/29/2001 added pragma for section per function
*   MODIFIED: 08/14/2001 added code for 5509 memfreq emif workaround
*   MODIFIED: 10/11/2002 removed write to EMIF_emirst register
*   MODIFIED: 11/22/2002 added 5509A to XBSR test
*   MODIFIED: 11/25/2002 added check for SDRAM with MEMFREQ != 1xCPUCLK
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
#include <csl_chiphal.h>

#if (CHIP_5509) || (CHIP_5509A)
    #include <csl_chip.h>
#endif   
 
#if (_EMIFA_SUPPORT)
#include <csl_emif.h> 

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

  int oldgie;
  oldgie = IRQ_globalDisable();

#if ((CHIP_5509) || (CHIP_5509A))
        if ((Config->egcr & _EMIF_EGCR_MEMFREQ_MASK) >= 0x200u)
         CHIP_FSET(XBSR,EMIFX2,1);
#endif
	  EMIF_RSET(EGCR,Config -> egcr);     
        EMIF_RSET(CE01,Config -> ce01); 
        EMIF_RSET(CE02,Config -> ce02);
        EMIF_RSET(CE03,Config -> ce03);
        EMIF_RSET(CE11,Config -> ce11); 
        EMIF_RSET(CE12,Config -> ce12);
        EMIF_RSET(CE13,Config -> ce13);
        EMIF_RSET(CE21,Config -> ce21); 
        EMIF_RSET(CE22,Config -> ce22);
        EMIF_RSET(CE23,Config -> ce23);
        EMIF_RSET(CE31,Config -> ce31); 
        EMIF_RSET(CE32,Config-> ce32);
        EMIF_RSET(CE33,Config -> ce33);
        EMIF_RSET(SDC1,Config -> sdc1);
        EMIF_RSET(SDPER,Config -> sdper);
        EMIF_RSET(INIT,Config -> init);
        EMIF_RSET(SDC2,Config -> sdc2);

#if (CHIP_5509A)
   {
     Uint16 clkMem = Config->egcr & _EMIF_EGCR_MEMFREQ_MASK;
     Uint16 mMask = _EMIF_CE01_MTYPE_MASK;
     Uint16 mSdram = ((Config->ce01 & mMask) != 0) ||
                     ((Config->ce11 & mMask) != 0) ||
                     ((Config->ce21 & mMask) != 0) ||
                     ((Config->ce31 & mMask) != 0);
     if (clkMem && mSdram) {
        _EMIF_IPMR = 0x0000u;
     }
   }
#endif


  IRQ_globalRestore(oldgie);

}
/*----------------------------------------------------------------------------*/

#endif /* EMIF_SUPPORT */
/******************************************************************************\
* End of emif_cfga.c
\******************************************************************************/
