/****************************************************************************\
*           Copyright (C) 1999-2000 Texas Instruments Incorporated.
*                           All Rights Reserved 
*------------------------------------------------------------------------------
* MODULE NAME... MMC
* FILENAME...... mmc_read.c
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
* DESCRIPTION: Reads a block of data from MMC card
*
\*************************************************************************/
#define _MMC_MOD_
/*************************************************************************\
* Included Header Files                                                  *
\*************************************************************************/

#include <csl_mmc.h>

#if (_MMC_SUPPORT)

#pragma CODE_SECTION(MMC_read,".text:MMC_read")

/**************************************************************************
* MMC_read(MMC_Handle mmc, Uint32 cardAddr, Uint16 *buffer, Uint32 length)
*          mmc     		- Handle of MMC device reserved by previous call to 
*                         MMC_open()
*          carAddr 		- Address on MMC Card where read starts
*          buffer  		- Pointer to buffer where data should be stored
*          no_of_bytes  - number of bytes to transfer.
***************************************************************************/

int MMC_read(MMC_Handle mmc, Uint32 cardAddr, Uint16 *buffer, Uint32 no_of_bytes){
   Uint16 old_intm;
   Uint16 *buf = buffer;
   Uint32 count, length;   
   Uint16 data;
   ioport MMC_MmcRegObj *regs = mmc->mmcRegs;

   MMC_ASSERT_HANDLE(mmc, return);
   old_intm = IRQ_globalDisable();
   MMC_clearResponse(mmc);
   MMC_getStatus(mmc, MMC_ST0_TOUTRD | MMC_ST0_DATDNE);

   regs->mmcnblk = 1;
   regs->mmcblen = no_of_bytes;
   regs->mmcargh = (Uint16)((cardAddr >>  16) & 0xFFFF);
   regs->mmcargl = (Uint16)(cardAddr & 0xFFFF);
   regs->mmccmd  = MMC_READ_SINGLE_BLOCK;
   
   IRQ_globalRestore(old_intm);
                                                              
   while (!MMC_FGET_H(mmc,MMCST0,RSPDNE))
   					;
   
   length = (Uint16)(no_of_bytes/2);
   for (count = 0; count < length; count++) 
   {
      while (!MMC_FGET_H(mmc,MMCST0,DRRDY))
                       ;
      if (MMC_FGET_H(mmc,MMCST0,DATDNE) || MMC_FGET_H(mmc,MMCST0,TOUTRD) || MMC_FGET_H(mmc,MMCST0,CRCRD)) 
         break;
      
      data = regs->mmcdrr;
      buf[count] = data;
   }  
   
   mmc->cards->lastAddrRead = cardAddr + length;
   
   if (count < length)
      return (-1);

   return 0;
}
#endif

