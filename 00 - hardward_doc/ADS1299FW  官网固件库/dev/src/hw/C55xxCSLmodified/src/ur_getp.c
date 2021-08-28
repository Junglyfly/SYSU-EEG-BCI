/*****************************************************************************\
*           Copyright (C) 2000 Texas Instruments Incorporated.
*                           All Rights Reserved
*------------------------------------------------------------------------------
* MODULE.NAME... UART
* FILENAME...... UART_getParams.c
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
#pragma CODE_SECTION(UART_getSetup,".text:UART_getParams")
/*----------------------------------------------------------------------------*/
/* Function                                                                   */
/*----------------------------------------------------------------------------*/
void UART_getSetup(UART_Setup *params)
{
    Int16 old_intm;
    Uint16 temp1;
    old_intm = IRQ_globalDisable();  

    //DLAB = 1
    UART_FSET(URLCR,DLAB,1);
    temp1 = UART_RGET(URDLM) << UART_HIGHBYTE_SHIFT;
    params->baud = temp1 | (UART_HIGHBYTE_MASK & UART_RGET(URDLL));
    //DLAB = 0
    UART_FSET(URLCR,DLAB,0);

    temp1 = UART_RGET(URLCR);
    params->wordLength = temp1 & _UART_URLCR_WLS_MASK;
    params->stopBits = temp1 & _UART_URLCR_STB_MASK;
    params->parity = (temp1 & UART_PARITY_MASK);
        

    //FIFO control. 
    params->fifoControl = UART_FGET(URIIR, FIENR);    

    //Loopback control
    params->loopBackEnable = UART_RGET(URMCR) & _UART_URMCR_LOOP_MASK;    

    IRQ_globalRestore(old_intm);    
    return;
}
#endif /* UART_SUPPORT */
