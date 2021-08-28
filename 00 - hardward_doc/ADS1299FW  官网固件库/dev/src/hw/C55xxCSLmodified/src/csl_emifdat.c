/******************************************************************************\
*           Copyright (C) 1999 Texas Instruments Incorporated.
*                           All Rights Reserved
*------------------------------------------------------------------------------
* MODULE.NAME... EMIF
* FILENAME...... csl_emifdat.c
* DATE CREATED.. Thu 10 April 2003
* PROJECT....... CSL - Chip Support Library
* COMPONENT..... 
* IMPORTS....... 
*------------------------------------------------------------------------------
* HISTORY:
*   CREATED:  10 April 2003
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

#if _EMIF_SUPPORT

   #pragma DATA_SECTION(CSL_EmifData,".csldata:EMIF_data")

   CSL_EmifDataObj CSL_EmifData = CSL_EMIFDATAINIT;


#endif /* EMIF_SUPPORT */
/******************************************************************************\
* End of csl_emifdat.c
\******************************************************************************/
