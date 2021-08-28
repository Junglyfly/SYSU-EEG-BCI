/******************************************************************************\
*           Copyright (C) 1999 Texas Instruments Incorporated.
*                           All Rights Reserved
*------------------------------------------------------------------------------
* MODULE.NAME... DAT
* FILENAME...... DAT_wait.c
* DATE CREATED.. Thu 11/11/1999 
* PROJECT....... Chip Support Library
* COMPONENT..... CSL service layer
* IMPORTS.......
*------------------------------------------------------------------------------
* HISTORY:
*   CREATED:  11/11/1999 (BWC) Created
*   MODIFIED: 06/29/2001 added pragma for section per function and updated
*                        for new CSL initalization/data model
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

#pragma CODE_SECTION(DmaWait,".text:DAT_wait")

/*----------------------------------------------------------------------------*/
static inline void DmaWait(DMA_Handle hDma) {
    Uint16 chaMask = 1u<<((DMA_PrivateObj*)hDma)->DmaChaNum;
    if (CSL_SYS_DATA.DatSubmitMask & chaMask) {
      while (!IRQ_test(((DMA_PrivateObj*)hDma)->EventId)){};
      IRQ_clear(((DMA_PrivateObj*)hDma)->EventId);   
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

#pragma CODE_SECTION(DAT_wait,".text:DAT_wait")

/*----------------------------------------------------------------------------*/
void DAT_wait(DAT_Handle hDat) {
  DMA_Handle hDma = (DMA_Handle) hDat;
  Uint16 chaMask = 1u<<((DMA_PrivateObj*)hDma)->DmaChaNum;

  #ifdef _MCRTE_DEBUG
    if (!(CSL_SYS_DATA.DatOpenFlag & chaMask)) {
      ERR_submit(DAT_ERR_MAJOR,DAT_ERR_NOT_OPEN);
      return;
    }
  #endif
    
  DmaWait(hDma);
  CSL_SYS_DATA.DatSubmitMask &= ~chaMask;   
}
/*----------------------------------------------------------------------------*/

#endif /* DAT_SUPPORT */
/******************************************************************************\
* End of DAT_Wait.c
\******************************************************************************/

