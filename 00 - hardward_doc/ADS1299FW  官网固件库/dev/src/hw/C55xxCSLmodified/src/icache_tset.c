/******************************************************************************\
*           Copyright (C) 1999 Texas Instruments Incorporated.
*                           All Rights Reserved
*------------------------------------------------------------------------------
* MODULE.NAME... ICACHE
* FILENAME...... icache_tset.c
* DATE CREATED.. Mon 02/18/2002 
* PROJECT....... 
* COMPONENT..... 
* IMPORTS....... 
*------------------------------------------------------------------------------
* HISTORY:
*   02/18/2002 (IC) Created
*   10 April 2003   added #pragma CODE_SECTION
*------------------------------------------------------------------------------
* DESCRIPTION:  (body file for the CACHE module)
*
*
*
\******************************************************************************/
#define _ICACHE_MOD_

/****************************************\
* include files
\****************************************/
#include "csl_chip.h" 
#include "csl_icache.h"
#include "csl.h"

#if (_ICACHE_SUPPORT)
/******************************************************************************\
*                         L O C A L   S E C T I O N
\******************************************************************************/

/****************************************\
* ICACHE static macro declarations
\****************************************/

/****************************************\
* ICACHE static typedef declarations
\****************************************/

/****************************************\
* ICACHE static function declarations
\****************************************/

/****************************************\
* ICACHE static variable definitions
\****************************************/
 

/****************************************\
* ICACHE static function definitions
\****************************************/

/*----------------------------------------------------------------------------*/

/******************************************************************************\
*                        G L O B A L   S E C T I O N
\******************************************************************************/

/****************************************\
* ICACHE global variable definitions
\****************************************/
  
/****************************************\
* ICACHE global function definitions
\****************************************/


#if (CHIP_5510) | (CHIP_5510PG2_0)

#pragma CODE_SECTION(ICACHE_tagset,".text:ICACHE_tagset")

 void ICACHE_tagset(ICACHE_Tagset *params) {    

	Uint16	rmode;
	Uint16 r1tag, r2tag;
    rmode = ICACHE_RGET(ICGC);

   /* Initialize tags according to rmode values */

   if (rmode == ICACHE_ICGC_RMODE_1RAMSET )		/* 1 RAMSET */
	{ 	
	
	   r1tag = (Uint16)(((params -> r1addr)>>12)&0x0FFFu);
	   ICACHE_FSET(ICRTAG1, R1TAG, r1tag);
	   while(ICACHE_FGET(ICRC1, R1TVALID));

  	} 

   if (rmode == ICACHE_ICGC_RMODE_2RAMSET)		/* 2 RAMSET */
	{ 	
	
	   r1tag = (Uint16)(((params -> r1addr)>>12)&0x0FFFu);
	   r2tag = (Uint16)(((params -> r2addr)>>12)&0x0FFFu);
	   ICACHE_FSET(ICRTAG1, R1TAG, r1tag);
	   while(ICACHE_FGET(ICRC1, R1TVALID));
	   ICACHE_FSET(ICRTAG2, R2TAG, r2tag);
	   while(ICACHE_FGET(ICRC2, R2TVALID));

  	}


}
/*----------------------------------------------------------------------------*/

#endif /* CHIP_5510 */

#endif /* ICACHE_SUPPORT */
/******************************************************************************\
* End of icache_tset.c
\******************************************************************************/

