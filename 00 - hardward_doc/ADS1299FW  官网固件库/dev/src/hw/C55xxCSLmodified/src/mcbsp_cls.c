/******************************************************************************\
*           Copyright (C) 1999 Texas Instruments Incorporated.
*                           All Rights Reserved
*------------------------------------------------------------------------------
* MODULE.NAME... MCBSP
* FILENAME...... MCBSP_close.c
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
*                        and updated for new CSL initialization model
*   MODIFIED: 10/06/2003 Fixed the typo in _MMC_SUPPORT
*
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

#if (_MMC_SUPPORT)
  #include <csl_chip.h>
#endif

#if (_MCBSP_SUPPORT)
/******************************************************************************\
*                         L O C A L   S E C T I O N
\******************************************************************************/

/****************************************\
* MCBSP static macro declarations
\****************************************/

 
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

#pragma CODE_SECTION(MCBSP_close,".text:MCBSP_close")

/*----------------------------------------------------------------------------*/
void MCBSP_close(MCBSP_Handle hMcbsp) {
  int oldgie;
  Uint16 port;

  MCBSP_ASSERT_HANDLE(hMcbsp,return);

  oldgie = IRQ_globalDisable(); 
  port = ((MCBSP_PrivateObj*)hMcbsp)->Port;
  MCBSP_reset(hMcbsp);
  CSL_SYS_DATA.McbspPortAllocMask &= (~(1u<<port));
  #if (_MMC_SUPPORT)
  if (port)
    CSL_SYS_DATA.MmcAllocMask &= (~(1u<<(port-1)));
  #endif
  IRQ_globalRestore(oldgie);


  return;
}
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/

#endif /* MCBSP_SUPPORT */
/******************************************************************************\
* End of MCBSP_close.c
\******************************************************************************/

