/******************************************************************************\
*           Copyright (C) 1999 Texas Instruments Incorporated.
*                           All Rights Reserved
*------------------------------------------------------------------------------
* MODULE.NAME... ADC
* FILENAME...... adc_setfreq.c
* DATE CREATED.. Thu 08/02/2001
* PROJECT....... Chip Support Library
* COMPONENT..... 
* IMPORTS....... 
*------------------------------------------------------------------------------
* HISTORY:
*   CREATED:  08/02/2001
*------------------------------------------------------------------------------
* DESCRIPTION:  (Read data from Analog -to- Digital Converter)
*
*
*
\******************************************************************************/
#define _ADC_MOD_

#include <csl_adc.h>

#if (_ADC_SUPPORT)

#pragma CODE_SECTION(ADC_setFreq,".text:ADC_setFreq")

void ADC_setFreq(int sysclkdiv, int convratediv, int sampletimediv) {

int old_intm;

  old_intm = IRQ_globalDisable();  
  ADC_FSET(ADCCLKCTL,CPUCLKDIV,sysclkdiv);
  ADC_FSET(ADCCLKDIV,CONVRATEDIV,convratediv);
  ADC_FSET(ADCCLKDIV,SAMPTIMEDIV,sampletimediv);
  IRQ_globalRestore(old_intm);   


}

#endif //_ADC_SUPPORT

