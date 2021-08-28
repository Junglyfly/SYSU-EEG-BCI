/******************************************************************************\
*           Copyright (C) 1999 Texas Instruments Incorporated.
*                           All Rights Reserved
*------------------------------------------------------------------------------
* MODULE.NAME... MCBSP
* FILENAME...... mcbsp_cfg.c
* DATE CREATED.. Fri 06/11/1999 
* PROJECT....... Chip Support Library
* COMPONENT..... 
* IMPORTS....... 
*------------------------------------------------------------------------------
* HISTORY:
*   CREATED:  06/11/1999 
*   MODIFIED: 01/25/2000 Modification for C54x DSP
*   MODIFIED: 02/23/2000 C54x ROMABILITY. 
*   MODIFIED: 06/02/2000 Modification for C55x DSP
*   MODIFIED: 06/29/2001 added pragma for section per function
*------------------------------------------------------------------------------
* DESCRIPTION:  (body file for the MCBSP module)
*
*
*
\******************************************************************************/
#define _MCBSP_MOD_

/****************************************\
* include files
\****************************************/

#include <csl_mcbsp.h>    

#if (_MCBSP_SUPPORT)
/******************************************************************************\
*                         L O C A L   S E C T I O N
\******************************************************************************/

/****************************************\
* MCBSP static macro declarations
\****************************************/



/* Map-register definition */
#define DRR1(hMcbsp)  PREG16(((MCBSP_PrivateObj*)hMcbsp)->Drr1Addr)
#define DRR2(hMcbsp)  PREG16(((MCBSP_PrivateObj*)hMcbsp)->Drr2Addr)
#define DXR1(hMcbsp)  PREG16(((MCBSP_PrivateObj*)hMcbsp)->Dxr1Addr)
#define DXR2(hMcbsp)  PREG16(((MCBSP_PrivateObj*)hMcbsp)->Dxr2Addr)
#define XMTEVENT(hMcbsp) (((MCBSP_PrivateObj*)hMcbsp)->XmtEvent)
#define RCVEVENT(hMcbsp) (((MCBSP_PrivateObj*)hMcbsp)->RcvEvent)
#define ALLOCATED(hMcbsp)(((MCBSP_PrivateObj*)hMcbsp)->Allocated)


/****************************************\
* MCBSP static typedef declarations
\****************************************/

/****************************************\
* MCBSP static function declarations
\****************************************/

/****************************************\
* MCBSP static variable definitions       
\****************************************/
 
 
  
/****************************************\
* MCBSP static function definitions
\****************************************/

/*----------------------------------------------------------------------------*/

/******************************************************************************\
*                        G L O B A L   S E C T I O N
\******************************************************************************/

/****************************************\
* MCBSP global variable definitions
\****************************************/

/****************************************\
* MCBSP global function definitions
\****************************************/

#pragma CODE_SECTION(MCBSP_config,".text:MCBSP_config")

/*----------------------------------------------------------------------------*/
void MCBSP_config(MCBSP_Handle hMcbsp, MCBSP_Config *Config) {
  int oldgie;
  MCBSP_ASSERT_HANDLE(hMcbsp,return);
  

  oldgie = IRQ_globalDisable();                       

  PREG16(((MCBSP_PrivateObj*)hMcbsp)->Spcr1Addr)=Config->spcr1;
  PREG16(((MCBSP_PrivateObj*)hMcbsp)->Spcr2Addr)=Config->spcr2;
  PREG16(((MCBSP_PrivateObj*)hMcbsp)->Rcr1Addr)=Config->rcr1;
  PREG16(((MCBSP_PrivateObj*)hMcbsp)->Rcr2Addr)=Config->rcr2;
  PREG16(((MCBSP_PrivateObj*)hMcbsp)->Xcr1Addr)=Config->xcr1;
  PREG16(((MCBSP_PrivateObj*)hMcbsp)->Xcr2Addr)=Config->xcr2;
  PREG16(((MCBSP_PrivateObj*)hMcbsp)->Srgr1Addr)=Config->srgr1;
  PREG16(((MCBSP_PrivateObj*)hMcbsp)->Srgr2Addr)=Config->srgr2;
  PREG16(((MCBSP_PrivateObj*)hMcbsp)->Mcr1Addr)=Config->mcr1;     
  PREG16(((MCBSP_PrivateObj*)hMcbsp)->Mcr2Addr)=Config->mcr2;
  PREG16(((MCBSP_PrivateObj*)hMcbsp)->RceraAddr)=Config->rcera;
  PREG16(((MCBSP_PrivateObj*)hMcbsp)->RcerbAddr)=Config->rcerb;
  PREG16(((MCBSP_PrivateObj*)hMcbsp)->RcercAddr)=Config->rcerc;
  PREG16(((MCBSP_PrivateObj*)hMcbsp)->RcerdAddr)=Config->rcerd;
  PREG16(((MCBSP_PrivateObj*)hMcbsp)->RcereAddr)=Config->rcere;
  PREG16(((MCBSP_PrivateObj*)hMcbsp)->RcerfAddr)=Config->rcerf;
  PREG16(((MCBSP_PrivateObj*)hMcbsp)->RcergAddr)=Config->rcerg;
  PREG16(((MCBSP_PrivateObj*)hMcbsp)->RcerhAddr)=Config->rcerh;
  PREG16(((MCBSP_PrivateObj*)hMcbsp)->XceraAddr)=Config->xcera;
  PREG16(((MCBSP_PrivateObj*)hMcbsp)->XcerbAddr)=Config->xcerb;
  PREG16(((MCBSP_PrivateObj*)hMcbsp)->XcercAddr)=Config->xcerc;
  PREG16(((MCBSP_PrivateObj*)hMcbsp)->XcerdAddr)=Config->xcerd;
  PREG16(((MCBSP_PrivateObj*)hMcbsp)->XcereAddr)=Config->xcere;
  PREG16(((MCBSP_PrivateObj*)hMcbsp)->XcerfAddr)=Config->xcerf;
  PREG16(((MCBSP_PrivateObj*)hMcbsp)->XcergAddr)=Config->xcerg;
  PREG16(((MCBSP_PrivateObj*)hMcbsp)->XcerhAddr)=Config->xcerh;  
  PREG16(((MCBSP_PrivateObj*)hMcbsp)->PcrAddr)=Config->pcr;  
 
  IRQ_globalRestore(oldgie);

  return;
}
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/

#endif /* MCBSP_SUPPORT */
/******************************************************************************\
* End of MBSP_cfg.c
\******************************************************************************/

