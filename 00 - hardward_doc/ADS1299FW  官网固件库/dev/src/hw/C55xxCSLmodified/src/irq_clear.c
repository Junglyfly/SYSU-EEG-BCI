/******************************************************************************\
+*           Copyright (C) 1999 Texas Instruments Incorporated.
*                           All Rights Reserved
*------------------------------------------------------------------------------
* MODULE NAME... IRQ
* FILENAME...... irq_clear.c
* DATE CREATED.. Wed 06/19/1999 
* PROJECT....... Chip Support Library
* COMPONENT..... CSL service layer
* PREREQUISITS.. stdinc.h 
*------------------------------------------------------------------------------
* HISTORY:
*   CREATED:  06/19/2000
*   MODIFIED: 03/22/2001 modified to disable interrupts around IER update
*   MODIFIED: 06/29/2001 added pragma for section per function
*   MODIFIED: 01/18/2002 ficed IRQ mapping problem for INT1
*------------------------------------------------------------------------------
* DESCRIPTION:  (interface file for the IRQ module)
*
*
*
\******************************************************************************/
#define _IRQ_MOD_

#include <csl_irq.h>

#if (_IRQ_SUPPORT)

#pragma CODE_SECTION(IRQ_clear,".text:IRQ_clear")

/*----------------------------------------------------------------------------*/
void IRQ_clear(Uint16 EventId) {   
  Uint16 bit, reg, mask;
  int old_intm;
  bit= EventId & 0xfu; 
  reg=((EventId & 0x10) >> 4);
  mask=IRQ_MASK16(bit);
  old_intm = IRQ_globalDisable();
  if (reg)
    _IFR1 = mask;
  else
    _IFR0 = mask;
  IRQ_globalRestore(old_intm);
}
#endif /* IRQ_SUPPORT */
/******************************************************************************\
* End of IRQ_clear.c
\******************************************************************************/

