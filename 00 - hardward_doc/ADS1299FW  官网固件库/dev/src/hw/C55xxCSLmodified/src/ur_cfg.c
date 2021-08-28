/******************************************************************************\
*           Copyright (C) 2001 Texas Instruments Incorporated.
*                           All Rights Reserved
*------------------------------------------------------------------------------
* MODULE.NAME... UART
* FILENAME...... uart_cfga.c
* DATE CREATED.. Sun 01/18/2002 
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
/******************************************************************************\
*                         L O C A L   S E C T I O N
\******************************************************************************/

#pragma CODE_SECTION(UART_config,".text:UART_config")
/****************************************\
* UART global function definitions
\****************************************/
/*----------------------------------------------------------------------------*/
 void UART_config(UART_Config *config) {
  int old_intm;
  old_intm = IRQ_globalDisable();  

  UART_FSET(URLCR, DLAB, 1);
  UART_RSET(URDLL, config->dll);  
  UART_RSET(URDLM, config->dlm);  
  
  UART_RSET(URLCR, config->lcr);  
  UART_RSET(URFCR, 0x7);  
  UART_RSET(URFCR, config->fcr);  
  UART_FSET(URMCR, LOOP, config->mcr);  

  //clear IER first to make sure UART work properly
  UART_FSET(URLCR, DLAB, 0);
  UART_RSET(URIER, UART_NOINT);
 
  IRQ_globalRestore(old_intm);   
}
/*----------------------------------------------------------------------------*/

#endif /* UART_SUPPORT */
/******************************************************************************\
* End of uart_cfga.c
\******************************************************************************/

