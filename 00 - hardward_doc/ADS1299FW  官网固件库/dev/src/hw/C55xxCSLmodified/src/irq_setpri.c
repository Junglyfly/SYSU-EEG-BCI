/******************************************************************************\
+*           Copyright (C) 1999 Texas Instruments Incorporated.
*                           All Rights Reserved
*------------------------------------------------------------------------------
* MODULE NAME... IRQ
* FILENAME...... irq_setpri.c
* DATE CREATED.. Wed 06/19/1999 
* PROJECT....... Chip Support Library
* COMPONENT..... CSL service layer
* PREREQUISITS.. stdinc.h 
*------------------------------------------------------------------------------
* HISTORY:
*   CREATED:  02/15/2002
*------------------------------------------------------------------------------
* DESCRIPTION:  (interface file for the IRQ module)
*
*
*
\******************************************************************************/
#define _IRQ_MOD_

#include <csl_irq.h>

#if (_IRQ_SUPPORT)

#pragma CODE_SECTION(IRQ_setPriority,".text:IRQ_setPriority")

/*----------------------------------------------------------------------------*/
void IRQ_setPriority(Uint16 EventId, Uint16 priority) {   
   Uint16 odd = EventId & 0x1;
   Uint16 regIndex = EventId > 1;
   Uint16 *regPtr = (CSL_IRQ_DATA).perRegs;

    PREG16(regPtr[regIndex]) |= (odd) ? (priority << 8) : priority;
 } 

#endif /* IRQ_SUPPORT */
/******************************************************************************\
* End of IRQ_clear.c
\******************************************************************************/

