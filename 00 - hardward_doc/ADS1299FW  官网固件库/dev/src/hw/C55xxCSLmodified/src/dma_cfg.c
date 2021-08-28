/******************************************************************************\
*           Copyright (C) 1999 Texas Instruments Incorporated.
*                           All Rights Reserved
*------------------------------------------------------------------------------
* MODULE.NAME... DMA
* FILENAME...... DMA_cfg.c
* DATE CREATED.. Fri 06/11/1999 
* PROJECT....... Chip Support Library
* COMPONENT..... 
* IMPORTS....... 
*------------------------------------------------------------------------------
* HISTORY:
*   CREATED:  06/11/1999
*   MODIFIED: 06/30/2000 Modified for C55xx   
*   MODIFIED: 03/27/2001 Modified to support 5510PG2.0
*   MODIFIED: 06/29/2001 added pragma for section per function
*   MODIFIED: 04/03/2002 fixed addressing problem for large model
*   MODIFIED: 07/26/2002 fixed typo for CDMACSSAL
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

#pragma CODE_SECTION(DMA_config,".text:DMA_config")

/*----------------------------------------------------------------------------*/
void DMA_config(DMA_Handle hDma, DMA_Config *Config) {
  int oldgie;
  DMA_ASSERT_HANDLE(hDma,return);

  oldgie = IRQ_globalDisable();         
 

  DMA_RSETH(hDma,DMACSDP,Config -> dmacsdp);
  DMA_RSETH(hDma,DMACCR,Config -> dmaccr);
  DMA_RSETH(hDma,DMACICR,Config -> dmacicr); 
  DMA_RSETH(hDma,DMACSSAL,(Uint16)((Uint32)(Config -> dmacssal)& 0xFFFFu)); 
  DMA_RSETH(hDma,DMACSSAU,Config -> dmacssau);
  DMA_RSETH(hDma,DMACDSAL,(Uint16)((Uint32)(Config -> dmacdsal)& 0xFFFFu));
  DMA_RSETH(hDma,DMACDSAU,Config -> dmacdsau);
  DMA_RSETH(hDma,DMACEN,Config -> dmacen);  
  DMA_RSETH(hDma,DMACFN,Config -> dmacfn); 
#if _DMA_DST_AND_SRC_INDEX_SUPPORT
  DMA_RSETH(hDma,DMACSFI,Config -> dmacsfi); 
  DMA_RSETH(hDma,DMACSEI,Config -> dmacsei); 
  DMA_RSETH(hDma,DMACDFI,Config -> dmacdfi); 
  DMA_RSETH(hDma,DMACDEI,Config -> dmacdei); 
#else
  DMA_RSETH(hDma,DMACFI,Config -> dmacfi); 
  DMA_RSETH(hDma,DMACEI,Config -> dmacei); 
#endif
  IRQ_globalRestore(oldgie);
  return;
}
/*----------------------------------------------------------------------------*/

#endif /* DMA_SUPPORT */
/******************************************************************************\
* End of DMA_cfg.c
\******************************************************************************/

