*********************************************************************
** api_vect.s55                                                    **
**                                                                 **
** Copyright (c) 2001, Texas Instruments, Inc.                     **
** All rights reserved.                                            **
**                                                                 **
** TMS320C55x code for VC5509 ROM API Call Table                   **
**                                                                 **
** Author: MH(partial)                                             **
**                                                                 **
** Based On:  API Call Vector table and the API Look Up Table      **
**            for security routines or media drivers developed by  **
**            Bill Wrightson for IA group                          **
**                                                                 **
** Date: 01 Mar 2001                                               **
**                                                                 **
** 18 Jul 2001 $MH$                                                **
** Replaced USB_setParams with USB_init                            **
**                                                                 **
*********************************************************************

*********************************************************************
* This file includes the API Call Vector table and the API Look Up  *
* Table which are combined to implement a relocate-able call table  *
* for the USB API routines                                          *
*                                                                   *
* This relocateable table allows user applications to call these    *
* routines out of ROM or out of RAM by relocating the API Vector    *
* Table to RAM and replacing any desired functions with new ones    *
* while still being able to use ROM functions.  The initial API     *
* Vector Table contained in this file is the ROM version which will *
* be used as the default table until or if a user application moves *
* the table to another location in ROM or more likely RAM.          *
*                                                                   *
* This scheme is specially helpful to replace the buggy USB API in  *
* ROM with a bug free one                                           *
*                                                                   *
* USB Memory locations 0x667E and 0x667F are reserved to point to   *
* the API Vector Table.  These are 8 bit locations and hold the     *
* two bytes of a 24 bit address, the lower byte is assumed to be 0  *
* thus forcing the table to be allocated on 256 byte boundaries.    *
*                                                                   *
*********************************************************************
* Revision 0.0                                                      *
*********************************************************************


*********************************************************************
*                      Constants                                    *
*********************************************************************
USBIDLCTL        .set 7000h    ; USB Idle Control Register address
USBIDLCTL_USBEN  .set 0004h    ; USB enable bit mask

USB_API_PTR      .set 667Eh    ; USB RAM pointer to API Vector Table


*********************************************************************
*                      Global Symbols                               *
*********************************************************************
                .def  _USB_setAPIVectorAddress


*********************************************************************
* API Vector Table - This is a table of call vectors for VC5509     *
*                    USB API routines .  This table can be          *
*                    copied to RAM and used if the USB RAM pointers *
*                    are changed to the new location.  The symbols  *
*                    defined in this table are used for calculating *
*                    the index of each function within the table.   *
*                    This index is used in the API LUT to call the  *
*                    proper function for use while in ROM.  If this *
*                    table is copied to RAM the symbols can be      *
*                    replaced with the actual function call symbols *
*                    from the API LUT such that the API LUT code    *
*                    can be skipped for this new table. Functions   *
*                    in ROM will still have to go through the API   *
*                    LUT process so that they will use the new API  *
*                    Vector Table.                                  *
*********************************************************************

                .sect   "api_vect"

VecMacro .macro base, target
        .ref    _:target:
r:base: goto    _:target:
        .endm

                .align  256
                .def    _ROM_API_TABLE
_ROM_API_TABLE
        VecMacro USB_getEvents                 , vUSB_getEvents              
        VecMacro USB_peekEvents                , vUSB_peekEvents                                              
        VecMacro USB_getSetupPacket            , vUSB_getSetupPacket                                                                     
        VecMacro USB_postTransaction           , vUSB_postTransaction                                                                    
        VecMacro USB_isTransactionDone         , vUSB_isTransactionDone                                                                   
        VecMacro USB_bytesRemaining            , vUSB_bytesRemaining                                                                     
        VecMacro USB_stallEndpt                , vUSB_stallEndpt                                                                         
        VecMacro USB_clearEndptStall           , vUSB_clearEndptStall                                                                    
        VecMacro USB_getEndptStall             , vUSB_getEndptStall                                                                      
        VecMacro USB_initEndptObj              , vUSB_initEndptObj                                                                        
        VecMacro _USB_configEndpt              , _vUSB_configEndpt                                                                                 
        VecMacro USB_init                      , vUSB_init                                                                               
        VecMacro _USB_addInterface             , _vUSB_addInterface                                                                           
        VecMacro USB_epNumToHandle             , vUSB_epNumToHandle                                                                         
        VecMacro _USB_resetFirmwareEnv         , _vUSB_resetFirmwareEnv                                                                       
        VecMacro USB_abortTransaction          , vUSB_abortTransaction                                                                      
        VecMacro USB_abortAllTransaction       , vUSB_abortAllTransaction                                                                     
        VecMacro _USB_enableEpIntrpt           , _vUSB_enableEpIntrpt                                                                        
        VecMacro _USB_disableEpIntrpt          , _vUSB_disableEpIntrpt                                                                        
        VecMacro _USB_enableDmaIntrpt          , _vUSB_enableDmaIntrpt                                                                  
        VecMacro _USB_disableDmaIntrpt         , _vUSB_disableDmaIntrpt
        
        VecMacro USB_connectDev                , vUSB_connectDev                                                                         
        VecMacro USB_disconnectDev             , vUSB_disconnectDev                                                                                                                                     
        VecMacro USB_setRemoteWakeup           , vUSB_setRemoteWakeup
        VecMacro USB_getRemoteWakeupStat       , vUSB_getRemoteWakeupStat
        VecMacro USB_issueRemoteWakeup         , vUSB_issueRemoteWakeup                                                                                                                         
        VecMacro USB_resetDev                  , vUSB_resetDev                                                   
        VecMacro USB_setDevAddr                , vUSB_setDevAddr                                                                                                                              
        VecMacro USB_getFrameNo                , vUSB_getFrameNo                                      
                                                                                                                                         
        VecMacro USB_initPLL                   , vUSB_initPLL                                                                                  
        VecMacro _USB_procCtrlOutTransaction   , _vUSB_procCtrlOutTransaction                                                                               
        VecMacro _USB_procCtrlInTransaction    , _vUSB_procCtrlInTransaction                                                                        
        VecMacro _USB_procDmaTransaction       , _vUSB_procDmaTransaction
        VecMacro _USB_gotoNextList             , _vUSB_gotoNextList                                                                               
        VecMacro USB_evDispatch                , vUSB_evDispatch                                                                                           
                                                                        
