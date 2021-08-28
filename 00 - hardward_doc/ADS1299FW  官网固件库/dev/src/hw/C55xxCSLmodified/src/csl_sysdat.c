/******************************************************************************\
*           Copyright (C) 1999 Texas Instruments Incorporated.
*                           All Rights Reserved
*------------------------------------------------------------------------------
* MODULE.NAME... CSL
* FILENAME...... csl_sysdat.c
* DATE CREATED.. Thu April 10 2003
* PROJECT....... Chip Support Library (CSL)
* COMPONENT..... service layer
* IMPORTS....... csl.h, csl_timer.h
*------------------------------------------------------------------------------
* HISTORY:
*  CREATED: 10 April 2003
*------------------------------------------------------------------------------
* DESCRIPTION:  (body file for the CSL module)
*
*
*
\******************************************************************************/

#include <csl_std.h>  
#include <csl_csldat.h>


/******************************************************************************\
*                         L O C A L   S E C T I O N
\******************************************************************************/

/****************************************\
* CSL static variable definitions
\****************************************/



#pragma DATA_SECTION(CSL_SysData,".csldat:SYS_data")

CSL_SysDataObj CSL_SysData = CSL_SYSDATAINIT;
