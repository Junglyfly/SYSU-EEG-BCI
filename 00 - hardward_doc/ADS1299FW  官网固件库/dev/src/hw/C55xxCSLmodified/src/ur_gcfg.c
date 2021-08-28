/******************************************************************************\
*           Copyright (C) 2001 Texas Instruments Incorporated.
*                           All Rights Reserved
*------------------------------------------------------------------------------
* MODULE.NAME... UART
* FILENAME...... uart_gcfga.c
* DATE CREATED.. 01/18/2002 
* PROJECT....... Chip Support Library
* COMPONENT..... 
* IMPORTS....... 
*------------------------------------------------------------------------------
* HISTORY:
*   01/18/2002   Created 
*------------------------------------------------------------------------------
* DESCRIPTION:  (body file for the UART module)
*
*
*
\******************************************************************************/
#define _UART_MOD_

/****************************************\
* include files
\****************************************/  
#include <csl_irq.h>
#if (_UART_SUPPORT)
#include <csl_uart.h> 
#pragma CODE_SECTION(UART_getConfig,".text:UART_getConfig")
/******************************************************************************\
*                         L O C A L   S E C T I O N
\******************************************************************************/

/****************************************\
* UART global function definitions
\****************************************/
/*----------------------------------------------------------------------------*/
 void UART_getConfig(UART_Config *config) {
  int temp;
  int old_intm;
  old_intm = IRQ_globalDisable();  

  config->lcr = UART_RGET(URLCR);  
  
  temp = (config->lcr & _UART_URLCR_DLAB_MASK) >>_UART_URLCR_DLAB_SHIFT;  
  UART_FSET(URLCR, DLAB, 1);
  config->dll = UART_RGET(URDLL);  
  config->dlm = UART_RGET(URDLM);  
  UART_FSET(URLCR, DLAB, temp);


//Note: FCR is a write only register, when read back, it will be IIR.
//Only FIFO enable bits was obtained from IIR here
  config->fcr = UART_FGET(URIIR, FIENR);  
  config->mcr = UART_RGET(URMCR);  
 
  IRQ_globalRestore(old_intm);   
}
/*----------------------------------------------------------------------------*/

#endif /* UART_SUPPORT */
/******************************************************************************\
* End of uart_gcfga.c
\******************************************************************************/

