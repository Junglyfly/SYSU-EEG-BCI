/******************************************************************************\
*           Copyright (C) 1999 Texas Instruments Incorporated.
*                           All Rights Reserved
*------------------------------------------------------------------------------
* MODULE.NAME... ICACHE
* FILENAME...... icache_init.c
* DATE CREATED.. Tue 05/30/2000 
* PROJECT....... CSL - Chip Support Library
* COMPONENT..... 
* IMPORTS....... 
*------------------------------------------------------------------------------
* HISTORY:
*   05/30/2000 (IC) Created
*   MODIFIED: 10 April 2003 added #pragma CODE_SECTION
*             08/06/2004    Added 5501 support  
*------------------------------------------------------------------------------
* DESCRIPTION:  (body file for the ICACHE module)
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

#if CHIP_5510

#pragma CODE_SECTION(ICACHE_init,".text;ICACHE_init")

 void ICACHE_init(ICACHE_Init *params)    

	
  {
	
	Uint16 r1tag, r2tag;
   /* Set Ramset Mode in GCR register */

	ICACHE_RSET(ICGC, params -> rmode);
	
   /* Initialize Ramset N-Way Control Registers*/
   
   	ICACHE_FSET(ICWC,WINIT, ICACHE_ICWC_WINIT_WINIT);
   	ICACHE_FSET(ICRC1,R1INIT, ICACHE_ICRC1_R1INIT_INIT);
  	ICACHE_FSET(ICRC2,R2INIT,ICACHE_ICRC2_R2INIT_INIT);    	
   /* Enable ICACHE */
	CHIP_FSET(ST3_55,CAEN,1);  

   /* Poll to validate enable */	
    	while(!ICACHE_FGET(ICST, IEN));

   /* Initialize tags according to rmode values */

   if ((params -> rmode) == ICACHE_ICGC_RMODE_1RAMSET)		/* 1 RAMSET */
	{ 	
	
	   r1tag = (Uint16)(((params -> r1addr)>>12)&0x0FFFu);
	   ICACHE_FSET(ICRTAG1, R1TAG, r1tag);
	   while(!ICACHE_FGET(ICRC1, R1TVALID));

  	} 

   if ((params -> rmode) == ICACHE_ICGC_RMODE_2RAMSET)		/* 2 RAMSET */
	{ 
	
	 r1tag = (Uint16)(((params -> r1addr)>>12)&0x0FFFu);
	 r2tag = (Uint16)(((params -> r2addr)>>12)&0x0FFFu);	
	   ICACHE_FSET(ICRTAG1, R1TAG, r1tag);
	   while(!ICACHE_FGET(ICRC1, R1TVALID));
	   ICACHE_FSET(ICRTAG2, R2TAG, r2tag);
	   while(!ICACHE_FGET(ICRC2, R2TVALID));

  	}

   if ((params -> rmode) == ICACHE_ICGC_RMODE_0RAMSET)		/* 0 RAMSET */
      {}


}
/*----------------------------------------------------------------------------*/

#endif /* CHIP_5510 */

#if (CHIP_5502) || (CHIP_5501)


 void ICACHE_setup()    

	
  {

     /* Initialize ICGC register */
	ICACHE_RSET(ICGC, 0xCE3C);

       /* Enable ICACHE */
	CHIP_FSET(ST3_55,CAEN,1); 

  }
#endif /* CHIP_5502 || CHIP_5501 */

#endif /* ICACHE_SUPPORT */
/******************************************************************************\
* End of ICACHE_init.c
\******************************************************************************/

