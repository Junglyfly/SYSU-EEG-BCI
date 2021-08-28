/*****************************************************************************\
*           Copyright (C) 2000 Texas Instruments Incorporated.
*                           All Rights Reserved
*------------------------------------------------------------------------------
* MODULE.NAME... UART
* FILENAME...... UART_eventDisable.c
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
\*****************************************************************************/
#define _UART_MOD_

/*----------------------------------------------------------------------------*/
/* INCLUDES                                                                   */
/*----------------------------------------------------------------------------*/
#include <csl.h>
#include <csl_irq.h>

#if (_UART_SUPPORT)
#include <csl_uart.h>
#pragma CODE_SECTION(UART_eventDisable,".text:UART_eventDisable")
/*----------------------------------------------------------------------------*/
/* Function                                                                   */
/*----------------------------------------------------------------------------*/
void UART_eventDisable(Uint16 ierMask)
{
        UART_FSET(URLCR,DLAB,0);
//        UART_RAND(URIER,~ierMask);
        UART_RAOI(URIER,ierMask,0x0000,0x0000);
        if ( !(UART_RGET(URIER) & 0x0F) )
            IRQ_disable(IRQ_EVT_UART);
}
#endif /* UART_SUPPORT */

