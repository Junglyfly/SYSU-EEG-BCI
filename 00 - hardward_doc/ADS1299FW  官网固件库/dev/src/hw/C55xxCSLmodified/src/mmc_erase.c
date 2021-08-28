/****************************************************************************\
*           Copyright (C) 1999-2000 Texas Instruments Incorporated.
*                           All Rights Reserved 
*------------------------------------------------------------------------------
* MODULE NAME... MMC
* FILENAME...... mmc_erase.c
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
* DESCRIPTION: Erases a single block of data on MMC card
*
\*************************************************************************/
#define _MMC_MOD_
/*************************************************************************\
* Included Header Files                                                  *
\*************************************************************************/
#include <stdarg.h>
#include <csl.h>
#include <csl_mmc.h>

#if (_MMC_SUPPORT)

#pragma CODE_SECTION(MMC_erase,".text:MMC_erase")

/**************************************************************************
* MMC_erase(MMC_Handle mmc, Uint32 startAddr, Uint32 endAddr, Uint16 wait,...);
*          mmc     - Handle of MMC device reserved by previous call to 
*                    MMC_open()
*          startAddr - Start Address on MMC Card to be erased
*          endAddr   - Ending Address for block to erase
*          wait      - TRUE if function should wait for erase complete
*                      before returning
*          ...       Start of variable length argument list
*          untag     - Pointer to list of sectors to UNTAG from the erase group
*          numTags   - Number of sectors to UNTAG.
***************************************************************************/

int MMC_erase(MMC_Handle mmc, Uint32 startAddr, Uint32 endAddr, Uint16 flags,...){
  va_list argList;
  Uint16 old_intm;
  Uint16 numSectors;
  Uint32 *sectors;

  ioport MMC_MmcRegObj *regs;

  MMC_ASSERT_HANDLE(mmc,return);

  old_intm = IRQ_globalDisable();


  regs = mmc->mmcRegs;


  // Clear out response registers

  MMC_clearResponse(mmc);

  // Format and send command

  regs->mmcargh = (Uint16)((startAddr >> 16)& 0xFFFFu);
  regs->mmcargl = (Uint16)(startAddr & 0xFFFFu);
  regs->mmccmd = MMC_TAG_SECTOR_START;

  // Wait for response done
  MMC_getStatus(mmc,MMC_ST0_RSPDNE);
  while (!MMC_getStatus(mmc,MMC_USE_LAST_STATUS | MMC_ST0_RSPDNE)) {
    if (MMC_getStatus(mmc,MMC_USE_LAST_STATUS | MMC_ST0_TOUTRS ))
       return 0;
  }

  // Format and send command

  regs->mmcargh = (Uint16)((endAddr >> 16)& 0xFFFFu);
  regs->mmcargl = (Uint16)(endAddr & 0xFFFFu);
  regs->mmccmd = MMC_TAG_SECTOR_END;

  // Wait for response done

  // Wait for response done
  MMC_getStatus(mmc,MMC_ST0_RSPDNE);
  while (!MMC_getStatus(mmc,MMC_USE_LAST_STATUS | MMC_ST0_RSPDNE)) {
    if (MMC_getStatus(mmc,MMC_USE_LAST_STATUS | MMC_ST0_TOUTRS ))
       return 0;
  }

  // Check to see if there are any sectors to be untagged
  if (flags & MMC_UNTAG_ERASE_SECTORS){

    va_start(argList,flags);
    
    sectors = va_arg(argList, Uint32 *);
    numSectors = va_arg(argList, Uint16);
    va_end(argList);

    if (numSectors > 16)
      numSectors = 16;

    do{
      regs->mmcargh = (Uint16)(((*sectors)>>16) & 0xFFFFu);
      regs->mmcargl = (Uint16)((*sectors) & 0xFFFFu);
      regs->mmccmd = MMC_UNTAG_SECTOR;

      // Wait for response done
      MMC_getStatus(mmc,MMC_ST0_RSPDNE);
      while (!MMC_getStatus(mmc,MMC_USE_LAST_STATUS | MMC_ST0_RSPDNE)) {
       if (MMC_getStatus(mmc,MMC_USE_LAST_STATUS | MMC_ST0_TOUTRS ))
         return 0;
      }
      ++sectors;
    }while(--numSectors);
  }
  
  

  // Format and send command

  regs->mmcargh = MMC_STUFF_BITS;
  regs->mmcargl = MMC_STUFF_BITS;
  regs->mmccmd = MMC_ERASE|0x0100;  /* enable busy expect bit */

  // Wait for response done

  // Wait for response done
 
  MMC_getStatus(mmc,MMC_ST0_RSPDNE);
  while (!MMC_getStatus(mmc,MMC_USE_LAST_STATUS | MMC_ST0_RSPDNE)) {
    if (MMC_getStatus(mmc,MMC_USE_LAST_STATUS | MMC_ST0_TOUTRS ))
       return 0;
  }

  // Wait until card is no longer sending busy
/*
  if (flags & MMC_WAIT_FOR_DATDNE){
    while (!MMC_getStatus(mmc, MMC_USE_LAST_STATUS | MMC_ST0_DATDNE)){
      if(MMC_getStatus(mmc, MMC_USE_LAST_STATUS | MMC_ST0_TOUTRD))
        return 0;
    }
  }  */

 IRQ_globalRestore(old_intm);
 			      
 return 1;
}
#endif 
