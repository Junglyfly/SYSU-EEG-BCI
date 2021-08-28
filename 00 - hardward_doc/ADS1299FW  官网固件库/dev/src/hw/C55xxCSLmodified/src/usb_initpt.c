/******************************************************************************/
/* Name     : USB_initEndptObj                                                */
/*                                                                            */
/* Catagory : Initialization                                                  */
/*                                                                            */
/* Purpose  : Initialize an endpoint object                                   */
/*                                                                            */
/*                                                                            */
/* Author   : MH                                                              */
/*                                                                            */
/* Based on : From scratch                                                    */
/*                                                                            */
/*                                                                            */
/*============================================================================*/
/* Arguments:                                                                 */
/*                                                                            */
/* DevNum   : USB device number, enumerated data type of USB_DevNum.          */
/*            Only USB0 is active currently                                   */
/*                                                                            */
/* hEp      : Handle to an endpoint object to be initialized                  */
/*                                                                            */
/* EpNum    : USB endpoint number of enumarated data type USB_EpNum           */
/*                                                                            */
/* XferType : Type of data transfer will be implemented with this endpoint    */
/*                                                                            */
/* MaxPktSiz: Max data packet size supported by the endpoint                  */
/*                                                                            */
/* EvMsk    : ORed combination of USB Interrupt Events to be broadcasted      */
/*            to the associated event handler                                 */
/*                                                                            */
/* Fxn      : Associated event handler routine                                */
/*                                                                            */
/*============================================================================*/
/* Return Value:                                                              */
/*                                                                            */
/* USB_TRUE if the initialization is successful, else USB_FLASE               */
/*                                                                            */ 
/*============================================================================*/
/* Comments:                                                                  */
/*                                                                            */
/* The event handler should be in void Fxn(void) form. Using the INTERRUPT    */
/* is not necessary.  Once the program control branches to Fxn the user's     */
/* code is free to call other functions or event post DSP?BIOS SWI            */
/*                                                                            */
/*============================================================================*/
/* History:                                                                   */
/*                                                                            */
/* Created:       24 Nov 2000                                                 */
/* Last Modified: 05 Jan 2001                                                 */
/*                                                                            */
/* 08/27/02 $MH$ added check for max endpt size to 1023 (10bit value) bytes   */
/*                                                                            */
/******************************************************************************/
#define _USB_MOD

#include <csl_chiphal.h>
#include <csl_usbhal.h>
#include <_csl_usb.h>
#include <csl_usb.h>

#if (_USB_SUPPORT)

#pragma CODE_SECTION (vUSB_initEndptObj, ".text:vUSB_initEndptObj")

/* --------------------------11/27/00 $MH$----------------------------------
 Initialize USB endpoint handle
 01/05/00 added EventMask and EventFlag for Event broad casting capabilities 
 ----------------------------------------------------------------------------*/
 
//USB_Boolean USB_initEndptObj(USB_DevNum      DevNum,
//                              USB_EpHandle   hEp,
//                              USB_EpNum      EpNum, 
//                              USB_XferType   XferType,
//                              Uint16         MaxPktSiz,
//                              Uint16         EvMsk,
//                              USB_EVENT_ISR  Fxn)
                              
USB_Boolean vUSB_initEndptObj(USB_DevNum      DevNum,
                              USB_EpHandle   hEp,
                              USB_EpNum      EpNum, 
                              USB_XferType   XferType,
                              Uint16         MaxPktSiz,
                              Uint16         EvMsk,
                              USB_EVENT_ISR  Fxn)                              
{
  USB_Boolean ret_stat       = USB_FALSE;
  Uint16 ii                  = (Uint16)EpNum;
  USB_EDRegBlk *USB_EDB      = (USB_EDRegBlk *)USB_EDB_START;
  USB_DMA_Context *USB_DMA   = (USB_DMA_Context *)USB_DMA_REG_START;
  
  
  hEp->DataFlags    = 0x0000;
  hEp->Status       = 0x0000;
  hEp->EpNum        = EpNum;
  hEp->XferType     = XferType;
  hEp->MaxPktSiz    = MaxPktSiz & 0x3FF;  // $MH$ 08/27/02 check for max endpt size
  hEp->TotBytCnt    = 0x0000;
  hEp->BytInThisSeg = 0x0000;
  hEp->pBuffer      = NULL;
  hEp->pNextBuffer  = NULL;
  hEp->EventMask    = EvMsk;
  hEp->EventFlag    = 0x00;
  hEp->Fxn          = Fxn;
  
  switch(EpNum)
  {
    case USB_OUT_EP0 : /* use DevNum to calculate appropriate reg addr if future
                          DSPs support multiple USB_Module                      */
                       hEp->EDReg_SAddr  = (Uint32)USBOCNF0_ADDR;
                       /* since there is no DMA for EP0, use hEp->DMA_SAddr
                          to hold EP0 data buffer start addr, use DevNum to 
                          calculate appropriate data buffer start addr if future
                          DSPs support multiple USB_Module                      */
                       hEp->DMA_SAddr    = (Uint32)USBOEP0_BUF;
                       ret_stat = USB_TRUE;
                       break;
    
    case USB_IN_EP0  : /* use DevNum to calculate appropriate reg addr if future
                          DSPs support multiple USB_Module                      */
                       hEp->EDReg_SAddr  = (Uint32)USBICNF0_ADDR;
                       /* since there is no DMA for EP0, use hEp->DMA_SAddr
                          to hold EP0 data buffer start addr, use DevNum to 
                          calculate appropriate data buffer start addr if future
                          DSPs support multiple USB_Module                      */
                       hEp->DMA_SAddr    = (Uint32)USBIEP0_BUF;
                       ret_stat = USB_TRUE;
                       break;
                       
    default          : hEp->EDReg_SAddr  = (Uint32)(&USB_EDB[ii]);
                       hEp->DMA_SAddr    = (Uint32)(&USB_DMA[ii]);
                       ret_stat = USB_TRUE;
                       break;
  }          /* switch(EPxIndex) */
  return(ret_stat);
}
#endif
