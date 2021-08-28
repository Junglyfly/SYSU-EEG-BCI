/******************************************************************************\
+*           Copyright (C) 1999 Texas Instruments Incorporated.
*                           All Rights Reserved
*------------------------------------------------------------------------------
* MODULE NAME... IRQ
* FILENAME...... irq_gcfg.c
* DATE CREATED.. Wed 06/09/1999 
* PROJECT....... Chip Support Library
* COMPONENT..... CSL service layer
* PREREQUISITS.. stdinc.h 
*------------------------------------------------------------------------------
* HISTORY:
*   CREATED:  06/19/2000
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

#pragma CODE_SECTION(IRQ_getConfig,".text:IRQ_getConfig")

/*----------------------------------------------------------------------------*/
void IRQ_getConfig(Uint16 EventId, IRQ_Config *Config) {
    Config->funcAddr = (IRQ_IsrPtr)(CSL_IRQ_DATA.IrqDispatchTable[EventId].funcAddr);
    Config->ierMask = CSL_IRQ_DATA.IrqDispatchTable[EventId].ierMask;
    Config->cacheCtrl = CSL_IRQ_DATA.IrqDispatchTable[EventId].cacheCtrl;
    Config->funcArg = CSL_IRQ_DATA.IrqDispatchTable[EventId].funcArg; 
}
#endif /* IRQ_SUPPORT */
/******************************************************************************\
* End of irq_gcfg.c
\******************************************************************************/

