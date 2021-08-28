/****************************************************************************\
*           Copyright (C) 1999-2000 Texas Instruments Incorporated.
*                           All Rights Reserved 
*------------------------------------------------------------------------------
* MODULE NAME... MMC
* FILENAME...... mmc_drrdy.c
* DATE CREATED.. Thu May 24 14:48:09 2001
* PROJECT....... Chip Support Library
* COMPONENT..... Service Layer
* PREREQUISITS.. 
*------------------------------------------------------------------------------
*
* HISTORY:
*	 Created:		Thu May 24 14:48:09 2001 (Automatic generation)
*	 Last Modified:	
*
*------------------------------------------------------------------------------
* DESCRIPTION: Returns TRUE if receive register has new data and is
*              ready to be read. 
\*************************************************************************/
#define _MMC_MOD_
/*************************************************************************\
* Included Header Files                                                  *
\*************************************************************************/

#include <csl_mmc.h>

#if (_MMC_SUPPORT)

#pragma CODE_SECTION(MMC_drrdy,".text:MMC_drrdy")

/**************************************************************************
* MMC_drrdy(MMC_Handle mmc)
*          mmc      - Handle of MMC device reserved by previous call to 
*                     MMC_open()
***************************************************************************/

int MMC_drrdy(MMC_Handle mmc) {

  MMC_ASSERT_HANDLE(mmc,return);  
  return MMC_FGET_H(mmc,MMCST0,DRRDY);
}  
#endif
