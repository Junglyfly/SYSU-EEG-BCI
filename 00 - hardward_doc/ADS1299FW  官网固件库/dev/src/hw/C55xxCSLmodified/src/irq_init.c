/******************************************************************************\
*           Copyright (C) 1999 Texas Instruments Incorporated.
*                           All Rights Reserved
*------------------------------------------------------------------------------
* MODULE.NAME... IRQ
* FILENAME...... irq_init.c
* DATE CREATED.. Wed 06/09/1999 
* PROJECT....... Chip Support Library
* COMPONENT..... CSL service layer
* IMPORTS....... none
*------------------------------------------------------------------------------
* HISTORY:
*   CREATED:  06/09/1999 
*   MODIFIED: 12/02/1999 C54x Modification.
*   MODIFIED: 02/18/2000 C54x Romability ( All static variable were removed) .
*   MODIFIED: 06/29/2001 added pragma for section per function and updated
*                        for new CSL initialization/data model
*------------------------------------------------------------------------------
* DESCRIPTION:  (body file for the IRQ module)
*
*
*
\******************************************************************************/
#define _IRQ_MOD_

/****************************************\
* include files
\****************************************/

#include <csl_irq.h>

#if (_IRQ_SUPPORT)                                                               
/******************************************************************************\
*                         L O C A L   S E C T I O N
\******************************************************************************/

/****************************************\
* IRQ static variable definitions
\****************************************/

/****************************************\
* IRQ static function definitions
\****************************************/
/* See for romability */     

/******************************************************************************\
*                        G L O B A L   S E C T I O N
\******************************************************************************/
       
/****************************************\
* IRQ global variable definitions
\****************************************/

/* See definition under irq.h */

#pragma CODE_SECTION(_IRQ_init,".text:_IRQ_init")

/*----------------------------------------------------------------------------*/
void _IRQ_init(_IRQ_Dispatch *dispatchTable, Uint16 biosPresent){

   Uint16 i; 
  
  
   if (dispatchTable != INV) {
     CSL_IRQ_DATA.IrqDispatchTable = dispatchTable;
   }

   for (i=0; i<=IRQ_EVENT_CNT-1; i++)
    CSL_IRQ_DATA.IrqIntTable[i] = i;
   
   for(i=0; i<= IRQ_EVENT_CNT-1; i++)
    CSL_IRQ_DATA.IrqEventTable[i] = IRQ_MASK32(i);

    CSL_SYS_DATA.OsPresent = biosPresent;   
             
}

/*----------------------------------------------------------------------------*/

#endif /* IRQ_SUPPORT */
/******************************************************************************\
* End of irq_init.c
\******************************************************************************/

