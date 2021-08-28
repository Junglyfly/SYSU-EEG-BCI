/******************************************************************************\
+*           Copyright (C) 1999 Texas Instruments Incorporated.
*                           All Rights Reserved
*------------------------------------------------------------------------------
* MODULE NAME... IRQ
* FILENAME...... IRQ_disab.c
* DATE CREATED.. Wed 06/09/1999 
* PROJECT....... Chip Support Library
* COMPONENT..... CSL service layer
* PREREQUISITS.. stdinc.h 
*------------------------------------------------------------------------------
* HISTORY:
*   CREATED:  06/09/1999 Created
*   MODIFIED: 12/03/1999 Modified for TMS320C54xx
*   MODIFIED: 01/04/2000 Correction of EVENT Numbers
*                        Modification of IRQ_functions 
*   MODIFIED: 02/21/2000 C54x Romability .
*   MODIFIED: 06/19/2000 DSPBIOS integration
*   MODIFIED: 03/22/2001 modified to return old flag value
*   MODIFIED: 06/29/2001 added pragma for section per function
*   MODIFIED: 01/18/2002 fixed IRQ mapping problem for INT1
*------------------------------------------------------------------------------
* DESCRIPTION:  (interface file for the IRQ module)
*
*
*
\******************************************************************************/
#define _IRQ_MOD_

#include <csl_irq.h>

#if (_IRQ_SUPPORT)

#pragma CODE_SECTION(IRQ_disable,".text:IRQ_disable")

/*----------------------------------------------------------------------------*/
int IRQ_disable(Uint16 EventId) { 
  Uint16 bit,reg,mask,prev;
  int old_intm,old_flag;

  bit= EventId & 0xfu; 
  reg=((EventId & 0x10) >> 4);
  mask=IRQ_MASK16(bit);
  old_intm = IRQ_globalDisable();
  prev = (reg) ? CHIP_RGET(IER1) : CHIP_RGET(IER0);
  old_flag = (prev & mask) >> bit;
  if (reg) 
    _IER1 &= ~mask;
  else 
    _IER0 &= ~mask; 
  IRQ_globalRestore(old_intm);
  return old_flag;
}                                 
#endif /* IRQ_SUPPORT */
/******************************************************************************\
* End of IRQ_disab.c
\******************************************************************************/

