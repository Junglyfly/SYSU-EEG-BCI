/******************************************************************************\
*           Copyright (C) 1999 Texas Instruments Incorporated.
*                           All Rights Reserved
*------------------------------------------------------------------------------
* MODULE.NAME... DAT
* FILENAME...... DAT_open.c
* DATE CREATED.. Thu 11/11/1999
* PROJECT....... Chip Support Library
* COMPONENT..... CSL service layer
* IMPORTS.......
*------------------------------------------------------------------------------
* HISTORY:
*   CREATED:  11/11/1999
*   MODIFIED: 07/10/2000 modified for C55xx
*   MODIFIED: 06/29/2001 added pragma for section per function and updated
*                        for new CSL initialization/data model
*			  09/16/2003 Added chech for invalid channel number
*   MODIFIED: 08/24/2004 Fixed interrupt restore issues
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

/****************************************\
* DAT static variable definitions
\****************************************/
/****************************************\
* DAT static function definitions
\****************************************/
/*----------------------------------------------------------------------------*/
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

#pragma CODE_SECTION(DAT_open,".text:DAT_open")

/*----------------------------------------------------------------------------*/
DAT_Handle DAT_open(int ChaNum, int Priority, Uint32 Flags) {
  int oldgie;
  Uint16 chaMask = 1u << ChaNum;
  DMA_Handle hDma = INV;

  UNREFERENCED_PARAMETER(Flags);

  if ((ChaNum<DAT_CHA_ANY) || (ChaNum>=_DMA_CHA_CNT)) {
      return (DAT_Handle)INV;
  }


  oldgie = IRQ_globalDisable();

  if (!(CSL_SYS_DATA.DatOpenFlag & chaMask)) {

      hDma = DMA_open(ChaNum,DMA_OPEN_RESET);

    #ifdef _MCRTE_DEBUG
      if (hDma==INV) {
        ERR_submit(DAT_ERR_MAJOR,DAT_ERR_ALLOC);
        IRQ_globalRestore(oldgie);
        return;
      }
    #endif

    if (hDma != INV) {
      IRQ_clear(((DMA_PrivateObj*)hDma)->EventId);
      CSL_SYS_DATA.DatChaPriority = (CSL_SYS_DATA.DatChaPriority & (~chaMask))
                                     | ((Priority & 0x1u)<<ChaNum);
      CSL_SYS_DATA.DatOpenFlag |= chaMask;
    }
 }

  IRQ_globalRestore(oldgie);
  return (DAT_Handle) hDma;
}

/*----------------------------------------------------------------------------*/

#endif /* DAT_SUPPORT */
/******************************************************************************\
* End of DAT_open.c
\******************************************************************************/

