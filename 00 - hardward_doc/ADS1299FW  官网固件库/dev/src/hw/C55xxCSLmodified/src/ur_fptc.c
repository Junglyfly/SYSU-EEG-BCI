/******************************************************************************\
*           Copyright (C) 2001 Texas Instruments Incorporated.
*                           All Rights Reserved
*------------------------------------------------------------------------------
* MODULE.NAME... UART
* FILENAME...... uart_fptc.c
* DATE CREATED.. 01/18/2002 
* PROJECT....... Chip Support Library
* COMPONENT..... 
* IMPORTS....... 
*------------------------------------------------------------------------------
* HISTORY:
*   01/18/2002   Created 
*------------------------------------------------------------------------------
* DESCRIPTION:  (body file for the UART module)
*  This routine writes a character to the uart.                             
*                                                                           
*  Parameters:                                                              
*      - c - The character, as an int, to be sent to the uart.              
*                                                                           
*  Return:                                                                  
*  - On Success, returns character written                                  
*  - On Failure, returns EOF                                                
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
#pragma CODE_SECTION(UART_fputc,".text:UART_fputc")
/******************************************************************************\
*                         L O C A L   S E C T I O N
\******************************************************************************/

/****************************************\
* UART global function definitions
\****************************************/
/*----------------------------------------------------------------------------*/
CSLBool UART_fputc(const int c, Uint32 timeout)
{
    int i, temp;
    
    //DLAB=0
    UART_FSET(URLCR, DLAB, 0);
    if (timeout!=0)                 /* check for timeout */
    {    
      for (i=0;i<=timeout;i++)  {   /* check for DR */
        temp=UART_FGET(URLSR,THRE);
        if (temp==1) break; 
      }
  
      if (i>=timeout)               /* timeout error */
        return FALSE;  
    }   
    else                            /* no timeout */
    {    
      //wait for TX buffer empty
      while( !UART_FGET(URLSR,THRE) );
    }

    UART_RSET(URTHR,c);
    
    return TRUE;
}
#endif /* UART_SUPPORT */
/******************************************************************************\
* End of uart_fptc.c
\******************************************************************************/
