/******************************************************************************\
*           Copyright (C) 1999 Texas Instruments Incorporated.
*                           All Rights Reserved
*------------------------------------------------------------------------------
* MODULE.NAME... DMA
* FILENAME...... DMA_gcfg.c
* DATE CREATED.. Thu 05/10/2001
* PROJECT....... CSL - Chip Support Library
* COMPONENT..... 
* IMPORTS....... 
*------------------------------------------------------------------------------
* HISTORY:
*   CREATED:  05/10/2001 Created
*   MODIFIED: 06/29/2001 added pragma for section per function
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

#pragma CODE_SECTION(DMA_getConfig,".text:DMA_getConfig")

/*----------------------------------------------------------------------------*/
void DMA_getConfig(DMA_Handle hDma, DMA_Config *Config) {
  int oldgie;
  DMA_ASSERT_HANDLE(hDma,return);

  oldgie = IRQ_globalDisable();         
 

  Config -> dmacsdp = DMA_RGETH(hDma,DMACSDP);
  Config -> dmaccr  = DMA_RGETH(hDma,DMACCR);
  Config -> dmacicr = DMA_RGETH(hDma,DMACICR); 
  Config -> dmacssal = (DMA_AdrPtr)(DMA_RGETH(hDma,DMACSSAL)); 
  Config -> dmacssau =  DMA_RGETH(hDma,DMACSSAU);
  Config -> dmacdsal = (DMA_AdrPtr)(DMA_RGETH(hDma,DMACDSAL));
  Config -> dmacdsau = DMA_RGETH(hDma,DMACDSAU); 
  Config -> dmacen  = DMA_RGETH(hDma,DMACEN);  
  Config -> dmacfn = DMA_RGETH(hDma,DMACFN); 
#if _DMA_DST_AND_SRC_INDEX_SUPPORT
  Config -> dmacsfi = DMA_RGETH(hDma,DMACSFI); 
  Config -> dmacsei = DMA_RGETH(hDma,DMACSEI); 
  Config -> dmacdfi = DMA_RGETH(hDma,DMACDFI); 
  Config -> dmacdei = DMA_RGETH(hDma,DMACDEI); 
#else
  Config -> dmacfi = DMA_RGETH(hDma,DMACFI); 
  Config -> dmacei = DMA_RGETH(hDma,DMACEI); 
#endif
  IRQ_globalRestore(oldgie);
  return;
}
/*----------------------------------------------------------------------------*/

#endif /* DMA_SUPPORT */
/******************************************************************************\
* End of DMA_gcfg.c
\******************************************************************************/

