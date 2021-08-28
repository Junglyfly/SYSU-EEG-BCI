/******************************************************************************\
*           Copyright (C) 1999 Texas Instruments Incorporated.
*                           All Rights Reserved
*------------------------------------------------------------------------------
* MODULE.NAME... GPT
* FILENAME...... gpt_init32.c
* PROJECT....... CSL - Chip Support Library
* COMPONENT..... 
*------------------------------------------------------------------------------
* HISTORY:
*   CREATED:   02/10/2002 
*   MODIFIED : 10/28/2003 Fixed the bug to configure gptgctl1 register
*              03/12/2003 changed structure member names
*------------------------------------------------------------------------------
* DESCRIPTION:  (body file for initializing 32 bit Dual mode
*                for 5502 general purpose timer)
\******************************************************************************/
#define _GPT_MOD_

/****************************************\
* include files
\****************************************/    

#include <csl_gpt.h>
 
#if (_GPT_SUPPORT)
             
/******************************************************************************\
*                        G L O B A L   S E C T I O N
\******************************************************************************/

/****************************************\
* TIMER global function definitions
\****************************************/

#pragma CODE_SECTION(GPT_initDual32,".text:GPT_initDual32")

/*----------------------------------------------------------------------------*/
void GPT_initDual32(GPT_Handle hGpt, Uint16 dt1ctl, Uint16 dt2ctl, 
           Uint32 dt1prd, Uint32 dt2prd, Uint16 dt2prsc){
  int oldgie;
  ioport GPT_RegObj *regPtr = ((GPT_PrivateObj *)hGpt)->regs;

  GPT_ASSERT_HANDLE(hGpt,return);

    oldgie = IRQ_globalDisable();  
 
   (regPtr->gptgctl1) = 0x0000u;
   (regPtr->gptprd1) = dt1prd;
   (regPtr->gptprd2) = dt2prd;
   (regPtr->gptctl1) = dt1ctl;
   (regPtr->gptctl2) = dt2ctl;
   (regPtr->gptgctl1) = GPT_FMK(GPTGCTL1,TIMMODE,1) |
   						((dt2prsc & 0xF) << 8);  
  
  IRQ_globalRestore(oldgie); 
 
}
/*----------------------------------------------------------------------------*/

#endif /* _GPT_SUPPORT */
/******************************************************************************\
* End of gpt_init32.c
\******************************************************************************/

