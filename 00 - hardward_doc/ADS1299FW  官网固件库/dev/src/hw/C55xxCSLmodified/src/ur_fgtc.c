/******************************************************************************\
*           Copyright (C) 2001 Texas Instruments Incorporated.
*                           All Rights Reserved
*------------------------------------------------------------------------------
* MODULE.NAME... UART
* FILENAME...... uart_fgtc.c
* DATE CREATED.. 01/18/2002 
* PROJECT....... Chip Support Library
* COMPONENT..... 
* IMPORTS....... 
*------------------------------------------------------------------------------
* HISTORY:
*   01/18/2002   Created 
*------------------------------------------------------------------------------
* DESCRIPTION:  (body file for the UART module)
*  This routine reads a character from the uart.                            
*                                                                           
*  Parameters:                                                              
*      - None                                                               
*                                                                           
*  Return:                                                                  
*  - If character available, returns character as non-sign extended int     
*  - If no character available, returns EOF                                 
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
#pragma CODE_SECTION(UART_fgetc,".text:UART_fgetc")
/******************************************************************************\
*                         L O C A L   S E C T I O N
\******************************************************************************/

/****************************************\
* UART global function definitions
\****************************************/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

CSLBool UART_fgetc(int *c, Uint32 timeout)
{
    Uint16 i = 0;
    Uint16 temp;

    //DLAB=0
    UART_FSET(URLCR, DLAB, 0);

    if (timeout!=0)                 /* check for timeout */
    {    
      for (i=0;i<=timeout;i++)  {   /* check for DR */
        temp=UART_FGET(URLSR,DR);
        if (temp==1) break; 
      }
  
      if (i>=timeout)               /* timeout error */
        return FALSE;  
    }   
    else                            /* no timeout */
    {
      while( !UART_FGET(URLSR,DR) );
    } 
 
    *c = UART_RGET(URRBR);
    return TRUE;
    
}
#endif /* UART_SUPPORT */
/******************************************************************************\
* End of uart_fgtc.c
\******************************************************************************/
