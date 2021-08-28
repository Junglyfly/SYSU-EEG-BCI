/******************************************************************************\
*           Copyright (C) 1999 Texas Instruments Incorporated.
*                           All Rights Reserved
*------------------------------------------------------------------------------
* MODULE.NAME... DMA
* FILENAME...... DMA_cfgb.c
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
*   MODIFIED: 04/03/2002 Made src and dst equivalnet to config implementation
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

#pragma CODE_SECTION(DMA_configArgs,".text:DMA_configArgs")

/*----------------------------------------------------------------------------*/
#if _DMA_DST_AND_SRC_INDEX_SUPPORT
  void DMA_configArgs(DMA_Handle hDma, Uint16 dmacsdp, Uint16 dmaccr, Uint16 dmacicr, 
    DMA_AdrPtr dmacssal, Uint16 dmacssau, DMA_AdrPtr dmacdsal, Uint16 dmacdsau, Uint16 dmacen,
    Uint16 dmacfn, Int16 dmacsfi, Int16 dmacsei, Int16 dmacdfi, Int16 dmacdei)
#else
  void DMA_configArgs(DMA_Handle hDma, Uint16 dmacsdp, Uint16 dmaccr,
    Uint16 dmacicr, DMA_AdrPtr dmacssal, Uint16 dmacssau, DMA_AdrPtr dmacdsal, Uint16 dmacdsau,
    Uint16 dmacen, Uint16 dmacfn, Int16 dmacfi, Int16 dmacei)
#endif
{
   int oldgie; 
  DMA_ASSERT_HANDLE(hDma,return);


  oldgie = IRQ_globalDisable();  

  DMA_RSETH(hDma,DMACSDP,dmacsdp);
  DMA_RSETH(hDma,DMACCR,dmaccr);
  DMA_RSETH(hDma,DMACICR,dmacicr); 
  DMA_RSETH(hDma,DMACSSAL,(Uint16)((Uint32)(dmacssal)& 0xFFFFu)); 
  DMA_RSETH(hDma,DMACSSAU,dmacssau);
  DMA_RSETH(hDma,DMACDSAL,(Uint16)((Uint32)(dmacdsal)& 0xFFFFu));
  DMA_RSETH(hDma,DMACDSAU,dmacdsau);
  DMA_RSETH(hDma,DMACEN,dmacen);  
  DMA_RSETH(hDma,DMACFN,dmacfn); 
#if _DMA_DST_AND_SRC_INDEX_SUPPORT
  DMA_RSETH(hDma,DMACSFI,dmacsfi); 
  DMA_RSETH(hDma,DMACSEI,dmacsei); 
  DMA_RSETH(hDma,DMACDFI,dmacdfi); 
  DMA_RSETH(hDma,DMACDEI,dmacdei);
#else 
  DMA_RSETH(hDma,DMACFI,dmacfi); 
  DMA_RSETH(hDma,DMACEI,dmacei);
#endif 

  IRQ_globalRestore(oldgie);
  return;
}

/*----------------------------------------------------------------------------*/

#endif /* DMA_SUPPORT */
/******************************************************************************\
* End of DMA_cfgb.c
\******************************************************************************/

