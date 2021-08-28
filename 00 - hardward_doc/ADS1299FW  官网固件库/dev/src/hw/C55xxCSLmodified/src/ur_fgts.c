/******************************************************************************\
*           Copyright (C) 2001 Texas Instruments Incorporated.
*                           All Rights Reserved
*------------------------------------------------------------------------------
* MODULE.NAME... UART
* FILENAME...... uart_fgts.c
* DATE CREATED.. 01/18/2002 
* PROJECT....... Chip Support Library
* COMPONENT..... 
* IMPORTS....... 
*------------------------------------------------------------------------------
* HISTORY:
*   01/18/2002   Created 
*------------------------------------------------------------------------------
* DESCRIPTION:  (body file for the UART module)
*  This routine reads a string from the uart. The string will be read upto  
*  a newline or until the buffer is filled. The string is always NULL       
*  terminated and does not have any newline character removed.              
*                                                                           
*  Parameters:                                                              
*      - pbuf - an area of memory to use for character storage              
*      - bufSize - the size of that memory area in characters (must include 
*                  space for the NULL terminator).                          
*                                                                           
*                                                                           
*  Return:                                                                  
*  - On Success, returns pointer to string                                  
*  - On Failure, returns NULL pointer                                       
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
#pragma CODE_SECTION(UART_fgets,".text:UART_fgets")
/******************************************************************************\
*                         L O C A L   S E C T I O N
\******************************************************************************/

/****************************************\
* UART global function definitions
\****************************************/
/*----------------------------------------------------------------------------*/
CSLBool UART_fgets(char* pBuf, Uint16 bufSize, Uint32 timeout)
{
  int cnt = bufSize;
  int temp, i;
  
  //DLAB=0
  UART_FSET(URLCR, DLAB, 0);

  if (timeout!=0)
  {
    while (cnt--)
    {
        for(i=0; i<=timeout; i++)
        {
            temp=UART_FGET(URLSR,DR);
            if(temp == 1) break;
        }
        
        if (i>=timeout)
            return FALSE;
            
        *pBuf = UART_RGET(URRBR);
        if ((*pBuf == '\n') ||
            (*pBuf == '\r'))
        {
            *(pBuf) = '\0';

			break;
        }
        pBuf++;
    }
  }
  else
  {
    while (cnt--)
    {
        //wait for RX buffer full
        while( !UART_FGET(URLSR,DR) );

        *pBuf = UART_RGET(URRBR);
        if ((*pBuf == '\n') ||
            (*pBuf == '\r'))
        {
            *(pBuf) = '\0';

			break;
        }
        pBuf++;
    }
  }
  *pBuf = '\0';

  return TRUE;
}
#endif /* UART_SUPPORT */
/******************************************************************************\
* End of uart_fgts.c
\******************************************************************************/
