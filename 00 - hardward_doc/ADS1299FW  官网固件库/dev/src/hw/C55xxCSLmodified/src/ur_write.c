/******************************************************************************\
*           Copyright (C) 2001 Texas Instruments Incorporated.
*                           All Rights Reserved
*------------------------------------------------------------------------------
* MODULE.NAME... UART
* FILENAME...... uart_write.c
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

#pragma CODE_SECTION(UART_write,".text:UART_write")
/******************************************************************************\
*                         L O C A L   S E C T I O N
\******************************************************************************/

/****************************************\
* UART global function definitions
\****************************************/
/*----------------------------------------------------------------------------*/
CSLBool UART_write(char *pBuf, Uint16 length, Uint32 timeout) 
{
    int temp, i;

    UART_FSET(URLCR, DLAB, 0);
  
  if (timeout!=0)
  {
    while( length-- )
    {
        for(i=0; i<=timeout; i++)
        {
            temp=UART_FGET(URLSR,THRE);
            if(temp == 1) break;
        }
        
        if (i>=timeout)
            return FALSE;

        UART_RSET(URTHR, *pBuf++);
    }
  }
  else
  {    
    
    while( length-- )
    {
        /* wait for TX empty */
        while( !UART_FGET(URLSR,THRE) );
        UART_RSET(URTHR, *pBuf++);
    }
  }
  return TRUE;
}
/*----------------------------------------------------------------------------*/

#endif /* UART_SUPPORT */
/******************************************************************************\
* End of uart_write.c
\******************************************************************************/

