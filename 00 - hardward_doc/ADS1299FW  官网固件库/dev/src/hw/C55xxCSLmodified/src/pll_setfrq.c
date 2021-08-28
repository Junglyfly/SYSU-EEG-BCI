/******************************************************************************\
*           Copyright (C) 1999 Texas Instruments Incorporated.
*                           All Rights Reserved
*------------------------------------------------------------------------------
* MODULE NAME... PLL
* FILENAME...... pll_setfrq.c
* DATE CREATED.. 07/03/2001
* PROJECT....... Chip Support Library
* COMPONENT..... service layer
* PREREQUISITS.. 
*------------------------------------------------------------------------------
* HISTORY:
* CREATED:  07/03/2001 (RP) created for TMS320C5509
* MODIFIED: 08/04/2003 CLKOUTSR register added in and CLKMD0 renamed to CLKMD
* MODIFIED: 01/18/2004 Modified the direct register assignements to PLL_RSET(REG,val)
*------------------------------------------------------------------------------
* DESCRIPTION:  (interface file for the PLL module)
*
*
\******************************************************************************/
#define _PLL_MOD_

#include <csl_chiphal.h>

#if (_PLL_SUPPORT)

#include <csl_pll.h>

#if (_PLLA_SUPPORT)

#if (!(_PLL_ANALOG_SUPPORT))

void PLL_setFreq (Uint16 mul, Uint16 div)
{

	Uint16 pllenab = 1;	

	if (mul==1) pllenab = 0;					// select bypass or pll mode

										// write to clkmd register
	PLL_RSET(CLKMD, PLL_CLKMD_RMK(0,1,0,mul,div-1,pllenab,div-1)); 						


	if (pllenab==1)
	    while (!PLL_FGET(CLKMD,LOCK)) ;  // wait for lock

}

#else

void PLL_setFreq (Uint16 mul, Uint16 div)
{

	Uint16 plldiv, pllmul, pllenab = 1;
	int i = 0;

// decide pll or bypass mode and adjust pllmul

	plldiv = 3;				// pll fractional mode
	pllmul = mul-1;

	if ((mul==1) && (div==1)) // pll bypass
	{	plldiv = 0;	
		pllenab =0;		
	}
	else
		if (div==1) 
			plldiv = 2;		// pll integer mode [1-15]
	    else 
	    	if (div==4) 
				pllmul = mul;	// pll 1/4 mode

// write to CLKMD register

	PLL_RSET(CLKMD, PLL_CLKMD_RMK(1,pllmul,plldiv,pllenab)); 


	for (i=0; i<2000;i++)			// wait 2000 cycles for lock
 		asm(" nop" );

}

#endif

#endif /*PLLA_SUPPORT */

#if (_PLLB_SUPPORT)

#define wait_8_cycles {\
     Uint16 i;\
     for(i=0;i<=7;i++){\
       asm("\tNOP ; CSL Generated Code");\
     }\
}

void PLL_setFreq (Uint16 mode, Uint16 mul, Uint16 div0, Uint16 div1, Uint16 div2,
                  Uint16 div3, Uint16 oscdiv) {

  Uint16 i;

  // Make sure Fast Devices governed by Sysclk1 (Timer, ect)are
  // faster than Slow devices (Uart,MCBSP) governed by
  // Sysclk2

  if (div2 < div1) {
    div2 = div1;
  }
  
  // Make sure Fast Devices governed by Sysclk1 (Timer, ect)are
  // faster than EMIF governed Sysclk3
  if (div3 < div1) {
    div3 = div1;
  }

  // Place PLL in ByPass Mode
  PLL_FSET(PLLCSR,PLLEN,0);
  
  // Wait some cycles (ideally 4 of slowest clock, we just pick
  // arbitrary value here)
  for(i=0;i<20;i++) {
    asm("\tnop ;CSL generated code");
  }
    
  // Place PLL in Reset Mode
  PLL_FSET(PLLCSR,PLLRST,1);    

  // Configure PLL
     PLL_RSET(PLLDIV0 , _PLL_PLLDIV0_D0EN_MK(1) | div0);
     PLL_RSET(PLLM   , mul);
     PLL_RSET(OSCDIV1 , _PLL_OSCDIV1_OD1EN_MK(1) | oscdiv);

     PLL_RSET(PLLDIV1 , _PLL_PLLDIV1_D1EN_MK(1) | div1);
     wait_8_cycles;
     PLL_RSET(PLLDIV2 , _PLL_PLLDIV2_D2EN_MK(1) | div2);
     wait_8_cycles;
     PLL_RSET(PLLDIV3 , _PLL_PLLDIV3_D3EN_MK(1) | div3);

  // Wait for some time (should at least be 1micro sec)
  // the number used here is not a caluclated cycle count
  // but was chosen as generic value.
    for(i=0;i<=1000;i++){
     asm("\t nop  ; CSL generated code ");
    }

  // Release PLL from Reset
    PLL_FSET(PLLCSR,PLLRST,0);

  // Wait for Lock-Up & Oscillator Stable
    while (PLL_FGET(PLLCSR,LOCK) != 1){
    }
  
 // Set PLL mode

     PLL_FSET(PLLCSR,PLLEN,mode);

}

#endif /* PLLB_SUPPORT */
#endif /* PLL_SUPPORT */

/******************************************************************************\
* End of pll_setfrq.c
\******************************************************************************/
