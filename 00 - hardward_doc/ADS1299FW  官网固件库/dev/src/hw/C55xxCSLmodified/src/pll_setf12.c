/******************************************************************************\
*           Copyright (C) 1999 Texas Instruments Incorporated.
*                           All Rights Reserved
*------------------------------------------------------------------------------
* MODULE NAME... PLL
* FILENAME...... pll_setfreq12.c
* DATE CREATED.. 07/03/2001
* PROJECT....... Chip Support Library
* COMPONENT..... service layer
* PREREQUISITS.. 
*------------------------------------------------------------------------------
* HISTORY:
* 07/03/2001 (RP) created for TMS320C5510
*------------------------------------------------------------------------------
* DESCRIPTION:  (interface file for the PLL module)
*
* *********  C5510PG1.2 code   ********* 
*
*
\******************************************************************************/
#define _PLL_MOD_

#include <csl_pll.h>

#if (_PLL_SUPPORT) & (CHIP_5510PG1_2)


void PLL_setFreq12 (Uint16 mul, Uint16 div)
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


#endif /*PLL_SUPPORT */
/******************************************************************************\
* End of pll_setFreq12.c
\******************************************************************************/

