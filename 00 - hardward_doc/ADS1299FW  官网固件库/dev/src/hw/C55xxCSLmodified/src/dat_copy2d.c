/******************************************************************************\
*           Copyright (C) 1999 Texas Instruments Incorporated.
*                           All Rights Reserved
*------------------------------------------------------------------------------
* MODULE.NAME... DAT
* FILENAME...... DAT_copy2D.c
* DATE CREATED.. Thu 11/11/1999
* PROJECT....... Chip Support Library
* COMPONENT..... CSL service layer
* IMPORTS.......
*------------------------------------------------------------------------------
* HISTORY:
*   CREATED:  11/11/1999
*   MODIFIED: 07/10/2000 Modified for C55xx
*   MODIFIED: 06/29/2001 added pragma for section per function and updated
*                        for new CSL initialization/data model
*	      09/16/2003 Changed src and dst address generation expression, Added missing
*	    		 DMA control registers assignment
*             10/06/2003 Added CHIP_5502 type for CSDP register make.
*             01/19/2003 Added WP field to DMACCR register on 5502
*             08/06/2004 Added 5501 support
*             08/24/2004 Fixed interrupt restore issues
*             09/15/2004 Added AERRIE field to CICR register
*------------------------------------------------------------------------------
* DESCRIPTION:  (C body file for the DAT module)
*
*
*
\******************************************************************************/
#define _DAT_MOD_

/****************************************\
* include files
\****************************************/

#include <csl_dat.h>

#if (_DAT_SUPPORT)
/******************************************************************************\
*                         L O C A L   S E C T I O N
\******************************************************************************/

/****************************************\
* DAT static macro declarations
\****************************************/

/****************************************\
* DAT static typedef declarations
\****************************************/

/****************************************\
* DAT static function declarations
\****************************************/

/****************************************\
* DAT static variable definitions
\****************************************/
/*
static CSLBool Initialized   = FALSE;
static DMA_Handle hDma    = INV;
static Uint16 EventId     = 0x0000;
static Uint16 ChaPriority = 0;
static int SubmitMask     = 0;
static int OpenFlag       = 0;
*/
/****************************************\
* DAT static function definitions
\****************************************/

/*----------------------------------------------------------------------------*/

/******************************************************************************\
*                        G L O B A L   S E C T I O N
\******************************************************************************/

/****************************************\
* DAT global variable definitions
\****************************************/

/****************************************\
* DAT global function definitions
\****************************************/

#pragma CODE_SECTION(DAT_copy2D,".text:DAT_copy2D")

