/*****************************************************************************\
*           Copyright (C) 2000 Texas Instruments Incorporated.
*                           All Rights Reserved
*------------------------------------------------------------------------------
* MODULE.NAME... TIMER
* FILENAME...... uart_setCallback.c
* DATE CREATED.. Sun 01/18/2002
* PROJECT....... Chip Support Library
* COMPONENT.....
* IMPORTS.......
*------------------------------------------------------------------------------
* HISTORY:
*   01/18/2002   Created
*   09/11/2003   Modified  Checks the address validity in UART_intrDispatch.
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

#pragma CODE_SECTION(UART_setCallback,".text:UART_setCallback")
/*----------------------------------------------------------------------------*/
/* Function                                                                   */
/*----------------------------------------------------------------------------*/
void UART_setCallback(UART_IsrAddr *isrAddr)
{
//    UART_isrDispatchTable[UART_EVT_DSSI] = (Uint32) (isrAddr->dssiAddr);
    (CSL_UART_DATA).UART_isrDispatchTable[UART_EVT_TBEI] =
                  (Uint32) (isrAddr->tbeiAddr);
    (CSL_UART_DATA).UART_isrDispatchTable[UART_EVT_RBFI] =
                  (Uint32) (isrAddr->rbfiAddr);
    (CSL_UART_DATA).UART_isrDispatchTable[UART_EVT_LSI] =
                  (Uint32) (isrAddr->lsiAddr);
    (CSL_UART_DATA).UART_isrDispatchTable[UART_EVT_CTOI] =
                  (Uint32) (isrAddr->ctoiAddr);
    (CSL_UART_DATA).UART_isrDispatchTable[UART_EVT_DSSI] =
                  (Uint32) (isrAddr->dssiAddr);

    IRQ_plug(IRQ_EVT_UART, UART_intrDispatch);

}

/* Private function */
//interrupt void UART_intrDispatch(void)
//{
//    Uint16 EventId;
//
//    EventId = (UART_RGET(URIIR) & 0x0E) >> 1; // get IID bits
//
//    ((void (*)(void))((CSL_UART_DATA).UART_isrDispatchTable[EventId]))();
//
//    return;
//
//}
interrupt void UART_intrDispatch(void)
{
  Uint16 lcl_URIIR;
  Uint16 EventId;
// check for valid interrupt
    lcl_URIIR  = UART_RGET(URIIR);
    if ((lcl_URIIR & 0x01) == 1) ; //bit 0 = 1 means no interrupt pending
    { // decipher valid interrupt code and get table offset
    EventId = (lcl_URIIR & 0x0E) >> 1; // get IID bits
    // call indicated interrupt routine
     if (((void (*)(void))((CSL_UART_DATA).UART_isrDispatchTable[EventId]))) // check for valid address
        ((void (*)(void))((CSL_UART_DATA).UART_isrDispatchTable[EventId]))();
  }// endif

return;
}


#endif /* UART_SUPPORT */
