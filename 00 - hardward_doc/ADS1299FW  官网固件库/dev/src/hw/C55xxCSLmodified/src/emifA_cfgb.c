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
*   MODIFIED: 10/11/2002 removed write to EMIF_emirst register
*   MODIFIED: 11/21/2002 added 5509A to XBSR test
*   MODIFIED: 11/25/2002 added heck for SDRAM & MEMFREQ != 1x CPUCLK
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

#if ((CHIP_5509) || (CHIP_5509A))
  #include <csl_chip.h>
#endif
 
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

#pragma CODE_SECTION(EMIF_configArgs,".text:EMIF_configArgs")

/*----------------------------------------------------------------------------*/


void EMIF_configArgs(Uint16 egcr, Uint16 emirst, Uint16 ce01,\
  Uint16 ce02, Uint16 ce03, Uint16 ce11, Uint16 ce12, Uint16 ce13,\
  Uint16 ce21, Uint16 ce22, Uint16 ce23, Uint16 ce31, Uint16 ce32,\
  Uint16 ce33, Uint16 sdc1, Uint16 sdper, Uint16 init,\
  Uint16 sdc2)

 {

  int oldgie;

  oldgie = IRQ_globalDisable(); 
#if ((CHIP_5509) || (CHIP_5509A))
        if ((egcr & _EMIF_EGCR_MEMFREQ_MASK) >= 0x200u)
         CHIP_FSET(XBSR,EMIFX2,1);
#endif

	  EMIF_RSET(EGCR,egcr);     
        EMIF_RSET(CE01,ce01); 
        EMIF_RSET(CE02,ce02);
        EMIF_RSET(CE03,ce03);
        EMIF_RSET(CE11,ce11); 
        EMIF_RSET(CE12,ce12);
        EMIF_RSET(CE13,ce13);
        EMIF_RSET(CE21,ce21); 
        EMIF_RSET(CE22,ce22);
        EMIF_RSET(CE23,ce23);
        EMIF_RSET(CE31,ce31); 
        EMIF_RSET(CE32,ce32);
        EMIF_RSET(CE33,ce33);
        EMIF_RSET(SDC1,sdc1);
        EMIF_RSET(SDPER,sdper);
        EMIF_RSET(INIT,init);
        EMIF_RSET(SDC2,sdc2);

#if (CHIP_5509A)
   {
     Uint16 clkMem = egcr & _EMIF_EGCR_MEMFREQ_MASK;
     Uint16 mMask = _EMIF_CE01_MTYPE_MASK;
     Uint16 mSdram = ((ce01 & mMask) != 0) ||
                     ((ce11 & mMask) != 0) ||
                     ((ce21 & mMask) != 0) ||
                     ((ce31 & mMask) != 0);
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
* End of emif_cfgb.c
\******************************************************************************/