; reserve some space for future entries... total of 63 entries

                .space     28*32                   

*********************************************************************
* API Look Up Table - This is a table of individual look up table   *
*                     routines which are used to call a function    *
*                     in the API Vector Table while using a global  *
*                     memory pointer to indicate which API Vector   *
*                     table needs to be used at run-time.  This     *
*                     table implements the relocation capability of *
*                     the call table system.
*********************************************************************

                .sect   "api_lut"
_GetApiLutStart
        push(AR5)                        ; save AR5 for the C env.
        dbl(push(AC0))                   ; save AC0 for the C env.
     || AR5 = #USB_API_PTR               ; get ptr to API Vect table
        AC0 = *AR5+ << #16 || readport() ; read A23-A16 of API Table
        AC3 = *AR5  << #8  || readport() ; read A15-A8 of API Table
        AC3 = AC3 + AC0                  ; combine A23-A16 and A15-A8
     || AC0 = dbl(pop())                 ; restore AC0
        AR5 = pop()                      ; restore AR5
        return

*********************************************************************
* API Function indexes - These are indexes used in the API LUT code.*
*                        These indexes indicate the offset for each *
*                        vector in the API Call Vector table.       *
*********************************************************************
LutMacro .macro base
i:base: .set (r:base: - _ROM_API_TABLE)
        .def    _:base:

_:base:
        call _GetApiLutStart
        AC3 = AC3 + #i:base:             ; add API offset to base addr
        goto AC3                         ; call API vector
        .endm

        LutMacro USB_getEvents           ; total 26 entries
        LutMacro USB_peekEvents
        LutMacro USB_getSetupPacket
        LutMacro USB_postTransaction
        LutMacro USB_isTransactionDone
        LutMacro USB_bytesRemaining
        LutMacro USB_stallEndpt
        LutMacro USB_clearEndptStall
        LutMacro USB_getEndptStall
        LutMacro USB_initEndptObj
        LutMacro _USB_configEndpt
        LutMacro USB_init
        LutMacro _USB_addInterface
        LutMacro USB_epNumToHandle
        LutMacro _USB_resetFirmwareEnv
        LutMacro USB_abortTransaction
        LutMacro USB_abortAllTransaction
        LutMacro _USB_enableEpIntrpt
        LutMacro _USB_disableEpIntrpt
        LutMacro _USB_enableDmaIntrpt       
        LutMacro _USB_disableDmaIntrpt 
        LutMacro USB_connectDev       
        LutMacro USB_disconnectDev
        LutMacro USB_setRemoteWakeup
        LutMacro USB_getRemoteWakeupStat    
        LutMacro USB_issueRemoteWakeup
        LutMacro USB_resetDev     
        LutMacro USB_setDevAddr       
        LutMacro USB_getFrameNo              
        LutMacro USB_initPLL       
        LutMacro _USB_procCtrlOutTransaction
        LutMacro _USB_procCtrlInTransaction
        LutMacro _USB_procDmaTransaction
        LutMacro _USB_gotoNextList       
        LutMacro USB_evDispatch       

                .sect   ".text"              

********************************************************************************
** Name     : _USB_setAPIVectorAddress                                        **
**                                                                            **
** Purpose  : Initialize the USB API vector pointer with the address of the   **
**            address of the USB API lookup table.  The default address of    **
**            the USB API lookup table is in the ROM.                         **
**                                                                            **
** Author   : MH                                                              **
**                                                                            **
********************************************************************************
_USB_setAPIVectorAddress:

        push(AR5)                         ; save AR5 for the C env.
        dbl(push(AC0))                    ; save AC0 for the C env.
        
        AR5  = #USBIDLCTL                 ; get ptr to API Vect table
        AC0  = *AR5 || readport()         ; read USB idle ctrl reg
        AC0  = AC0 | #USBIDLCTL_USBEN     ; set USB enable bit
        *AR5 = AC0 || writeport()         ; read USB idle ctrl reg
        
        AR5 = #USB_API_PTR                ; get ptr to API Vect table
        AC0 = #(_ROM_API_TABLE >> 8)      ; read A23-A8 of API Table 
               
        AC3 = AC0 << #-8                  ; move A23-A16 of API Table to AC3
        *AR5+ = AC3 || writeport()        ; write A23-A16 of API Table
        
        AC3  = AC0 & #00FFh               ; A15-A8 of API Table to AC3
        *AR5  = AC3 || writeport()        ; write A15-A8 of API Table
        
        AC0 = dbl(pop())                  ; restore AC0
        AR5 = pop()                       ; restore AR5
        return
        
        .end
