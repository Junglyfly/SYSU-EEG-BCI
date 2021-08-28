/******************************************************************************\
*           Copyright (C) 1999 Texas Instruments Incorporated.
*                           All Rights Reserved
*------------------------------------------------------------------------------
* MODULE.NAME... CSL
* FILENAME...... _csl_init.c
* DATE CREATED.. Sun 06/20/1999 
* PROJECT....... Chip Support Library (CSL)
* COMPONENT..... service layer
* IMPORTS....... csl.h, csl_timer.h
*------------------------------------------------------------------------------
* HISTORY:
*   MODIFIED: 02/21/2000 C54x Romability 
*   MODIFIED: 06/19/2000 modified for DSPBIOS integration 
*   MODIFIED: 07/13/2000 modifed for TMS3205510
*   MODIFIED: 04/16/2001 updated GIO to GPIO references 
*   MODIFIED: 08/15/2002 reinstated CSL_init function in library for
*                        legacy code support
*   MODIFIED: 11/23/2002 re-introduced missing #pragma
*   MODIFIED: 07/28/2004 Added 5501 support  
*------------------------------------------------------------------------------
 
*------------------------------------------------------------------------------
* DESCRIPTION:  (body file for the CSL module)
*
*
*
\******************************************************************************/

#define _CSL_MOD_

#include <csl.h>  

/******************************************************************************\
*                         L O C A L   S E C T I O N
\******************************************************************************/

/****************************************\
* CSL static macro declarations
\****************************************/

/****************************************\
* CSL static typedef declarations
\****************************************/

/****************************************\
* CSL static function declarations
\****************************************/

/****************************************\
* CSL static variable definitions
\****************************************/

/****************************************\
* CSL static function definitions
\****************************************/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/******************************************************************************\
*                        G L O B A L   S E C T I O N
\******************************************************************************/

/****************************************\
* CSL global variable definitions
\****************************************/

/****************************************\
* CSL global function definitions
\****************************************/

#pragma CODE_SECTION(CSL_init,".text:CSL_init")

/*----------------------------------------------------------------------------*/
void CSL_init(void) {

  #if (CHIP_5510)
    CSL5510_LIB_();
  #elif (CHIP_5510PG1_0)
    CSL5510_PG1_LIB_();
  #elif (CHIP_5510PG1_2)
    CSL5510_PG1_2_LIB_();

  #elif (CHIP_5510PG2_0)
    CSL5510_PG2_0_LIB_();
  #elif (CHIP_5510PG2_1)
    CSL5510_PG2_1_LIB_();
  #elif (CHIP_5510PG2_2)
    CSL5510_PG2_2_LIB_();

  #elif (CHIP_5509)
    CSL5509_LIB_();
  #elif (CHIP_5509A)
    CSL5509A_LIB_();

  #elif (CHIP_5502)
    CSL5502_LIB_();
  #elif (CHIP_5501)
    CSL5501_LIB_();
  
  #endif


   _CSL_init(0);     // Call _CSL_init with 0 for operating system
                     // flag


  }  

// Define Library Symbl for compatability check
  #if (CHIP_5510)
    void CSL5510_LIB_(){}
  #elif (CHIP_5510PG1_0)
    void CSL5510_PG1_LIB_(){}
  #elif (CHIP_5510PG1_2)
    void CSL5510_PG1_2_LIB_(){}

  #elif (CHIP_5510PG2_0)
    void CSL5510_PG2_0_LIB_(){}
  #elif (CHIP_5510PG2_1)
    void CSL5510_PG2_1_LIB_(){}
  #elif (CHIP_5510PG2_2)
    void CSL5510_PG2_2_LIB_(){}

  #elif (CHIP_5509)
    void CSL5509_LIB_(){}
  #elif (CHIP_5509A)
    void CSL5509A_LIB_(){}

  #elif (CHIP_5502)
    void CSL5502_LIB_(){}
  #elif (CHIP_5501)
    void CSL5501_LIB_(){}

  #endif
