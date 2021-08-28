/******************************************************************************\
*           Copyright (C) 1999 Texas Instruments Incorporated.
*                           All Rights Reserved
*------------------------------------------------------------------------------
* MODULE NAME... WDTIMER
* FILENAME...... csl_wdtimdat.c
* DATE CREATED.. Thu 10 April 2003
* PROJECT....... Chip Support Library
* COMPONENT..... service layer
* PREREQUISITS.. cslhal.h
*------------------------------------------------------------------------------
* HISTORY:
*   CREATED: 10 April 2003
*------------------------------------------------------------------------------
* DESCRIPTION:  (Data file for Watch Dog Timer, assumes preconfig)
*
*
*
\******************************************************************************/
#define _WDTIM_MOD_

#include <csl_chiphal.h>

#if _WDTIM_SUPPORT

#include <csl_wdtim.h>
#include <csl_wdtimdat.h>

#pragma DATA_SECTION(CSL_WdtimData,".csldata:WDTIM_data")

CSL_WdtimDataObj  CSL_WdtimData = CSL_WDTIMDATAINIT;

/*----------------------------------------------------------------------------*/
#endif /* WDTIM_SUPPORT */
/******************************************************************************\
* End of csl_wdtimdat.c
\******************************************************************************/


