/******************************************************************************\
*           Copyright (C) 1999 Texas Instruments Incorporated.
*                           All Rights Reserved
*------------------------------------------------------------------------------
* MODULE NAME... PLL
* FILENAME...... pll_cfg.c
* DATE CREATED.. Wed 06/06/2000
* PROJECT....... CSL - Chip Support Library
* COMPONENT..... service layer
* PREREQUISITS..
*------------------------------------------------------------------------------
* HISTORY:
*   CREATED:  06/06/2000 created
*   MODIFIED: 04/17/2001 updated for 5510 PG2.0 support
*   MODIFIED: 06/28/2001 added pragma for section per function
*   MODIFIED: 01/28/2002 added PLL support for 5502
*   MODIFIED: 08/04/2003 Added CLKOUTSR register for 5502 PLL config sequence
*   MODIFIED: 09/25/2003 Renamed WAKEUP->WKEN
*   MODIFIED: 01/18/2004 Modified the direct register assignements to PLL_RSET(REG,val) 
*------------------------------------------------------------------------------
* DESCRIPTION:  (interface file for the PLL module)
\******************************************************************************/
#define _PLL_MOD_

#include <csl_emif.h>
#include <csl_pll.h>

#if (_PLLA_SUPPORT)

#pragma CODE_SECTION(PLL_config,".text:PLL_config")

/*----------------------------------------------------------------------------*/
void PLL_config(PLL_Config *pcfg) {
  Uint16  pllmult = pcfg->pllmult;
  Uint16  pllenab = 1;		//initialize to PLL mode

#if (_PLL_DIGITAL_SUPPORT)	//Digital PLL (5509)
  Uint16  iai = pcfg->iai;
  Uint16  iob = pcfg->iob;
  Uint16  plldiv = pcfg->div;
#else						//Analog PLL
  Uint16 vcoonoff = pcfg->vcoonoff;
  Uint16  plldiv = pcfg->plldiv;
  int i = 0;
#endif

  if (pllmult==1)
  	pllenab = 0;		// select bypass mode if the multiplier is 1

/*-------------------------------------------------------------*\
 * Place PLL in BYPASS mode
\*-------------------------------------------------------------*/

  PLL_FSET(CLKMD,PLLENABLE,0);


/*-------------------------------------------------------------*\
 * Check status to make use we are in BYPASS mode (LOCK != 0)
\*-------------------------------------------------------------*/

#if (_PLL_DIGITAL_SUPPORT)      //(CHIP_5509)Digital PLL
  while(PLL_FGET(CLKMD,LOCK));
#endif

/*-------------------------------------------------------------*\
 * Reconfigure CLKMD
\*-------------------------------------------------------------*/

#if (_PLL_DIGITAL_SUPPORT)  //(CHIP_5509 & CHIP_PG2_0)  Digital PLL
  _PLL_CLKMD_CFG(iai,iob,0,pllmult,plldiv,pllenab,plldiv);
  //val = PLL_CLKMD_RMK(iai,iob,0,pllmult,plldiv,pllenab,plldiv);
  //PLL_RSET(CLKMD, val);

#else
  _PLL_CLKMD_CFG(vcoonoff,pllmult,plldiv,pllenab);
#endif

/*-------------------------------------------------------------*\
 * Wait for PLL lock-up, LOCK == 1
\*-------------------------------------------------------------*/

#if (_PLL_DIGITAL_SUPPORT)   //(CHIP_5509)	//Digital PLL
	if (pllenab==1)
		while(!(PLL_FGET(CLKMD,LOCK)));
#else
	for (i=0; i<2000;i++)			// wait 2000 cycles for lock
 		asm(" nop" );

#endif
}

#endif /*PLLA_SUPPORT */

#if (_PLLB_SUPPORT)

#define wait_8_cycles {\
   Uint16 i;\
   for(i=0;i<=7;i++) {\
     asm("\tNOP ; CSL Generated Code");\
   }\
}

#pragma CODE_SECTION(PLL_config,".text:PLL_config")

/*----------------------------------------------------------------------------*/
void PLL_config(PLL_Config *pcfg) {

  Uint16 i;

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
     PLL_RSET(CLKMD,pcfg->clkmd);
     PLL_RSET(PLLDIV0,pcfg->plldiv0);
     PLL_RSET(PLLM, pcfg->pllm);
     PLL_RSET(OSCDIV1,pcfg->oscdiv1);

     PLL_RSET(PLLDIV3,pcfg->plldiv3);
	 wait_8_cycles;
     PLL_RSET(PLLDIV2,pcfg->plldiv2);
	 wait_8_cycles;
     PLL_RSET(PLLDIV1,pcfg->plldiv1);

     PLL_RSET(CK3SEL,PLL_CK3SEL_CK3SEL_PLL);
     PLL_RSET(WKEN, pcfg->wken);

  // Write to PLL control register making sure PLL
  // reset is not being written
     PLL_RSET(PLLCSR,(pcfg->pllcsr) & (~(0x80u)));

  // Wait for some time (should at least be 1ms)
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

  // Put PLL in PLL mode
   PLL_FSET(PLLCSR,PLLEN,1);

   PLL_RSET(CLKOUTSR,pcfg->clkoutsr);

}
#endif    //PLLB_SUPPORT

/******************************************************************************\
* End of pll_cfg.c
\******************************************************************************/


