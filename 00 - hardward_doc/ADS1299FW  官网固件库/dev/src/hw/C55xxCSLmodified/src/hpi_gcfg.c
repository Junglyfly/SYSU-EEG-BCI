/******************************************************************************\
*           Copyright (C) 2003 Texas Instruments Incorporated.
*                           All Rights Reserved
*------------------------------------------------------------------------------
* MODULE NAME... HPI
* FILENAME...... hpi_gcfg.c
* DATE CREATED.. WED 04/23/2003
* PROJECT....... CSL - Chip Support Library
* COMPONENT..... service layer
* PREREQUISITS.. 
*------------------------------------------------------------------------------
* HISTORY:
*   CREATED:  04/24/2003
*------------------------------------------------------------------------------
* DESCRIPTION:  
\******************************************************************************/
#define _HPI_MOD_

#include <csl_chiphal.h>

#if (_HPI_SUPPORT)

#include <csl_hpihal.h>
#include <csl_hpi.h>

#pragma CODE_SECTION(HPI_getConfig,".text:HPI_getConfig")

/*----------------------------------------------------------------------------*/
/*------------------------------------------------------*/
void HPI_getConfig(HPI_Config *Config) {
  Uint16 old_intm;
  ioport HPI_RegObj *rPtr = (ioport HPI_RegObj*)(_HPI_BASE_ADDR);
  
   old_intm = IRQ_globalDisable();
   
   Config->hpwremu = rPtr->hpwremu ;
   Config->hgpioen = rPtr->hgpioen;
   Config->hgpiodir = rPtr->hgpiodir;
   Config->hpic = rPtr->hpic ;
   IRQ_globalRestore(old_intm);
    
}
/*----------------------------------------------------------------------------*/

#endif /* HPI_SUPPORT */
/******************************************************************************\
* End of hpi_gcfg.c
\******************************************************************************/

