/******************************************************************************\
+*           Copyright (C) 1999 Texas Instruments Incorporated.
*                           All Rights Reserved
*------------------------------------------------------------------------------
* MODULE NAME... IRQ
* FILENAME...... IRQ_globalRestore.c
* DATE CREATED.. Wed 06/09/1999 
* PROJECT....... Chip Support Library
* COMPONENT..... CSL service layer
* PREREQUISITS.. csl_std.h 
*------------------------------------------------------------------------------
* HISTORY:
*   CREATED:  06/09/1999 
*   MODIFIED: 12/03/1999 Modified for TMS320C54xx
*   MODIFIED: 01/04/2000 Corrrection of EVENT Numbers
*                        Modification of IRQ_functions 
*   MODIFIED: 02/21/2000 C54x Romability .
*   MODIFIED: 06/19/2000 DSPBIOS integration
*   MODIFIED: 08/05/2000 Modified for TMS320C5510
*   MODIFIED: 06/29/2001 added pragma for section per function
*------------------------------------------------------------------------------
* DESCRIPTION:  (interface file for the IRQ module)
*
*
*
\******************************************************************************/
#define _IRQ_MOD_

#include <csl_irq.h>

#if (_IRQ_SUPPORT)

#pragma CODE_SECTION(IRQ_globalRestore,".text:IRQ_globalRestore")

/*--------------------------------------------------------------------*/
void IRQ_globalRestore(int gie) {
   if (gie) {
   asm("\tNOP                      ;====> CODE AUTO-GENERATED by CSL");
#ifdef ALGEBRAIC
   asm("\tBIT(ST1,#ST1_INTM)=#1 ;====> CODE AUTO-GENERATED by CSL");
#else
   asm("\tBSET INTM                ;====> CODE AUTO-GENERATED by CSL");
#endif
   }
   else {
   asm("\tNOP                      ;====> CODE AUTO-GENERATED by CSL");
#ifdef ALGEBRAIC
   asm("\tBIT(ST1,#ST1_INTM)=#0 ;====> CODE AUTO-GENERATED by CSL");
#else
   asm("\tBCLR INTM                ;====> CODE AUTO-GENERATED by CSL");
#endif
   } 

}
#endif /* IRQ_SUPPORT */
/******************************************************************************\
* End of IRQ_globalRestore.c
\******************************************************************************/

