/******************************************************************************\
*           Copyright (C) 1999 Texas Instruments Incorporated.
*                           All Rights Reserved
*------------------------------------------------------------------------------
* MODULE.NAME... MCBSP
* FILENAME...... MCBSP_configB.c
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

#pragma CODE_SECTION(MCBSP_configArgs,".text:MCBSP_configArgs")

/*----------------------------------------------------------------------------*/
void MCBSP_configArgs(MCBSP_Handle hMcbsp,                        
                       Uint16 spcr1, Uint16 spcr2, 
                       Uint16 rcr1,  Uint16 rcr2, 
                       Uint16 xcr1,  Uint16 xcr2, 
                       Uint16 srgr1, Uint16 srgr2, 
                       Uint16 mcr1,  Uint16 mcr2, 
                       Uint16 pcr,   Uint16 rcera, 
                       Uint16 rcerb, Uint16 rcerc, 
                       Uint16 rcerd, Uint16 rcere, 
                       Uint16 rcerf, Uint16 rcerg, 
                       Uint16 rcerh, Uint16 xcera, 
                       Uint16 xcerb, Uint16 xcerc, 
                       Uint16 xcerd, Uint16 xcere, 
                       Uint16 xcerf, Uint16 xcerg, 
                       Uint16 xcerh 
                       ) {  
  
   int oldgie;
  MCBSP_ASSERT_HANDLE(hMcbsp,return);

  oldgie = IRQ_globalDisable(); 

  PREG16(((MCBSP_PrivateObj*)hMcbsp)->Spcr1Addr)=spcr1;
  PREG16(((MCBSP_PrivateObj*)hMcbsp)->Spcr2Addr)=spcr2;
  PREG16(((MCBSP_PrivateObj*)hMcbsp)->Rcr1Addr)=rcr1;
  PREG16(((MCBSP_PrivateObj*)hMcbsp)->Rcr2Addr)=rcr2;
  PREG16(((MCBSP_PrivateObj*)hMcbsp)->Xcr1Addr)=xcr1;
  PREG16(((MCBSP_PrivateObj*)hMcbsp)->Xcr2Addr)=xcr2;
  PREG16(((MCBSP_PrivateObj*)hMcbsp)->Srgr1Addr)=srgr1;
  PREG16(((MCBSP_PrivateObj*)hMcbsp)->Srgr2Addr)=srgr2;
  PREG16(((MCBSP_PrivateObj*)hMcbsp)->Mcr1Addr)=mcr1;     
  PREG16(((MCBSP_PrivateObj*)hMcbsp)->Mcr2Addr)=mcr2;
  PREG16(((MCBSP_PrivateObj*)hMcbsp)->RceraAddr)=rcera;
  PREG16(((MCBSP_PrivateObj*)hMcbsp)->RcerbAddr)=rcerb;
  PREG16(((MCBSP_PrivateObj*)hMcbsp)->RcercAddr)=rcerc;
  PREG16(((MCBSP_PrivateObj*)hMcbsp)->RcerdAddr)=rcerd;
  PREG16(((MCBSP_PrivateObj*)hMcbsp)->RcereAddr)=rcere;
  PREG16(((MCBSP_PrivateObj*)hMcbsp)->RcerfAddr)=rcerf;
  PREG16(((MCBSP_PrivateObj*)hMcbsp)->RcergAddr)=rcerg;
  PREG16(((MCBSP_PrivateObj*)hMcbsp)->RcerhAddr)=rcerh;
  PREG16(((MCBSP_PrivateObj*)hMcbsp)->XceraAddr)=xcera;
  PREG16(((MCBSP_PrivateObj*)hMcbsp)->XcerbAddr)=xcerb;
  PREG16(((MCBSP_PrivateObj*)hMcbsp)->XcercAddr)=xcerc;
  PREG16(((MCBSP_PrivateObj*)hMcbsp)->XcerdAddr)=xcerd;
  PREG16(((MCBSP_PrivateObj*)hMcbsp)->XcereAddr)=xcere;
  PREG16(((MCBSP_PrivateObj*)hMcbsp)->XcerfAddr)=xcerf;
  PREG16(((MCBSP_PrivateObj*)hMcbsp)->XcergAddr)=xcerg;
  PREG16(((MCBSP_PrivateObj*)hMcbsp)->XcerhAddr)=xcerh;  
  PREG16(((MCBSP_PrivateObj*)hMcbsp)->PcrAddr)=pcr;   
  IRQ_globalRestore(oldgie);

  return;
}      
 
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/

#endif /* MCBSP_SUPPORT */
/******************************************************************************\
 * End MCBSP_configB.c
\******************************************************************************/

