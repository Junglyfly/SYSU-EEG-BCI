/******************************************************************************\
*           Copyright (C) 1999 Texas Instruments Incorporated.
*                           All Rights Reserved
*------------------------------------------------------------------------------
* MODULE.NAME... MCBSP
* FILENAME...... MCBSP_open.c
* DATE CREATED.. Fri 06/11/1999
* PROJECT....... Chips Support Library
* COMPONENT.....
* IMPORTS.......
*------------------------------------------------------------------------------
* HISTORY:
*   CREATED:  06/11/1999 Created for C6x
*   MODIFIED: 01/25/2000 Modification for C54x DSP
*   MODIFIED: 02/23/2000 C54x ROMABILITY.
*   MODIFIED: 06/02/2000 Modification for C55x DSP
*   MODIFIED: 06/28/2001 added pragma for section per function
*   MODIFIED: 09/12/2003 Added Check for invalid port number
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

#pragma CODE_SECTION(MCBSP_open,".text:MCBSP_open")

/*---/MCBSP_Handle -------------------------------------------------------------------------*/
MCBSP_Handle MCBSP_open(int DevNum, Uint32 Flags) {
  int oldgie;
  Uint16 portMask = 1u<<DevNum;
  Uint16 port = DevNum;

  MCBSP_Handle McbspPtr = INV;

  #ifdef _MCRTE_DEBUG
    if ((DevNum<0) || (DevNum>=MCBSP_PORT_CNT)) {
      ERR_submit(MCBSP_ERR_MAJOR, MCBSP_ERR_ALLOC);
      return (MCBSP_Handle)INV;
    }
  #endif


  if ((DevNum<MCBSP_PORT_ANY) || (DevNum>=MCBSP_PORT_CNT)) {
      return (MCBSP_Handle)INV;
  }

 oldgie = IRQ_globalDisable();

  if (DevNum == MCBSP_PORT_ANY) {
    for(port=0; port<= MCBSP_PORT_CNT-1;port++) {
      if (!(CSL_SYS_DATA.McbspPortAllocMask & (1u<<port))) {
        McbspPtr = MCBSP_HDEV(port);
        break;
      }
    }
  }
  else {
    if (!(CSL_SYS_DATA.McbspPortAllocMask & portMask)) {
      McbspPtr = MCBSP_HDEV(port);
    }
  }

  if (McbspPtr != INV) {
     CSL_SYS_DATA.McbspPortAllocMask |= 1u<<port;

     #if (_MMC_SUPPORT)
     if (port) {
         Uint16 mask;
         CSL_SYS_DATA.MmcAllocMask |= 1u<<(port-1);
         mask = (port==1) ? (0xFFF3u) : (0xFFCFu);
         _XBSR &= mask;
     }
     #endif

     if (Flags&MCBSP_OPEN_RESET) {
       MCBSP_reset((MCBSP_Handle)McbspPtr);
     }
  }


  IRQ_globalRestore(oldgie);

  MCBSP_ASSERT_HANDLE((MCBSP_Handle)McbspPtr,return INV);

   return (MCBSP_Handle)McbspPtr;
}

/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/

#endif /* MCBSP_SUPPORT */
/******************************************************************************\
* End of MCBSP_open.c
\******************************************************************************/

