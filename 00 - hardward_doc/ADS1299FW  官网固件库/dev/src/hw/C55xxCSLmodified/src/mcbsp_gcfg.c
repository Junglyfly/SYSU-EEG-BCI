/******************************************************************************\
*           Copyright (C) 1999 Texas Instruments Incorporated.
*                           All Rights Reserved
*------------------------------------------------------------------------------
* MODULE.NAME... MCBSP
* FILENAME...... MCBSP_gcfg.c
* DATE CREATED.. Thu 05/10/2001
* PROJECT....... CSL - Chip Support Library
* COMPONENT..... 
* IMPORTS....... 
*------------------------------------------------------------------------------
* HISTORY:
*   CREATED:  05/10/2001
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

#pragma CODE_SECTION(MCBSP_getConfig,".text:MCBSP_getCOnfig")

/*----------------------------------------------------------------------------*/
void MCBSP_getConfig(MCBSP_Handle hMcbsp, MCBSP_Config *Config) {
  int oldgie;
  MCBSP_ASSERT_HANDLE(hMcbsp,return);
  

  oldgie = IRQ_globalDisable();                       

  Config->spcr1 = PREG16(((MCBSP_PrivateObj*)hMcbsp)->Spcr1Addr);
  Config->spcr2 = PREG16(((MCBSP_PrivateObj*)hMcbsp)->Spcr2Addr);
  Config->rcr1 = PREG16(((MCBSP_PrivateObj*)hMcbsp)->Rcr1Addr);
  Config->rcr2 = PREG16(((MCBSP_PrivateObj*)hMcbsp)->Rcr2Addr);
  Config->xcr1 = PREG16(((MCBSP_PrivateObj*)hMcbsp)->Xcr1Addr);
  Config->xcr2 = PREG16(((MCBSP_PrivateObj*)hMcbsp)->Xcr2Addr);
  Config->srgr1 = PREG16(((MCBSP_PrivateObj*)hMcbsp)->Srgr1Addr);
  Config->srgr2 = PREG16(((MCBSP_PrivateObj*)hMcbsp)->Srgr2Addr);
  Config->mcr1 = PREG16(((MCBSP_PrivateObj*)hMcbsp)->Mcr1Addr);     
  Config->mcr2 = PREG16(((MCBSP_PrivateObj*)hMcbsp)->Mcr2Addr);
  Config->rcera = PREG16(((MCBSP_PrivateObj*)hMcbsp)->RceraAddr);
  Config->rcerb = PREG16(((MCBSP_PrivateObj*)hMcbsp)->RcerbAddr);
  Config->rcerc = PREG16(((MCBSP_PrivateObj*)hMcbsp)->RcercAddr);
  Config->rcerd = PREG16(((MCBSP_PrivateObj*)hMcbsp)->RcerdAddr);
  Config->rcere = PREG16(((MCBSP_PrivateObj*)hMcbsp)->RcereAddr);
  Config->rcerf = PREG16(((MCBSP_PrivateObj*)hMcbsp)->RcerfAddr);
  Config->rcerg = PREG16(((MCBSP_PrivateObj*)hMcbsp)->RcergAddr);
  Config->rcerh = PREG16(((MCBSP_PrivateObj*)hMcbsp)->RcerhAddr);
  Config->xcera = PREG16(((MCBSP_PrivateObj*)hMcbsp)->XceraAddr);
  Config->xcerb = PREG16(((MCBSP_PrivateObj*)hMcbsp)->XcerbAddr);
  Config->xcerc = PREG16(((MCBSP_PrivateObj*)hMcbsp)->XcercAddr);
  Config->xcerd = PREG16(((MCBSP_PrivateObj*)hMcbsp)->XcerdAddr);
  Config->xcere = PREG16(((MCBSP_PrivateObj*)hMcbsp)->XcereAddr);
  Config->xcerf = PREG16(((MCBSP_PrivateObj*)hMcbsp)->XcerfAddr);
  Config->xcerg = PREG16(((MCBSP_PrivateObj*)hMcbsp)->XcergAddr);
  Config->xcerh = PREG16(((MCBSP_PrivateObj*)hMcbsp)->XcerhAddr);  
  Config->pcr = PREG16(((MCBSP_PrivateObj*)hMcbsp)->PcrAddr);  
 
  IRQ_globalRestore(oldgie);

  return;
}
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/

#endif /* MCBSP_SUPPORT */
/******************************************************************************\
* End of MCBSP_gcfg.c
\******************************************************************************/

