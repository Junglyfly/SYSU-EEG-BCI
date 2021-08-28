/******************************************************************************\
*           Copyright (C) 2001 Texas Instruments Incorporated.
*                           All Rights Reserved
*------------------------------------------------------------------------------
* MODULE.NAME... UART
* FILENAME...... uart_cfgb.c
* DATE CREATED.. 01/18/2002 
* PROJECT....... Chip Support Library
* COMPONENT..... 
* IMPORTS....... 
*------------------------------------------------------------------------------
* HISTORY:
*   02/18/2002   Created 
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
#pragma CODE_SECTION(UART_configArgs,".text:UART_configArgs")

/******************************************************************************\
*                         L O C A L   S E C T I O N
\******************************************************************************/

/****************************************\
* UART global function definitions
\****************************************/
/*----------------------------------------------------------------------------*/
 void UART_configArgs(Uint16 dll, Uint16 dlm, Uint16 lcr, Uint16 fcr, Uint16 mcr) {
  int old_intm;
  old_intm = IRQ_globalDisable();  

  UART_FSET(URLCR, DLAB, 1);  
  UART_RSET(URDLL, dll);  
  UART_RSET(URDLM, dlm);
    
  UART_RSET(URLCR, lcr);  
  UART_RSET(URFCR, 0x7);  
  UART_RSET(URFCR, fcr);  
  UART_RSET(URMCR, mcr);  

  //clear IER first to make sure UART work properly
  UART_FSET(URLCR, DLAB, 0);
  UART_RSET(URIER, UART_NOINT);
 
  IRQ_globalRestore(old_intm);   
}
/*----------------------------------------------------------------------------*/

#endif /* UART_SUPPORT */
/******************************************************************************\
* End of uart_cfgb.c
\******************************************************************************/

