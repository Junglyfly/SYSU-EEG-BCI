/****************************************************************************\
*           Copyright (C) 1999-2000 Texas Instruments Incorporated.
*                           All Rights Reserved 
*------------------------------------------------------------------------------
* MODULE NAME... MMC
* FILENAME...... mmc_write.c
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
* DESCRIPTION: Writes a single block of data to MMC card
*
\*************************************************************************/
#define _MMC_MOD_
/*************************************************************************\
* Included Header Files                                                  *
\*************************************************************************/
#include <csl.h>
#include <csl_mmc.h>
#include <csl_emif.h>

#if (_MMC_SUPPORT)

#pragma CODE_SECTION(MMC_write, ".text:MMC_write")

/**************************************************************************
* MMC_write(MMC_Handle mmc, Uint32 cardAddr, Uint16 *buffer, Uint32 length)
*          mmc     		- Handle of MMC device reserved by previous call to 
*                         MMC_open()
*          cardAddr 	- Start Address on MMC Card where data is to be written
*          buffer  		- Pointer to buffer where data should be stored
*          no_of_bytes  - number of bytes to transfer.
***************************************************************************/

int MMC_write(MMC_Handle mmc, Uint32 cardAddr, void *buffer, Uint32 no_of_bytes) {
    Uint16 old_intm;
    Uint16 count, length;
    Uint16 *buf = buffer;
    ioport MMC_MmcRegObj *regs = mmc->mmcRegs;

    MMC_ASSERT_HANDLE(mmc, return);
    old_intm = IRQ_globalDisable();

    MMC_clearResponse(mmc);

    /* NOTE: The MMC Controller Reference Guide states that the 
    first half-word(16 bits) is to be sent before the command is
    sent and the next one after the response is obtained. The C5509
    MMC Controller expects the first half-word only after it receives
    the MMC_WRITE_BLOCK command. Hence, we do not send any data before
    the command as this is not operated upon. Only after the response 
    for the command is received do we send the first half-word.      */
    
	regs->mmcnblk = 1;
	regs->mmcblen = no_of_bytes;
	regs->mmcargh = (Uint16)((cardAddr >>  16) & 0xFFFF);
	regs->mmcargl = (Uint16)(cardAddr & 0xFFFF);
	regs->mmccmd  = MMC_WRITE_BLOCK;
	
    length = (Uint16)(no_of_bytes/2);
    
    IRQ_globalRestore(old_intm);
    
    for (count = 0; count < length; count++) 
    {
        if ((MMC_FGET_H(mmc,MMCST0,CRCWR)) || (MMC_FGET_H(mmc,MMCST0,TOUTRD)) || MMC_FGET_H(mmc,MMCST0,DATDNE)) 
           break;

        while (!MMC_FGET_H(mmc,MMCST0,DXRDY))
                      ;
        regs->mmcdxr = *buf++;
    }    

    mmc->cards->lastAddrWritten = cardAddr + length;
    
   
    if (count < length)
       return (-1);

    return 0;
}
#endif
