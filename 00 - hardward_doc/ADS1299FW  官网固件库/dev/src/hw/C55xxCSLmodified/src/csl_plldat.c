/******************************************************************************\
*           Copyright (C) 1999 Texas Instruments Incorporated.
*                           All Rights Reserved
*------------------------------------------------------------------------------
* MODULE NAME... PLL
* FILENAME...... csl_plldat.c
* DATE CREATED.. Thu 10 April 2003
* PROJECT....... CSL - Chip Support Library
* COMPONENT..... service layer
* PREREQUISITS.. 
*------------------------------------------------------------------------------
* HISTORY:
*   CREATED:  10 April 2003
*------------------------------------------------------------------------------
* DESCRIPTION:  (Data file for the PLL module)
\******************************************************************************/
#define _PLL_MOD_

#include <csl_emif.h>
#include <csl_pll.h>

#if (_PLLA_SUPPORT)

#pragma DATA_SECTION(CSL_PllData,".csldata:PLL_data")

CSL_PllDataObj  CSL_PllData = CSL_PLLDATAINIT;


#endif    //PLLB_SUPPORT



