/******************************************************************************\
*           Copyright (C) 1999 Texas Instruments Incorporated.
*                           All Rights Reserved
*------------------------------------------------------------------------------
* MODULE.NAME... I2C
* FILENAME...... i2c_receive.c
* DATE CREATED.. Sun 06/20/1999 
* PROJECT....... Chip Support Library
* COMPONENT..... 
* IMPORTS....... 
*------------------------------------------------------------------------------
* HISTORY:
*   CREATED:    08/02/2001
*   MODIFIED :  01/06/2003  I2C enhancement - reducing scope of global int. disable and enable
*------------------------------------------------------------------------------
* DESCRIPTION:  (I2C receive data)
*
*
*
\******************************************************************************/
#define _I2C_MOD_

#include <csl_chiphal.h>

#if (_I2C_SUPPORT)

#include <csl_i2c.h>

#pragma CODE_SECTION(I2C_read,".text:I2C_read")

int I2C_read(Uint16 *data,int length,int master,Uint16 slaveaddress,int transfermode,int timeout, int checkbus)  {

  int i,j,k,m;
  int old_intm;  
  Uint16 temp;
  
  //old_intm = IRQ_globalDisable();  
  
  /* set in receive mode */
  I2C_FSET(I2CMDR,TRX,0);
   
  if (master==1) {
   
  I2C_RSET(I2CSAR,slaveaddress); /* specify slave address */
  I2C_FSET(I2CMDR,MST,1);      /* turn master mode on */
  
  /* setting the transfer mode */

  if (transfermode==1) {   /* S-A-D..(n)..D-P mode */
  I2C_FSET(I2CMDR,RM,0);
  I2C_FSET(I2CMDR,STP,1);
  
  /* setting the data count in the ICCNT register */  
  I2C_RSET(I2CCNT,length);    
  }
  
  else if (transfermode==2) {   /* S-A-D..(n)..D mode  (repeat n times)*/
  I2C_FSET(I2CMDR,RM,0);
  I2C_FSET(I2CMDR,STP,0);
  }
  
  else if (transfermode==3) {   /* S-A-D-D-D ... (repeat continuous) mode */
  I2C_FSET(I2CMDR,RM,1);
  I2C_FSET(I2CMDR,STP,0);
  }
  
  else {                   /* if user specifies something else, go to idle mode */
  I2C_FSET(I2CMDR,RM,0);
  I2C_FSET(I2CMDR,STP,0);
  }
  
  }  /* end if master */
   
  else {
  
  I2C_FSET(I2CMDR,MST,0);  /* if not master, set to slave */
  
  }  /* end set up */ 
    
      
  if (checkbus==1) {
	  
  old_intm = IRQ_globalDisable(); 
  
  for (i=0;i<=timeout;i++)  {   /* check for bus busy */
  temp=I2C_FGET(I2CSTR,BB);
  if (temp==0) break; 
  }
  
  IRQ_globalRestore(old_intm);

  
  if (i>=timeout)  /* bus busy timeout error */
    return 1;  
  
  }  /* end of bus busy */
  
  
                          /* generate start condition */
    I2C_FSET(I2CMDR,STT,1);  
  
                           /* delay loop to receive data */
      for (i=0;i<32000;i++)  {
      asm (" NOP");
      }
       
  
  for (j=0;j<=timeout;j++)  {   /* check for DRR status */
  temp=I2C_FGET(I2CSTR,RRDY);
  if (temp==1) break; 
  }
    
  if (j>=timeout) 
  	return 2;         /* time out for receive DRR ready */ 
   
  
  *data++ = I2C_RGET(I2CDRR);  /* put first data value in DXR */
                 
  
  for (k=1;k<length;k++)  {   /* enter main loop for receiving data */
  
     
      for (i=0;i<32000;i++)  {
      asm (" NOP");
      }
     
    
  
  for (m=0;m<=timeout;m++)  {   /* check for DRR status */
  temp=I2C_FGET(I2CSTR,RRDY);
  if (temp==1) break; 
  }
    
  if (m>=timeout) 
  	return 4;         /* time out for transmit DRR ready */ 
  	
  *data++ = I2C_RGET(I2CDRR);  /* put next data value in DXR */
     
  
  } /* end of for loop */
  
                       /* last delay loop */
      for (i=0;i<32000;i++)  {
      asm (" NOP");
      }
                            
                                                      
 
  //IRQ_globalRestore(old_intm);
  
  return 0;
  
}  /* end of I2C_receive */

#endif
