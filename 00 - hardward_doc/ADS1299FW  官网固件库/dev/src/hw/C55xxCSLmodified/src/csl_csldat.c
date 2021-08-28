/******************************************************************************\
*           Copyright (C) 1999 Texas Instruments Incorporated.
*                           All Rights Reserved
*------------------------------------------------------------------------------
* MODULE NAME... CSL - chip support library service layer interface
* FILENAME...... csldata.h
* DATE CREATED.. Fri 06/11/1999 
* PROJECT....... Chip Support Library
* COMPONENT..... HAL
* PREREQUISITS.. 
*------------------------------------------------------------------------------
* HISTORY:
*   CREATED: 02/04/2000 
*   MODIFIED: 06/19/2000  incorporate cpluplus compatability
*   MODIFIED: 06/28/2001  updatd for new CSL initialization model
*   MODIFIED: 10 April 2003 for .csldata size fix
*------------------------------------------------------------------------------
* DESCRIPTION:  (Global/Static Data Table to Support ROMability of CSL modules)
*
*
*
\******************************************************************************/

#include <csl_csldat.h>



#pragma DATA_SECTION(CSL_SysData,".csldata:SYS_data")

CSL_SysDataObj CSL_SysData = CSL_SYSDATAINIT;

/******************************************************************************\
* End of csl_csldat.c
\******************************************************************************/

