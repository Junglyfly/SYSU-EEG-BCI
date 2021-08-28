/******************************************************************************\
*           Copyright (C) 1999 Texas Instruments Incorporated.
*                           All Rights Reserved
*------------------------------------------------------------------------------
* MODULE.NAME... DMA
* FILENAME...... DMA_open.c
* DATE CREATED.. Fri 06/30/2000 
* PROJECT....... Chip Support Library
* COMPONENT..... 
* IMPORTS....... 
*------------------------------------------------------------------------------
* HISTORY:
*   CREATED:  06/11/1999 (BWC) Created
*   MODIFIED: 06/30/2000 (IC) Modified for C55xx   
*   MODIFIED: 06/29/2001 added pragma for sectiuon per function and
*                        updated for new CSL initialization/data model
*             08/24/2004 Removed IRQ_restoreGie() from 55x CSL 
*------------------------------------------------------------------------------
* DESCRIPTION:  (body file for the DMA module)
*
*
*
\******************************************************************************/
#define _DMA_MOD_

/****************************************\
* include files
\****************************************/

#include <csl_dma.h>

#if (_DMA_SUPPORT)
/******************************************************************************\
*                         L O C A L   S E C T I O N
\******************************************************************************/

/****************************************\
* DMA static macro declarations
\****************************************/

/****************************************\
* DMA static typedef declarations
\****************************************/

/****************************************\
* DMA static function declarations
\****************************************/

/****************************************\
* DMA static variable definitions
\****************************************/

/****************************************\
* DMA static function definitions
\****************************************/

/******************************************************************************\
*                        G L O B A L   S E C T I O N
\******************************************************************************/

/****************************************\
* DMA global variable definitions
\****************************************/

/****************************************\
* DMA global function definitions
\****************************************/

#pragma CODE_SECTION(DMA_open,".text:DMA_open");

/*----------------------------------------------------------------------------*/
  DMA_Handle DMA_open(int ChaNum, Uint32 Flags) {

  DMA_Handle hDma = INV;
  int cha = ChaNum;

  int oldgie;

  oldgie = IRQ_globalDisable();    

  #ifdef _MCRTE_DEBUG
  if (!((ChaNum==-1)||(ChaNum>=0)&&(ChaNum<DMA_CHA_CNT))) {
    ERR_submit(DMA_ERR_MAJOR, DMA_ERR_ALLOC);
    IRQ_globalRestore(oldgie);
    return (DMA_Handle)INV;
  }
  #endif

  if (ChaNum==-1) {
    for (cha=0; cha<DMA_CHA_CNT; cha++) {
      if (!(CSL_SYS_DATA.DmaAllocMask & (1u<<cha))) {
        hDma = DMA_hCha(cha);
        break;
      }
    }
  } else if (!(CSL_SYS_DATA.DmaAllocMask & (1u<<cha))) {
    hDma = DMA_hCha(ChaNum);
  }

  #ifdef _MCRTE_DEBUG
    if (hDma == INV) {
      ERR_submit(DMA_ERR_MAJOR, DMA_ERR_ALLOC);
      IRQ_globalRestore(oldgie);
      return (DMA_Handle)INV;
  }
  #endif

  if (hDma != INV) {
    CSL_SYS_DATA.DmaAllocMask |= 1u<<cha;
  
    if (Flags & DMA_OPEN_RESET) {
      DMA_reset(hDma);
    }
  }


  IRQ_globalRestore(oldgie);

  return hDma;
}
/*----------------------------------------------------------------------------*/

#endif /* DMA_SUPPORT */
/******************************************************************************\
* End of DMA_open.c
\******************************************************************************/

