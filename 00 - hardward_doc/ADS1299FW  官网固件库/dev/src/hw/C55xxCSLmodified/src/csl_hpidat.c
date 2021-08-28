/******************************************************************************\
*           Copyright (C) 1999 Texas Instruments Incorporated.
*                           All Rights Reserved
*------------------------------------------------------------------------------
* MODULE NAME... HPI
* FILENAME...... csl_hpidat.c
* DATE CREATED.. Thu 10 April 2003
* PROJECT....... CSL - Chip Support Library
* COMPONENT..... service layer
* PREREQUISITS.. 
*------------------------------------------------------------------------------
* HISTORY:
*   CREATED:  10 April 2003
*------------------------------------------------------------------------------
* DESCRIPTION:  (Data file for HPI module)
*              
\******************************************************************************/
#define _HPI_MOD_

#include <csl_chiphal.h>

#if (_HPI_SUPPORT)

#include <csl_hpi.h>

#pragma DATA_SECTION(CSL_HpiData,".csldata:HPI_data")

CSL_HpiDataObj  CSL_HpiData = CSL_HPIDATAINIT;

#endif /* HPI_SUPPORT */
/******************************************************************************\
* End of csl_hpidat.c
\******************************************************************************/
