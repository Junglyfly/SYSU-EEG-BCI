/******************************************************************************\
*           Copyright (C) 2001 Texas Instruments Incorporated.
*                           All Rights Reserved
*------------------------------------------------------------------------------
* MODULE.NAME... UART
* FILENAME...... uart_read.c
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

#pragma CODE_SECTION(UART_read,".text:UART_read")
/******************************************************************************\
*                         L O C A L   S E C T I O N
\******************************************************************************/

/****************************************\
* UART global function definitions
\****************************************/
/*----------------------------------------------------------------------------*/
CSLBool UART_read(char *pBuf, Uint16 length, Uint32 timeout ) 
{
    int i, temp;
    
    UART_FSET(URLCR, DLAB, 0);

    if (timeout!=0)
    {      
        while( length-- )
        {
            for(i=0; i<=timeout; i++)
            {
                temp=UART_FGET(URLSR,DR);
                if(temp == 1) break;
            }
        
            if (i>=timeout)
                return FALSE;
            
            *pBuf++ = UART_RGET(URRBR);
        }     
    }
    else            
    {                   
        while( length-- )
        {
            /* wait for RX empty */
            while(!UART_FGET(URLSR,DR));
            *pBuf++ = UART_RGET(URRBR);
        }
    }    
    
    return TRUE;
}
/*----------------------------------------------------------------------------*/

#endif /* UART_SUPPORT */
/******************************************************************************\
* End of uart_read.c
\******************************************************************************/

