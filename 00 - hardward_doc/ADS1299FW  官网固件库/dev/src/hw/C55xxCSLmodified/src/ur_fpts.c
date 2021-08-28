/******************************************************************************\
*           Copyright (C) 2001 Texas Instruments Incorporated.
*                           All Rights Reserved
*------------------------------------------------------------------------------
* MODULE.NAME... UART
* FILENAME...... uart_fpts.c
* DATE CREATED.. Sun 01/18/2002 
* PROJECT....... Chip Support Library
* COMPONENT..... 
* IMPORTS....... 
*------------------------------------------------------------------------------
* HISTORY:
*   01/18/2002   Created 
*------------------------------------------------------------------------------
* DESCRIPTION:  (body file for the UART module)
*  This routine writes a string to the uart. The NULL terminator is not     
*  written and a newline is not added to the output.                        
*                                                                           
*  Parameters:                                                              
*      - pbuf - an area of memory containing the string to be written.      
*                                                                           
*                                                                           
*  Return:                                                                  
*  - On Success, returns 0                                                  
*  - On Failure, returns non-0                                              
*                                                                           
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
#pragma CODE_SECTION(UART_fputs,".text:UART_fputs")
/******************************************************************************\
*                         L O C A L   S E C T I O N
\******************************************************************************/

/****************************************\
* UART global function definitions
\****************************************/
/*----------------------------------------------------------------------------*/
CSLBool UART_fputs(const char* pBuf, Uint32 timeout)
{
    int temp, i;

    //DLAB=0
    UART_FSET(URLCR, DLAB, 0);
    
  if (timeout!=0)
  {
    while (*pBuf != '\0')
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
    while (*pBuf != '\0')
    {
        //wait for TX buffer empty
        while( !UART_FGET(URLSR,THRE) );
        UART_RSET(URTHR, *pBuf++);
    }
  }    
  return TRUE;
}
#endif /* UART_SUPPORT */
/******************************************************************************\
* End of uart_fpts.c
\******************************************************************************/
