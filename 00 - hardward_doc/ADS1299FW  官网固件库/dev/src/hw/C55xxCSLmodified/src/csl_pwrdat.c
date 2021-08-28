/******************************************************************************\
*           Copyright (C) 1999 Texas Instruments Incorporated.
*                           All Rights Reserved
*------------------------------------------------------------------------------
* MODULE NAME... PWR
* FILENAME...... csl_pwrdat.c
* DATE CREATED.. Thu 10 April 2003
* PROJECT....... CSL - Chip Support Library
* COMPONENT..... service layer
* PREREQUISITS.. 
*------------------------------------------------------------------------------
* HISTORY:
*  CREATED:  Thu 10 April 2003
*------------------------------------------------------------------------------
* DESCRIPTION:  (Data file for the PWR module)
\******************************************************************************/
#define _PWR_MOD_

#include <csl_irq.h>
#include <csl_pwr.h>

#if (_PWR_SUPPORT)

#pragma DATA_SECTION(CSL_PwrData,".csldata:PWR_data")

CSL_PwrDataObj CSL_PwrData = CSL_PWRDATAINIT;


#endif /*PWR_SUPPORT */
/******************************************************************************\
* End of csl_pwrdat.c
\******************************************************************************/
