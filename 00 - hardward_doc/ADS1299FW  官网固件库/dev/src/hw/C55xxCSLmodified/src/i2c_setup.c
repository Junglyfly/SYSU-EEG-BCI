/******************************************************************************\
*           Copyright (C) 1999 Texas Instruments Incorporated.
*                           All Rights Reserved
*------------------------------------------------------------------------------
* MODULE.NAME... I2C
* FILENAME...... i2c_setup.c
* DATE CREATED.. Sun 06/20/1999 
* PROJECT....... Chip Support Library
* COMPONENT..... 
* IMPORTS....... 
*------------------------------------------------------------------------------
* HISTORY:
*   CREATED:  08/02/2001
*   MODIFIED: 04/04/2002 CHanged name of function to I2C_setup
*------------------------------------------------------------------------------
* DESCRIPTION:  (I2C initialization)
*
*
*
\******************************************************************************/
#define _I2C_MOD_

#include <csl_chiphal.h>

#if (_I2C_SUPPORT)

#include <csl_i2c.h>

#pragma CODE_SECTION(I2C_setup,".text:I2C_setup")

void I2C_setup(I2C_Setup *Init) {

  int old_intm;
  Uint16 IPSC_calc;

  old_intm = IRQ_globalDisable();  
  
  
  
  I2C_RSET(I2CMDR,I2C_I2CMDR_RMK(Init->free,0,0,0,1,1,Init->addrmode,0,Init->dlb,1,0,0,Init->bitbyte));
    
/* set own address */  

  I2C_RSET(I2COAR,Init->ownaddr); /* if slave, need to specify own address */
  
/* calculating the IPSC value */
   	
  IPSC_calc = (Init->sysinclock)/12;  /* must correct rounding issue */
  I2C_RSET(I2CPSC,IPSC_calc);
  
/* calculating the ICCLKL and ICCLKH register values */ 
  
  I2C_RSET(I2CCLKL,15);
  I2C_RSET(I2CCLKH,15);    
  
  IRQ_globalRestore(old_intm);   

}   /* end of init  */
#endif