/*----------------------------------------------------------------------------*/
Uint16 DAT_copy2D(DAT_Handle hDat,
                  Uint16 Type,
                  DMA_AdrPtr Src,
                  DMA_AdrPtr Dst,
                  Uint16 LineLen,
                  Uint16 LineCnt,
                  Uint16 LinePitch) {

  DMA_Handle hDma = (DMA_Handle) hDat;
  Uint16 chaMask;
  Uint16 Index;
  Uint16 prio;
  Uint16 srcAmode;
  Uint16 dstAmode;
    int oldgie;

  volatile Uint16 x;

  Index = (LinePitch - LineLen + 1);
  chaMask = 1u<<((DMA_PrivateObj*)hDma)->DmaChaNum;
  srcAmode = (Type >> 4) & 0xFu;
  dstAmode = (Type & 0xFu);
  prio = (CSL_SYS_DATA.DatChaPriority & chaMask)?
            DMA_DMACCR_PRIO_HI : DMA_DMACCR_PRIO_LOW;

  oldgie = IRQ_globalDisable();

  #ifdef _MCRTE_DEBUG
    if (!(CSL_SYS_DATA.DatOpenFlag & chaMask)) {
      ERR_submit(DAT_ERR_MAJOR,DAT_ERR_NOT_OPEN);
      IRQ_globalRestore(oldgie);
      return 0x00000000;
    }
  #endif

//  DmaWait();
    CSL_SYS_DATA.DatSubmitMask |= chaMask;


    DMA_RSETH(hDma,DMACSSAL,(((Uint32)Src << 1) & 0xFFFF));
    DMA_RSETH(hDma,DMACSSAU,(((Uint32)Src >> 15) & 0xFFFF));
    DMA_RSETH(hDma,DMACDSAL,(((Uint32)Dst << 1) & 0xFFFF));
    DMA_RSETH(hDma,DMACDSAU,(((Uint32)Dst >> 15) & 0xFFFF));

    DMA_RSETH(hDma,DMACEN,(Uint16)LineLen);
    DMA_RSETH(hDma,DMACFN,(Uint16)LineCnt);

#if _DMA_DST_AND_SRC_INDEX_SUPPORT
    DMA_RSETH(hDma,DMACSEI,1);
    DMA_RSETH(hDma,DMACSFI,(Uint16)Index);
    DMA_RSETH(hDma,DMACDEI,1);
    DMA_RSETH(hDma,DMACDFI,(Uint16)Index);
#else
    DMA_RSETH(hDma,DMACEI,1);
    DMA_RSETH(hDma,DMACFI,(Uint16)Index);
#endif

#if (CHIP_5502) || (CHIP_5501)
    
    DMA_RSETH(hDma,DMACCR,DMA_DMACCR_RMK(
                      dstAmode,
                      srcAmode,
                      DMA_DMACCR_ENDPROG_OFF,
		      DMA_DMACCR_WP_DEFAULT,
		      DMA_DMACCR_REPEAT_OFF,
                      DMA_DMACCR_AUTOINIT_OFF,
                      DMA_DMACCR_EN_STOP,
                      prio,
                      DMA_DMACCR_FS_DISABLE,
                      DMA_DMACCR_SYNC_NONE
                      ));
#else
    DMA_RSETH(hDma,DMACCR,DMA_DMACCR_RMK(
                      dstAmode,
                      srcAmode,
                      DMA_DMACCR_ENDPROG_OFF,
                      DMA_DMACCR_REPEAT_OFF,
                      DMA_DMACCR_AUTOINIT_OFF,
                      DMA_DMACCR_EN_STOP,
                      prio,
                      DMA_DMACCR_FS_DISABLE,
                      DMA_DMACCR_SYNC_NONE
                      ));
#endif
    
#if (CHIP_5502) || (CHIP_5501)
DMA_RSETH(hDma,DMACICR,DMA_DMACICR_RMK(
                      DMA_DMACICR_AERRIE_ON,
                      DMA_DMACICR_BLOCKIE_ON,
                      DMA_DMACICR_LASTIE_OFF,
                      DMA_DMACICR_FRAMEIE_ON,
                      DMA_DMACICR_HALFIE_OFF,
                      DMA_DMACICR_DROPIE_OFF,
                      DMA_DMACICR_TIMEOUTIE_OFF
                      ));
#else
DMA_RSETH(hDma,DMACICR,DMA_DMACICR_RMK(
                      DMA_DMACICR_BLOCKIE_ON,
                      DMA_DMACICR_LASTIE_OFF,
                      DMA_DMACICR_FRAMEIE_ON,
                      DMA_DMACICR_HALFIE_OFF,
                      DMA_DMACICR_DROPIE_OFF,
                      DMA_DMACICR_TIMEOUTIE_OFF
                      ));
#endif

#if (CHIP_5502) || (CHIP_5501)

    DMA_RSETH(hDma,DMACSDP,DMA_DMACSDP_RMK(
                      DMA_DMACSDP_DSTBEN_NOBURST,
                      DMA_DMACSDP_DSTPACK_OFF,
                      DMA_DMACSDP_DST_DARAMPORT0,
                      DMA_DMACSDP_SRCBEN_NOBURST,
                      DMA_DMACSDP_SRCPACK_OFF,
                      DMA_DMACSDP_SRC_DARAMPORT0,
                      DMA_DMACSDP_DATATYPE_16BIT
                      ));
  #else
       
    DMA_RSETH(hDma,DMACSDP,DMA_DMACSDP_RMK(
                      DMA_DMACSDP_DSTBEN_NOBURST,
                      DMA_DMACSDP_DSTPACK_OFF,
                      DMA_DMACSDP_DST_SARAM,
                      DMA_DMACSDP_SRCBEN_NOBURST,
                      DMA_DMACSDP_SRCPACK_OFF,
                      DMA_DMACSDP_SRC_SARAM,
                      DMA_DMACSDP_DATATYPE_16BIT
                      ));             
  #endif


  DMA_start(hDma);


  IRQ_globalRestore(oldgie);

  return DMA_FGETH(hDma,DMACSR,FRAME);

}
/*----------------------------------------------------------------------------*/

#endif /* DAT_SUPPORT */
/******************************************************************************\
* End of DAT_Copy2D.c
\******************************************************************************/

