/******************************************************************************/
/* Name     : USB_bytesRemaining                                              */
/*                                                                            */
/* Catagory : Status / Query                                                  */
/*                                                                            */
/* Purpose  : Find out number of bytes waiting to be transferred from the     */
/*            previously posted data transfer request                         */
/*                                                                            */
/* Author   : MH (partially)                                                  */
/*                                                                            */
/* Based on : USB code by Jason Kridner(IA) and Venkat Raghavan (TI India)    */
/*                                                                            */
/*                                                                            */
/*============================================================================*/
/* Arguments:                                                                 */
/*                                                                            */
/* hEp      : Handle to an initialized endpoint object                        */
/*                                                                            */
/*============================================================================*/
/* Return Value:                                                              */
/*                                                                            */
/* Number of bytes remaining to be transferred. Return value is 0xFFFF if     */
/* USB_IOFLAG_EOLL flag used while posting the transfer request       */
/*                                                                            */ 
/*============================================================================*/
/* Comments:                                                                  */
/*                                                                            */
/* The endpoint handle determines the endpoint data moves pass through.       */
/*                                                                            */
/*============================================================================*/
/* History:                                                                   */
/*                                                                            */
/* Created:        28 Nov 2000                                                */
/* Last Modified:  21 Feb 2001                                                */
/*                                                                            */
/******************************************************************************/
#define _USB_MOD

#include <csl_chiphal.h>
#include <csl_usbhal.h>
#include <_csl_usb.h>
#include <csl_usb.h>

#if (_USB_SUPPORT)

#pragma CODE_SECTION (vUSB_bytesRemaining, ".text:vUSB_bytesRemaining")

/* ----------------------------------------------------------------------------
 Give the number of bytes remained to be transferred in the endpoint buffer.
 11/28/00 $MH$:  Incomplete
 12/05/00 $MH$:  made changes to support linked list in USB_postTransaction( )
 ----------------------------------------------------------------------------*/
// USB_BYTE_COUNT USB_bytesRemaining(USB_EpHandle hEp)
USB_BYTE_COUNT vUSB_bytesRemaining(USB_EpHandle hEp)
{
  USB_DMA_Context *USB_DMA               = (USB_DMA_Context *)hEp->DMA_SAddr;
  Uint16 Endpt                = (Uint16)hEp->EpNum;
  Uint16 BytesInDMA              = 0x00;  
  /*
  12/04/00 $MH$
  USB_EP0_RegBlk *USB_EP0Reg             = (USB_EP0_RegBlk *)hEp->EDReg_SAddr;
  USB_Boolean ret_stat;
  */
#ifdef USE_BIOS
  Uint16 int_status = HWI_disable();
#endif

  if((Endpt % 8) && !(hEp->DataFlags & USB_IOFLAG_EOLL))
  {
    /* -----------------------------------------------------------------------
    For DMA transfers, the number of bytes transferred is indicated by DMA
    transfer count. So, the number of bytes to be transferred would be:

    1. DMA transfer size - DMA transfer count if RLD is not set.
    2. DMA transfer size + DMA reload size - DMA transfer count if RLD is set.
     -----------------------------------------------------------------------*/

    if(USB_DMA->DCTL & USBDCTL_GO)
    {
      BytesInDMA = USB_DMA->DSIZ - USB_DMA->DCT;
      if(USB_DMA->DCTL & USBDCTL_RLD)
      {
        BytesInDMA += USB_DMA->DRSZ;
      }
    }
  }
#ifdef USE_BIOS
  HWI_enable(int_status);
#endif

  return(hEp->TotBytCnt + BytesInDMA);
}
#endif
