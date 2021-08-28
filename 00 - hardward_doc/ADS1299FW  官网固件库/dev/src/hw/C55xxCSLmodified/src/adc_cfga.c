/****************************************************************************\
*           Copyright (C) 1999-2000 Texas Instruments Incorporated.
*                           All Rights Reserved 
*------------------------------------------------------------------------------
* MODULE NAME... ADC
* FILENAME...... adc_cfga.c
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

#pragma CODE_SECTION(ADC_configArgs,".text:ADC_configArgs")

void ADC_configArgs(Uint16 adcctl,Uint16 adcclkdiv,Uint16 adcclkctl) {

int old_intm;

  old_intm = IRQ_globalDisable();  
  ADC_RSET(ADCCTL,adcctl);
  ADC_RSET(ADCCLKDIV,adcclkdiv);	
  ADC_RSET(ADCCLKCTL,adcclkctl);
  IRQ_globalRestore(old_intm);   



}


#endif  /* _ADC_SUPPORT  */
/******************************************************************************\
*     
*      End of adc_cfga.c 
*
\******************************************************************************/
