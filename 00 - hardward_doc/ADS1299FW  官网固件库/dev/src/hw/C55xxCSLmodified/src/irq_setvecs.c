/******************************************************************************\
+*           Copyright (C) 1999 Texas Instruments Incorporated.
*                           All Rights Reserved
*------------------------------------------------------------------------------
* MODULE NAME... IRQ
* FILENAME...... irq_setvecs.c
* DATE CREATED.. Wed 06/09/1999 
* PROJECT....... MCRTE - multichannel runtime environment
* COMPONENT..... CSL service layer
* PREREQUISITS.. stdinc.h 
*------------------------------------------------------------------------------
* HISTORY:
*   CREATED:  06/09/1999
*   MODIFIED: 12/03/1999 Modified for TMS320C54xx
*   MODIFIED: 01/04/2000 Correction of EVENT Numbers
*                        Modification of IRQ_functions 
*   MODIFIED: 02/21/2000 C54x Romability .
*   MODIFIED: 07/11/2000 Modified for TMS320C5510 
*   MODIFIED: 03/22/2001 modified to disable interrupts around IVP updates
*   MODIFIED: 06/29/2001 added pragma for section per function
*   MODIFIED: 08/01/2001 removed IVPH update
*------------------------------------------------------------------------------
* DESCRIPTION:  (interface file for the IRQ module)
*
*
*
\******************************************************************************/
#define _IRQ_MOD_

#include <csl_irq.h>

#if (_IRQ_SUPPORT)

//******************************************************************************/

#pragma CODE_SECTION(IRQ_setVecs,".text:IRQ_setVecs")

//******************************************************************************/
int IRQ_setVecs(Uint32 Ivpd) {
   int old_intm;
   Uint16 old_ivpd;
   Uint16 val = (Uint16)((Ivpd & 0xFFFFFF00)>>8);
   old_intm = IRQ_globalDisable();
   old_ivpd = _IVPD;
   _IVPD = val;
   _IVPH = val;
   IRQ_globalRestore(old_intm);
   return old_ivpd;
 }  
#endif /* IRQ_SUPPORT */
/******************************************************************************\
* End of irq_setvecs.c
\******************************************************************************/

