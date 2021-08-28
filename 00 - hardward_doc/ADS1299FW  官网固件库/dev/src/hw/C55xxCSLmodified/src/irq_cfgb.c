/******************************************************************************\
+*           Copyright (C) 1999 Texas Instruments Incorporated.
*                           All Rights Reserved
*------------------------------------------------------------------------------
* MODULE NAME... IRQ
* FILENAME...... irq_cfgb.c
* DATE CREATED.. Wed 06/19/1999 
* PROJECT....... Chip Support Library
* COMPONENT..... CSL service layer
* PREREQUISITS.. stdinc.h 
*------------------------------------------------------------------------------
* HISTORY:
*   CREATED:  06/19/2000
*   MODIFIED: 07/10/2000 Modified for TMS320C5510
*   MODIFIED: 03/22/2001 modified for IRQ_IEMASK_SELF support
*   MODIFIED: 06/29/2001 added pragma for section per function and updated for
*                        new CSL initialization/data model
*------------------------------------------------------------------------------
* DESCRIPTION:  (interface file for the IRQ module)
*
*
*
\******************************************************************************/
#define _IRQ_MOD_

#include <csl_irq.h>

#if (_IRQ_SUPPORT)

#pragma CODE_SECTION(IRQ_configArgs, ".text:IRQ_configArgs")

/*----------------------------------------------------------------------------*/
void IRQ_configArgs(Uint16 EventId, IRQ_IsrPtr funcAddr, Uint32 funcArg,
                 Uint32 ierMask, Uint32 cacheCtrl) {
  CSL_IRQ_DATA.IrqDispatchTable[EventId].funcAddr = funcAddr;
  CSL_IRQ_DATA.IrqDispatchTable[EventId].ierMask = (ierMask == IRQ_IEMASK_SELF)?
                                                 ((Uint32)1u<<EventId): ierMask;
  CSL_IRQ_DATA.IrqDispatchTable[EventId].cacheCtrl = cacheCtrl;
  CSL_IRQ_DATA.IrqDispatchTable[EventId].funcArg = funcArg; 
}
#endif /* IRQ_SUPPORT */
/******************************************************************************\
* End of irq_cfgb.c
\******************************************************************************/

