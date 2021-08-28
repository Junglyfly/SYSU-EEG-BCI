/******************************************************************************\
*           Copyright (C) 1999 Texas Instruments Incorporated.
*                           All Rights Reserved
*------------------------------------------------------------------------------
* MODULE.NAME... ICACHE
* FILENAME...... csl_icachedat.c
* DATE CREATED.. Thu 10 April 2003
* PROJECT....... CSL - Chip Support Library
* COMPONENT..... 
* IMPORTS....... 
*------------------------------------------------------------------------------
* HISTORY:
*   10 April 2003   added reference to CLS_IcacheData for .csldata size fix
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
             
/****************************************\
* ICACHE static function definitions
\****************************************/

/*----------------------------------------------------------------------------*/

#pragma DATA_SECTION(CSL_IcacheData,".csldata:ICACHE_data")

CSL_IcacheDataObj   CSL_IcacheData = CSL_ICACHEDATAINIT;

/*----------------------------------------------------------------------------*/

#endif /* ICACHE_SUPPORT */
/******************************************************************************\
* End of csl_icachedat.c
\******************************************************************************/

