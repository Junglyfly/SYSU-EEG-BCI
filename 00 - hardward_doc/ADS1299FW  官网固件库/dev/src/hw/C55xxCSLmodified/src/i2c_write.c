/******************************************************************************\
*           Copyright (C) 1999 Texas Instruments Incorporated.
*                           All Rights Reserved
*------------------------------------------------------------------------------
* MODULE.NAME... I2C
* FILENAME...... i2c_write.c
* DATE CREATED.. Sun 06/20/1999 
* PROJECT....... Chip Support Library
* COMPONENT..... 
* IMPORTS....... 
*------------------------------------------------------------------------------
* HISTORY:
*   CREATED:    08/02/2001
*   MODIFIED :  01/06/2003  I2C enhancement - reducing scope of global int. disable and enable 
*------------------------------------------------------------------------------
* DESCRIPTION:  (I2C send data)
*
*
*
\******************************************************************************/
#define _I2C_MOD_


#include <csl_i2c.h>

#if (_I2C_SUPPORT)

#pragma CODE_SECTION(I2C_write,".text:I2C_write")

int I2C_write(Uint16 *data,int length,int master,Uint16 slaveaddress,int transfermode,int timeout)  {


  int i,j,k,l,m;
  int old_intm;  
  Uint16 temp=1;
  
  //old_intm = IRQ_globalDisable();  
  
  /* set in transmit mode */
  I2C_FSET(I2CMDR,TRX,1);
  
  I2C_RSET(I2CCNT,length);    /* set the count register */
   
  if (master==1) {
   
  I2C_RSET(I2CSAR,slaveaddress); /* specify slave address */
  I2C_FSET(I2CMDR,MST,1);      /* turn master mode on */
  
  /* setting the transfer mode */

  if (transfermode==1) {   /* S-A-D..(n)..D-P mode */
  I2C_FSET(I2CMDR,RM,0);
  I2C_FSET(I2CMDR,STP,1);
  
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

  old_intm = IRQ_globalDisable();      
  
  for (i=0;i<=timeout;i++)  {   /* check for bus busy */
  temp=I2C_FGET(I2CSTR,BB);
  if (temp==0) break; 
  }
  IRQ_globalRestore(old_intm);
  
  if (i>=timeout) {  /* bus busy timeout error */
    //IRQ_globalRestore(old_intm);
    return 1;
  }  
  
              
  for (m=0;m<=timeout;m++)  {   /* check for DXR status before first write*/

  temp=I2C_FGET(I2CSTR,XRDY);
  if (temp==1) break;                  
  }
          
  if (m>=timeout) {
    //IRQ_globalRestore(old_intm);
    return 2;         /* time out for transmit DXR ready */   
  }
    
  I2C_RSET(I2CDXR,*data++);  /* put first data value in DXR */
  
                            /* generate start condition */
  I2C_FSET(I2CMDR,STT,1);
             
  
  for (k=1;k<length;k++)  {   /* enter main loop for transmitting data */
        
       
      for (i=0;i<32000;i++)  {  /* delay loop */
      asm (" NOP");
      }
                     
  for (l=0;l<=timeout;l++)  {   /* check for NACK status */
  temp=I2C_FGET(I2CSTR,NACK);
  if (temp==0) break; 
  }
    
  if (l>=timeout) { 
     //IRQ_globalRestore(old_intm);
     return 3;        /* No acknowledge bit is set, NACK error */
  }

  for (m=0;m<=timeout;m++)  {   /* check for DXR status */

  temp=I2C_FGET(I2CSTR,XRDY);
  if (temp==1) break; 
                 
  }
      
  if (m>=timeout) {
      //IRQ_globalRestore(old_intm);
  	return 4;         /* time out for transmit DXR ready */ 
  }
	
  I2C_RSET(I2CDXR,*data++);  /* put next data value in DXR */
    
  
  } /* end of for loop */
   
   for (j=0;j<10;j++) {    /* last delay loop for last data transferred*/
      for (i=0;i<32000;i++)  {
      asm (" NOP");
      }
      }
     
    for (l=0;l<=timeout;l++)  {   /* check for NACK status for last data byte transferred*/
  temp=I2C_FGET(I2CSTR,NACK);
  if (temp==0) break; 
  }
    
  if (l>=timeout) {
      //IRQ_globalRestore(old_intm); 
  	return 5;        /* No acknowledge bit is set, NACK error */                                                             
  }
   
  //IRQ_globalRestore(old_intm);

  return 0;
  
}  /* end of I2C_send */

#endif
