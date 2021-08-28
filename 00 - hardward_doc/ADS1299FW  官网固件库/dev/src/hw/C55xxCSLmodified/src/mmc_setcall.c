/****************************************************************************\
*           Copyright (C) 1999-2000 Texas Instruments Incorporated.
*                           All Rights Reserved 
*------------------------------------------------------------------------------
* MODULE NAME... MMC
* FILENAME...... mmc_setcall.c
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
* DESCRIPTION: Halts current data transfer.
\*************************************************************************/
#define _MMC_MOD_
/*************************************************************************\
* Included Header Files                                                  *
\*************************************************************************/

#include <csl_mmc.h>

#if (_MMC_SUPPORT)

#pragma CODE_SECTION(MMC_setCallBack,".text:MMC_setCallBack")

/**************************************************************************
* MMC_setCallBack(MMC_Handle mmc, MMC_CallBackObj *callBackFuncs)
*      mmc - Handle for MMC Device returned from call to MMC_open
*      callBackFuncs - pointer to MMC_CallBackObj containing predefined
*                      set of functions to call to service flagged 
*                      MMC interrupts       
***************************************************************************/

 void MMC_setCallBack(MMC_Handle mmc, MMC_CallBackObj *callBackFuncs) {
   
   mmc->callBackTbl->isr[0]  = callBackFuncs->isr[0];
   mmc->callBackTbl->isr[1]  = callBackFuncs->isr[1];
   mmc->callBackTbl->isr[2]  = callBackFuncs->isr[2];
   mmc->callBackTbl->isr[3]  = callBackFuncs->isr[3];
   mmc->callBackTbl->isr[4]  = callBackFuncs->isr[4];
   mmc->callBackTbl->isr[5]  = callBackFuncs->isr[5];
   mmc->callBackTbl->isr[6]  = callBackFuncs->isr[6];
   mmc->callBackTbl->isr[7]  = callBackFuncs->isr[7];
   mmc->callBackTbl->isr[8]  = callBackFuncs->isr[8];
   mmc->callBackTbl->isr[9]  = callBackFuncs->isr[9];
   mmc->callBackTbl->isr[10] = callBackFuncs->isr[10];
   mmc->callBackTbl->isr[11] = callBackFuncs->isr[11];

}

#endif
 
