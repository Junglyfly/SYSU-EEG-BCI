/******************************************************************************\
*           Copyright (C) 1999 Texas Instruments Incorporated.
*                           All Rights Reserved
*------------------------------------------------------------------------------
* MODULE.NAME... DMA
* FILENAME...... DMA_reset.c
* DATE CREATED.. Fri 06/11/1999 
* PROJECT....... Chip Support Library
* COMPONENT..... 
* IMPORTS....... 
*------------------------------------------------------------------------------
* HISTORY:
*   CREATED:  06/11/1999 
*   MODIFIED: 03/27/2001 Modified to support 5510PG2.0
*   MODIFIED: 06/29/2001 added pragma for section per program
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

#pragma CODE_SECTION(DMA_reset,".text:DMA_reset")

/*----------------------------------------------------------------------------*/
void DMA_reset(DMA_Handle hDma) {

  int oldgie;
  int i;

  oldgie = IRQ_globalDisable();  
  if (hDma == INV) {

  for (i = 0; i <=DMA_CHA_CNT-1; i++){

   DMA_Handle tmp = DMA_hCha(i);
    DMA_reset(tmp);
  }
  } else {

    DMA_RSETH(hDma,DMACSDP,DMA_DMACSDP_DEFAULT);
    DMA_RSETH(hDma,DMACCR,DMA_DMACCR_DEFAULT);  
    DMA_RSETH(hDma,DMACICR,DMA_DMACICR_DEFAULT);  
    DMA_RSETH(hDma,DMACSSAL,DMA_DMACSSAL_DEFAULT);  
    DMA_RSETH(hDma,DMACSSAU,DMA_DMACSSAU_DEFAULT);  
    DMA_RSETH(hDma,DMACDSAL,DMA_DMACDSAL_DEFAULT);  
    DMA_RSETH(hDma,DMACDSAU,DMA_DMACDSAU_DEFAULT); 
    DMA_RSETH(hDma,DMACEN,DMA_DMACEN_DEFAULT);  
    DMA_RSETH(hDma,DMACFN,DMA_DMACFN_DEFAULT); 
#if _DMA_DST_AND_SRC_INDEX_SUPPORT
    DMA_RSETH(hDma,DMACSFI,DMA_DMACSFI_DEFAULT);                       
    DMA_RSETH(hDma,DMACSEI,DMA_DMACSEI_DEFAULT);  
    DMA_RSETH(hDma,DMACDFI,DMA_DMACDFI_DEFAULT);                       
    DMA_RSETH(hDma,DMACDEI,DMA_DMACDEI_DEFAULT);
#else  
    DMA_RSETH(hDma,DMACFI,DMA_DMACFI_DEFAULT);                       
    DMA_RSETH(hDma,DMACEI,DMA_DMACEI_DEFAULT);  
#endif
    DMA_FSETH(hDma,DMACCR,PRIO,0); 
 
    IRQ_disable(((DMA_PrivateObj *)hDma)->EventId);
    IRQ_clear(((DMA_PrivateObj *)hDma)->EventId);
  }
  IRQ_globalRestore(oldgie);
  return;
}
#endif /* DMA_SUPPORT */
/******************************************************************************\
* End of DMA_reset.c
\******************************************************************************/

