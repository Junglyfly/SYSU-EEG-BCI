/******************************************************************************/
/* Name     : USB_isTransactionDone                                           */
/*                                                                            */
/* Catagory : Status / Query                                                  */
/*                                                                            */
/* Purpose  : Find out the status of the previously posted data transfer      */
/*            request                                                         */
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
/* USB_TRUE, if the previously posted transfer is completed, else USB_FLASE.  */
/*                                                                            */ 
/*============================================================================*/
/* Comments:                                                                  */
/*                                                                            */
/* The endpoint handle determines the endpoint data will pass through.        */
/* It is a good idea to call this function before posting a data transfer     */
/* request                                                                    */
/*                                                                            */
/*============================================================================*/
/* History:                                                                   */
/*                                                                            */
/* Created:        28 Nov 2000                                                */
/* Last Modified:  10 Jan 2001                                                */
/*                                                                            */
/******************************************************************************/
#define _USB_MOD

#include <csl_usbhal.h>
#include <_csl_usb.h>
#include <csl_usb.h>

#if (_USB_SUPPORT)

#pragma CODE_SECTION (vUSB_isTransactionDone, ".text:vUSB_isTransactionDone")

/* ----------------------------------------------------------------------------
  Return the status of the previous transaction.
  Modified 11/28/00 $MH$
           12/05/00 $MH$
           01/09/01 $MH$: test if USB_IOFLAG_DATAREQ is cleared to determine
                          if the transaction is done - for now it's not implemented
 ----------------------------------------------------------------------------*/
// USB_Boolean USB_isTransactionDone(USB_EpHandle hEp)
USB_Boolean vUSB_isTransactionDone(USB_EpHandle hEp)
{
  USB_DMA_Context *USB_DMA    = (USB_DMA_Context *)hEp->DMA_SAddr;
  USB_EP0_RegBlk  *USB_EP0Reg = (USB_EP0_RegBlk *)hEp->EDReg_SAddr;
  Uint16          Endpt       = (Uint16)hEp->EpNum;
  USB_Boolean     ret_stat;

  if(Endpt % 8)
  {
    ret_stat = ((USB_DMA->DCTL & USBDCTL_GO) || 
                (hEp->DataFlags & USB_IOFLAG_DATAREQ)) ? USB_FALSE : USB_TRUE;
  }
  
  else
  {
   ret_stat = ((USB_EP0Reg->CT & USBEPCT_NAK) && !(hEp->DataFlags & 
               (USB_IOFLAG_DATAREQ | USB_IOFLAG_NEW))) ? USB_TRUE : USB_FALSE;
  }

  return(ret_stat);
}

#endif

