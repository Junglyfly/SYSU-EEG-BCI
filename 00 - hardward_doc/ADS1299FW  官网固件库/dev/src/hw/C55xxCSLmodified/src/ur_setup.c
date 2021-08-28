/*****************************************************************************\
*           Copyright (C) 2000 Texas Instruments Incorporated.
*                           All Rights Reserved
*------------------------------------------------------------------------------
* MODULE.NAME... UART
* FILENAME...... UART_setup.c
* DATE CREATED.. Sun 01/18/2002 
* PROJECT....... Chip Support Library
* COMPONENT..... 
* IMPORTS....... 
*------------------------------------------------------------------------------
* HISTORY:
*   01/18/2002   Created 
*   MODIFIED:    09/06/2004   Added 5501 support
*------------------------------------------------------------------------------
* DESCRIPTION:  (body file for the UART module)
*
*
*
\*****************************************************************************/
#define _UART_MOD_

/*----------------------------------------------------------------------------*/
/* INCLUDES                                                                   */
/*----------------------------------------------------------------------------*/
#include <csl.h>
#include <csl_irq.h>

#if (_UART_SUPPORT)
#include <csl_uart.h>

#pragma CODE_SECTION(UART_setup,".text:UART_setup")
/*----------------------------------------------------------------------------*/
/* Function                                                                   */
/*----------------------------------------------------------------------------*/
void UART_setup(UART_Setup *params)
{

    #if(!(CHIP_5501))
      /* Set BSR to disable SP2MODE */
      CHIP_FSET(XBSR, SP2MODE,0) ;
    #endif 
    /* disable all UART events */
    UART_FSET(URLCR,DLAB,0);
    UART_RSET(URIER, UART_NOINT);

    /* reset and possibly enable FIFOs */
    UART_RSET(URFCR, 0x7);
    UART_RSET(URFCR, params->fifoControl); 

    /* set DLL and DLM to values appropriate for the required baudrate */
    UART_setupBaudRate (params->clkInput, params->baud);

    
    /* setup word size, stop bits, parity */
    UART_RSET(URLCR, params->wordLength | params->stopBits | params->parity);

    
    /* setup loopback control */
    UART_RSET(URMCR, params->loopBackEnable);            

    
    /* UART out of reset */
    UART_resetOff();
    
    
    return;
}
#endif /* UART_SUPPORT */
