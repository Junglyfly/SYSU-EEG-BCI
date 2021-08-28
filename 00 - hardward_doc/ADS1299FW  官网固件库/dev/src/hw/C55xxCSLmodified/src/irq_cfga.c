/******************************************************************************\
+*           Copyright (C) 1999 Texas Instruments Incorporated.
*                           All Rights Reserved
*------------------------------------------------------------------------------
* MODULE NAME... IRQ
* FILENAME...... irq_cfga.c
* DATE CREATED.. Wed 06/19/1999 
* PROJECT....... Chip Support Library
* COMPONENT..... CSL service layer
* PREREQUISITS.. stdinc.h 
*------------------------------------------------------------------------------
* HISTORY:
*   CREATED:  06/19/2000
*   MODIFIED: 07/10/2000 Modified for TMS320C5510
*   MODIFIED: 03/22/2001 modified to support IRQ_IEMASK_SELF
*   MODIFIED: 06/29/2001 added pragma for section per function and updated
*                        for new CSL initialization/data model
*------------------------------------------------------------------------------
* DESCRIPTION:  (interface file for the IRQ module)
*
*
*
\******************************************************************************/
#define _IRQ_MOD_

#include <csl_irq.h>

#if (_IRQ_SUPPORT)

#pragma CODE_SECTION(IRQ_config,".text:IRQ_config")

/*----------------------------------------------------------------------------*/
 void IRQ_config(Uint16 EventId, IRQ_Config *Config) {
   CSL_IRQ_DATA.IrqDispatchTable[EventId].funcAddr = Config->funcAddr;
   CSL_IRQ_DATA.IrqDispatchTable[EventId].ierMask = (Config->ierMask == IRQ_IEMASK_SELF)? 
                                                  ((Uint32)1u<<EventId) : Config->ierMask;
   CSL_IRQ_DATA.IrqDispatchTable[EventId].cacheCtrl = Config->cacheCtrl;
   CSL_IRQ_DATA.IrqDispatchTable[EventId].funcArg = Config->funcArg;
}
#endif /* IRQ_SUPPORT */
/******************************************************************************\
* End of irq_cfga.c
\******************************************************************************/

