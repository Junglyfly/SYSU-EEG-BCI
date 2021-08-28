/******************************************************************************\
*           Copyright (C) 1999 Texas Instruments Incorporated.
*                           All Rights Reserved
*------------------------------------------------------------------------------
* MODULE NAME... HPI
* FILENAME...... hpi_cfg.c
* DATE CREATED.. Thur 03/21/2002
* PROJECT....... CSL - Chip Support Library
* COMPONENT..... service layer
* PREREQUISITS.. 
*------------------------------------------------------------------------------
* HISTORY:
*   CREATED:  03/21/2002
*------------------------------------------------------------------------------
* DESCRIPTION:  
\******************************************************************************/
#define _HPI_MOD_

#include <csl_chiphal.h>

#if (_HPI_SUPPORT)

#include <csl_hpi.h>

#pragma CODE_SECTION(HPI_config,".text:HPI_config")

/*----------------------------------------------------------------------------*/
/*------------------------------------------------------*/
void HPI_config(HPI_Config *Config) {
   ioport HPI_RegObj *rPtr = (ioport HPI_RegObj*)(_HPI_BASE_ADDR);
   Uint16 old_intm;

   old_intm = IRQ_globalDisable();
   
   rPtr->hpwremu = Config->hpwremu;
   rPtr->hgpioen = Config->hgpioen;
   rPtr->hgpiodir = Config->hgpiodir;
   rPtr->hpic = Config->hpic;

   IRQ_globalRestore(old_intm);
}
#endif /* HPI_SUPPORT */
/******************************************************************************\
* End of hpi_cfg.c
\******************************************************************************/
