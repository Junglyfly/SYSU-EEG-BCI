/******************************************************************************\
+*           Copyright (C) 1999 Texas Instruments Incorporated.
*                           All Rights Reserved
*------------------------------------------------------------------------------
* MODULE NAME... IRQ
* FILENAME...... IRQ_rest.c
* DATE CREATED.. Thu 03/22/2001 
* PROJECT....... Chip Support Library
* COMPONENT..... CSL service layer
* PREREQUISITS.. stdinc.h 
*------------------------------------------------------------------------------
* HISTORY:
*   CREATED:  03/22/2001 Created
*   MODIFIED: 06/29/2001 added pragma for section per function
*   MODIFIED: 01/18/2002 fixed IRG mapping problem for INT1
*------------------------------------------------------------------------------
* DESCRIPTION:  (interface file for the IRQ module)
*
*
*
\******************************************************************************/
#define _IRQ_MOD_

#include <csl_irq.h>

#if (_IRQ_SUPPORT)

#pragma CODE_SECTION(IRQ_restore,".text:IRQ_restore")

/*--------------------------------------------------------------------*/
  void IRQ_restore(Uint16 EventId, Uint16 Old_flag) {
   Uint16 bit,reg,prev,value,mask;
   int old_intm;

   bit= EventId & 0xfu; 
   reg=((EventId & 0x10) >> 4);
   mask= IRQ_MASK16(bit);
   old_intm = IRQ_globalDisable();
   prev = (reg) ? CHIP_RGET(IER1) : CHIP_RGET(IER0);
   value= (prev & (~mask)) | (Old_flag << bit);
   if (reg) 
     _IER1 = value;
   else 
    _IER0  = value; 
   IRQ_globalRestore(old_intm);
} 
#endif /* IRQ_SUPPORT */
/******************************************************************************\
* End of IRQ_rest.c
\******************************************************************************/

