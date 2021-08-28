/****************************************************************************\
*           Copyright (C) 1999-2000 Texas Instruments Incorporated.
*                           All Rights Reserved 
*------------------------------------------------------------------------------
* MODULE NAME... ADC
* FILENAME...... adc_gcfg.c
* DATE CREATED.. Fri Jun 29 15:39:48 2001
* PROJECT....... Chip Support Library
* COMPONENT..... Service Layer
* PREREQUISITS.. 
*------------------------------------------------------------------------------
*
* HISTORY:
*	 Created:		Fri Jun 29 15:39:48 2001 (Automatic generation)
*	 Last Modified:	
*
*------------------------------------------------------------------------------
* DESCRIPTION: CSL Service Layer interface for the ADC module 
*
\*************************************************************************/

#define _ADC_MOD_


#include <csl_chiphal.h>	 

#if (_ADC_SUPPORT) 

#include <csl_adc.h>

#pragma CODE_SECTION(ADC_getConfig,".text:ADC_getConfig")

void ADC_getConfig(ADC_Config *Config) {

  int old_intm;

  old_intm = IRQ_globalDisable();
  Config->adcctl = ADC_RGET(ADCCTL);
  Config->adcclkdiv = ADC_RGET(ADCCLKDIV);
  Config->adcclkctl = ADC_RGET(ADCCLKCTL);
  IRQ_globalRestore(old_intm);   


}

#endif  /* _ADC_SUPPORT  */
/******************************************************************************\
*     
*      End of adc_gcfg.c 
*
\******************************************************************************/
