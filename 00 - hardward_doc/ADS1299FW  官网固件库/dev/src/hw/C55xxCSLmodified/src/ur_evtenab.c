/*****************************************************************************\
*           Copyright (C) 2000 Texas Instruments Incorporated.
*                           All Rights Reserved
*------------------------------------------------------------------------------
* MODULE.NAME... UART
* FILENAME...... UART_intrEnable.c
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
#pragma CODE_SECTION(UART_eventEnable,".text:UART_eventEnable")
/*----------------------------------------------------------------------------*/
/* Function                                                                   */
/*----------------------------------------------------------------------------*/
void UART_eventEnable(Uint16 isrMask)
{
        UART_FSET(URLCR,DLAB,0);
//        UART_ROR(URIER,isrMask); 
        UART_RAOI(URIER,0xFFFF, (isrMask), 0x0000);
        IRQ_enable(IRQ_EVT_UART);
}
#endif /* UART_SUPPORT */

