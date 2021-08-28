/******************************************************************************\
*           Copyright (C) 1999 Texas Instruments Incorporated.
*                           All Rights Reserved
*------------------------------------------------------------------------------
* MODULE.NAME... DAT
* FILENAME...... DAT_close.c
* DATE CREATED.. Thu 11/11/1999 
* PROJECT....... Chip Support Library
* COMPONENT..... CSL service layer
* IMPORTS.......
*------------------------------------------------------------------------------
* HISTORY:
*   CREATED:  11/11/1999 
*   MODIFIED: 07/10/2000 Modified for C55xx
*   MODIFIED: 06/29/2001 added pragma for section per function and updated
*                        for new CSL initialization/data model
*------------------------------------------------------------------------------
* DESCRIPTION:  (C body file for the DAT module)
*
*
*
\******************************************************************************/
#define _DAT_MOD_

/****************************************\
* include files
\****************************************/

#include <csl_dat.h>

#if (_DAT_SUPPORT)
/******************************************************************************\
*                         L O C A L   S E C T I O N
\******************************************************************************/

/****************************************\
* DAT static macro declarations
\****************************************/

/****************************************\
* DAT static typedef declarations
\****************************************/

/****************************************\
* DAT static function declarations
\****************************************/
static inline void DmaWait();

/****************************************\
* DAT static variable definitions
\****************************************/
/*
static CSLBool Initialized   = FALSE;
static DMA_Handle hDma    = INV;
static Uint16 EventId     = 0x0000;
static Uint16 ChaPriority = 0;
static int SubmitMask     = 0;
static int OpenFlag       = 0;
*/
/****************************************\
* DAT static function definitions
\****************************************/

#pragma CODE_SECTION(DmaWait,".text:DAT_close")

/*----------------------------------------------------------------------------*/
static inline void DmaWait(DMA_Handle hDma) {
   Uint16 chaMask = 1u<<((DMA_PrivateObj*)hDma)->DmaChaNum;
    if (CSL_SYS_DATA.DatSubmitMask & chaMask) {
      while (!IRQ_test(((DMA_PrivateObj*)hDma)->EventId)){};
      IRQ_clear(((DMA_PrivateObj*)hDma)->EventId); 
      CSL_SYS_DATA.DatSubmitMask &= (~chaMask);  
    }
  }

/*----------------------------------------------------------------------------*/

/******************************************************************************\
*                        G L O B A L   S E C T I O N
\******************************************************************************/

/****************************************\
* DAT global variable definitions
\****************************************/

/****************************************\
* DAT global function definitions
\****************************************/

#pragma CODE_SECTION(DAT_close,".text:DAT_close")

/*----------------------------------------------------------------------------*/
void DAT_close(DAT_Handle hDat) {
  
  int oldgie;
  Uint16 chaMask;
  DMA_Handle hDma = (DMA_Handle) hDat;


  chaMask = 1u<<((DMA_PrivateObj*)hDma)->DmaChaNum;
  oldgie = IRQ_globalDisable();  

  if (CSL_SYS_DATA.DatOpenFlag & chaMask) {
    DmaWait(hDma);
    DMA_close(hDma);
    CSL_SYS_DATA.DatOpenFlag &= (~chaMask);
  }

  IRQ_globalRestore(oldgie);
}
/*----------------------------------------------------------------------------*/

#endif /* DAT_SUPPORT */
/******************************************************************************\
* End of DAT_close.c
\******************************************************************************/

