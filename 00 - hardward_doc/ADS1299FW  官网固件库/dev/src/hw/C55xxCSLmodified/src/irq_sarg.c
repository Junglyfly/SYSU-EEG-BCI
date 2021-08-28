/******************************************************************************\
+*           Copyright (C) 1999 Texas Instruments Incorporated.
*                           All Rights Reserved
*------------------------------------------------------------------------------
* MODULE NAME... IRQ
* FILENAME...... irq_sarg.c
* DATE CREATED.. Wed 06/09/1999 
* PROJECT....... Chip Support Library
* COMPONENT..... CSL service layer
* PREREQUISITS.. stdinc.h 
*------------------------------------------------------------------------------
* HISTORY:
*   CREATED:  06/19/2000
*   MODIFIED: 07/10/2000
*   MODIFIED: 06/29/2001 added pragma for section per function and
*                        updated for new CSL initialization/data model
*------------------------------------------------------------------------------
* DESCRIPTION:  (interface file for the IRQ module)
*
*
*
\******************************************************************************/
#define _IRQ_MOD_

#include <csl_irq.h>

#if (_IRQ_SUPPORT)

#pragma CODE_SECTION(IRQ_setArg,".text:IRQ_setArg")

/*----------------------------------------------------------------------------*/
void IRQ_setArg(Uint16 EventId, Uint32 val){
 CSL_IRQ_DATA.IrqDispatchTable[EventId].funcArg = val;
}
#endif /* IRQ_SUPPORT */
/******************************************************************************\
* End of irq_sarg.c
\******************************************************************************/

