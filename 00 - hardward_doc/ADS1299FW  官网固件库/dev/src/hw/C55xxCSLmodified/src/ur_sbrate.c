/*****************************************************************************\
*           Copyright (C) 2000 Texas Instruments Incorporated.
*                           All Rights Reserved
*------------------------------------------------------------------------------
* MODULE.NAME... UART
* FILENAME...... UART_setupBaudRate.c
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

#include <csl.h>

/*----------------------------------------------------------------------------*/
/* INCLUDES                                                                   */
/*----------------------------------------------------------------------------*/
#if (_UART_SUPPORT)
#include <csl_uart.h>

#pragma CODE_SECTION(UART_setupBaudRate,".text:UART_setupBaudRate")

/*----------------------------------------------------------------------------*/
/* Function                                                                   */
/*----------------------------------------------------------------------------*/
void UART_setupBaudRate(Uint16 clkInput, Uint16 baudRate)
{
    Uint16 tempbaud;

    //----------------------------BAUD RATE SETUP----------------------------
    
    //Symbolic constants for baud rates in csl_uart.h are for 64MHz UART CLK
    //For any other UART CLK= c MHz, the formula for baud rate is:
    //  (c * baud_rate_for_64MHz) / 64
    
    tempbaud = (Uint16)(((Uint32)clkInput * (Uint32)baudRate) >> 6);

    //DLAB = 1
    UART_FSET(URLCR, DLAB, 1);
    UART_RSET(URDLL, (tempbaud & 0xFF));
    UART_RSET(URDLM, (tempbaud >> 8));
    //DLAB = 0
    UART_FSET(URLCR, DLAB, 0);    
   
    return;
}
#endif /* _UART_SUPPORT */

