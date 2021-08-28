/******************************************************************************\
+*           Copyright (C) 1999 Texas Instruments Incorporated.
*                           All Rights Reserved
*------------------------------------------------------------------------------
* MODULE NAME... IRQ
* FILENAME...... irq_map.c
* DATE CREATED.. Wed 06/09/1999 
* PROJECT....... Chip Support Library
* COMPONENT..... CSL service layer
* PREREQUISITS.. stdinc.h 
*------------------------------------------------------------------------------
* HISTORY:
*   CREATED:  06/19/2000
*   MODIFIED: 06/29/2001 added pragma for section per function and
*                        updated for new CSL intialization/data model
*------------------------------------------------------------------------------
* DESCRIPTION:  (interface file for the IRQ module)
*
*
*
\******************************************************************************/
#define _IRQ_MOD_

#include <csl_irq.h>

#if (_IRQ_SUPPORT)

#pragma CODE_SECTION(IRQ_map,".text:IRQ_map")

/*----------------------------------------------------------------------------*/
void IRQ_map(Uint16 EventId){
  CSL_IRQ_DATA.IrqIntTable[EventId] = IRQ_MASK32(EventId);
}
#endif /* IRQ_SUPPORT */
/******************************************************************************\
* End of irq_map.c
\******************************************************************************/

