/****************************************************************************\
*           Copyright (C) 1999-2000 Texas Instruments Incorporated.
*                           All Rights Reserved 
*------------------------------------------------------------------------------
* MODULE NAME... MMC
* FILENAME...... mmc_open.c
* DATE CREATED.. Thu May 24 14:48:09 2001
* PROJECT....... Chip Support Library
* COMPONENT..... Service Layer
* PREREQUISITS.. 
*------------------------------------------------------------------------------
*
* HISTORY:
*	 Created:		Thu May 24 14:48:09 2001 (Automatic generation)
*	 Modified:	Changed #include csl_ebselhal.h to csl_chiphal.h
*
*------------------------------------------------------------------------------
* DESCRIPTION: Returns MMC_Handle to specified device and optionally
*              initializes the MMC registers. 
*
\*************************************************************************/
#define _MMC_MOD_
/*************************************************************************\
* Included Header Files                                                  *
\*************************************************************************/

#include <csl_mmc.h>
#include <csl_pwr.h>
#include <csl_chiphal.h>

#if (_MMC_SUPPORT)

#pragma CODE_SECTION(MMC_open,".text:MMC_open")

/**************************************************************************
* MMC_open(Uint16 dev, Uint16 flags, void *mmcInit)
*          dev     - MMC device number (0 or 1)
*          
*          
***************************************************************************/

MMC_Handle MMC_open(int dev) {
  MMC_Handle mmc = (MMC_Handle)INV;
  Uint16 mmcMask = 1u << dev;
  Uint16 mcbspMask = 1u << (dev+1);
  Uint16 old_intm;

  old_intm = IRQ_globalDisable();

  // Check for correct device number //

  if (dev>1) return (MMC_Handle)INV;

  // Check to see if device is already in Use //
  if ((CSL_SYS_DATA.MmcAllocMask & (mmcMask)) ||
      (CSL_SYS_DATA.McbspPortAllocMask & (mcbspMask))
      )    
     return mmc;

  // Reserve MMC for use and also reserve corresponding MCBSP port //
  CSL_SYS_DATA.MmcAllocMask |= mmcMask;
  CSL_SYS_DATA.McbspPortAllocMask |= mcbspMask;

  // Set System Register to configure Port as MMC port //
  _XBSR |= MMC_SET_MMC_MODE << (dev ? 4 : 2);

  // Get MMC_Handle //
  mmc = &(CSL_MMC_DATA.mmc[dev]);

  // Set Default Interrupt Handler
  mmc->mmcDispatch = (dev == 0) ? &MMC_dispatch0 : &MMC_dispatch1;

  IRQ_globalRestore(old_intm);
  
  // return mmc handle
  return mmc;

}
#endif  
