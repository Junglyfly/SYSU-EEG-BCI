/******************************************************************************\
*           Copyright (C) 1999 Texas Instruments Incorporated.
*                           All Rights Reserved
*------------------------------------------------------------------------------
* MODULE.NAME... ADC
* FILENAME...... adc_read.c
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

#pragma CODE_SECTION(ADC_read,".text:ADC_read")

void ADC_read(int channelnumber,Uint16 *data,int length) {

  int old_intm,i;
  
  old_intm = IRQ_globalDisable();
  ADC_FSET(ADCCTL,CHSELECT,channelnumber);  /* set channel number */
  
  for (i=0;i<length;i++) {
  
  ADC_FSET(ADCCTL,ADCSTART,1);   /* start conversion */
  while (ADC_FGET(ADCDATA,ADCBUSY)) {  /* loop until not busy */
    asm(" NOP");
  }
  
  *data++ = (ADC_FGET(ADCDATA,ADCDATA)&0x0FFE);  /* mask LSB (due to noise) */
  
  }  /* end of main loop */
  
  IRQ_globalRestore(old_intm);
  
  
}
#endif //_ADC_SUPPORT
