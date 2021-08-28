/******************************************************************************\
*           Copyright (C) 1999 Texas Instruments Incorporated.
*                           All Rights Reserved
*------------------------------------------------------------------------------
* MODULE.NAME... MCBSP
* FILENAME...... MCBSP_reset.c
* DATE CREATED.. Fri 06/11/1999 
* PROJECT....... Chip Support Library
* COMPONENT..... 
* IMPORTS....... 
*------------------------------------------------------------------------------
* HISTORY:
*   CREATED:  06/11/1999 Created for C6x
*   MODIFIED: 01/25/2000 Modification for C54x DSP
*   MODIFIED: 02/23/2000 C54x ROMABILITY. 
*   MODIFIED: 06/02/2000 Modification for C55x DSP
*   MODIFIED: 06/28/2001 added pragma for section per function
*   MODIFIED: 09/06/2004 Added 5501 support
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
#include <csl.h>
#include <csl_irq.h> 
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

#pragma CODE_SECTION(MCBSP_reset,".text:MCBSP_reset")

/*----------------------------------------------------------------------------*/
void MCBSP_reset(MCBSP_Handle hMcbsp) {
  int oldgie;


  MCBSP_ASSERT_HANDLE(hMcbsp,return);
  oldgie = IRQ_globalDisable(); 
      
  if (hMcbsp == INV) {
    MCBSP_reset(MCBSP_HDEV0);
    MCBSP_reset(MCBSP_HDEV1);
    #if (!(CHIP_5501))
     MCBSP_reset(MCBSP_HDEV2);
    #endif
 
  } else {              
    _PREG_SET(((MCBSP_PrivateObj*)hMcbsp)->Spcr1Addr,MCBSP_SPCR1_DEFAULT) ; 
    _PREG_SET(((MCBSP_PrivateObj*)hMcbsp)->Spcr2Addr,MCBSP_SPCR2_DEFAULT) ;
    _PREG_SET(((MCBSP_PrivateObj*)hMcbsp)->Rcr1Addr,MCBSP_RCR1_DEFAULT);  
    _PREG_SET(((MCBSP_PrivateObj*)hMcbsp)->Rcr2Addr,MCBSP_RCR2_DEFAULT);  
    _PREG_SET(((MCBSP_PrivateObj*)hMcbsp)->Xcr1Addr,MCBSP_XCR1_DEFAULT);  
    _PREG_SET(((MCBSP_PrivateObj*)hMcbsp)->Xcr2Addr,MCBSP_XCR2_DEFAULT);  
    _PREG_SET(((MCBSP_PrivateObj*)hMcbsp)->Srgr1Addr,MCBSP_SRGR1_DEFAULT);  
    _PREG_SET(((MCBSP_PrivateObj*)hMcbsp)->Srgr2Addr,MCBSP_SRGR2_DEFAULT);   
    _PREG_SET(((MCBSP_PrivateObj*)hMcbsp)->Mcr1Addr,MCBSP_MCR1_DEFAULT);  
    _PREG_SET(((MCBSP_PrivateObj*)hMcbsp)->Mcr2Addr,MCBSP_MCR2_DEFAULT); 
    _PREG_SET(((MCBSP_PrivateObj*)hMcbsp)->RceraAddr,MCBSP_RCERA_DEFAULT);  
    _PREG_SET(((MCBSP_PrivateObj*)hMcbsp)->RcerbAddr,MCBSP_RCERB_DEFAULT);  
    _PREG_SET(((MCBSP_PrivateObj*)hMcbsp)->RcercAddr,MCBSP_RCERC_DEFAULT);  
    _PREG_SET(((MCBSP_PrivateObj*)hMcbsp)->RcerdAddr,MCBSP_RCERD_DEFAULT); 
    _PREG_SET(((MCBSP_PrivateObj*)hMcbsp)->RcereAddr,MCBSP_RCERE_DEFAULT);  
    _PREG_SET(((MCBSP_PrivateObj*)hMcbsp)->RcerfAddr,MCBSP_RCERF_DEFAULT); 
    _PREG_SET(((MCBSP_PrivateObj*)hMcbsp)->RcergAddr,MCBSP_RCERG_DEFAULT);  
    _PREG_SET(((MCBSP_PrivateObj*)hMcbsp)->RcerhAddr,MCBSP_RCERH_DEFAULT); 
    _PREG_SET(((MCBSP_PrivateObj*)hMcbsp)->XceraAddr,MCBSP_XCERA_DEFAULT);  
    _PREG_SET(((MCBSP_PrivateObj*)hMcbsp)->XcerbAddr,MCBSP_XCERB_DEFAULT);  
    _PREG_SET(((MCBSP_PrivateObj*)hMcbsp)->XcercAddr,MCBSP_XCERC_DEFAULT);  
    _PREG_SET(((MCBSP_PrivateObj*)hMcbsp)->XcerdAddr,MCBSP_XCERD_DEFAULT); 
    _PREG_SET(((MCBSP_PrivateObj*)hMcbsp)->XcereAddr,MCBSP_XCERE_DEFAULT);  
    _PREG_SET(((MCBSP_PrivateObj*)hMcbsp)->XcerfAddr,MCBSP_XCERF_DEFAULT); 
    _PREG_SET(((MCBSP_PrivateObj*)hMcbsp)->XcergAddr,MCBSP_XCERG_DEFAULT);  
    _PREG_SET(((MCBSP_PrivateObj*)hMcbsp)->XcerhAddr,MCBSP_XCERH_DEFAULT);      
    _PREG_SET(((MCBSP_PrivateObj*)hMcbsp)->PcrAddr,MCBSP_PCR_DEFAULT);     
    IRQ_disable(XMTEVENT(hMcbsp));    
    IRQ_disable(RCVEVENT(hMcbsp));
    IRQ_clear(XMTEVENT(hMcbsp));
    IRQ_clear(RCVEVENT(hMcbsp));
          
  }
  IRQ_globalRestore(oldgie);
     
  return;
}

/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/

#endif /* MCBSP_SUPPORT */
/******************************************************************************\
* End of MCBSP_reset.c
\******************************************************************************/

