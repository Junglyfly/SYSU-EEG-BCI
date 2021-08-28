/******************************************************************************\
+*           Copyright (C) 1999 Texas Instruments Incorporated.
*                           All Rights Reserved
*------------------------------------------------------------------------------
* MODULE NAME... IRQ
* FILENAME...... IRQ_test.c
* DATE CREATED.. Wed 06/09/1999 
* PROJECT....... Chip Support Library
* COMPONENT..... CSL service layer
* PREREQUISITS.. stdinc.h 
*------------------------------------------------------------------------------
* HISTORY:
*   CREATED:  06/09/1999 
*   MODIFIED: 12/03/1999 Modified for TMS320C54xx
*   MODIFIED: 01/04/2000 Correction of EVENT Numbers
*                        Modification of IRQ_functions 
*   MODIFIED: 02/21/2000 C54x Romability .
*   MODIFIED: 06/19/2000 DSPBIOS integration
*   MODIFIED: 07/10/2000 Modified for TMS320C5510
*   MODIFIED: 03/22/2001 modified to disable interrupts around IFR ref
*   MODIFIED: 03/23/2001 modified to use new CSL types to avoid conflict
*                        with DSPBIOS defined types.
*   MODIFIED: 06/29/2001 added pragma for section per function
*   MODIFIED: 01/18/2002 fixed EventID mapping problem for INT1
*------------------------------------------------------------------------------
* DESCRIPTION:  (interface file for the IRQ module)
*
*
*
\******************************************************************************/
#define _IRQ_MOD_

#include <csl_irq.h>

#if (_IRQ_SUPPORT)

//---------------------------------------------------------------

#pragma CODE_SECTION(IRQ_test,".text:IRQ_test")

//---------------------------------------------------------------
CSLBool IRQ_test(Uint16 EventId) {
  Uint16 bit,reg,mask;
  int old_intm;
  bit= EventId & 0xfu; 
  reg=((EventId & 0x10u) >> 4);
  mask=IRQ_MASK16(bit);
  old_intm = IRQ_globalDisable();
  bit = (CSLBool)( (reg?(_IFR1 & mask): (_IFR0 & mask)) != 0);
  IRQ_globalRestore(old_intm);
  return bit;
}

#endif /* IRQ_SUPPORT */
/******************************************************************************\
* End of IRQ_test.c
\******************************************************************************/

