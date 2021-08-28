/******************************************************************************/
/* Name     : USB_abortTransaction                                            */
/*                                                                            */
/* Catagory : Data Transfer                                                   */
/*                                                                            */
/* Purpose  : Terminate the data transfer in progress and free up the endpoint*/
/*            to post a new data transfer request                             */
/*                                                                            */
/* Author   : MH                                                              */
/*                                                                            */
/* Based on : None                                                            */
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
/* USB_TRUE if successful, else USB_FALSE                                     */
/*                                                                            */ 
/*============================================================================*/
/* Comments:                                                                  */
/*                                                                            */
/* The endpoint handle determines the endpoint associated with the data       */
/* transfer in progress                                                       */
/*                                                                            */
/*============================================================================*/
/* History:                                                                   */
/*                                                                            */
/* Created:    21 Dec 2000                                                    */
/*                                                                            */
/* Modified:   02/20/01 $MH$                                                  */
/*                                                                            */
/* Renamed from  USB_killTransaction( ) to USB_abortTransaction( )            */           
/*                                                                            */
/******************************************************************************/
#define _USB_MOD

#include <csl_chiphal.h>
#include <csl_usbhal.h>
#include <_csl_usb.h>
#include <csl_usb.h>


#if (_USB_SUPPORT)

#pragma CODE_SECTION (vUSB_abortTransaction, ".text:vUSB_abortTransaction")

/*
12/21/00 $MH$
Stop transaction if any in progress and update the endpoint handle
so a new transaction can be initiated.

** CHECK WITH JDK - WHAT WILL HAPPEN IF EPO BCNT REG NAK-BIT IS SET
   IN THE MIDDLE OF A TRANSFER  **
   
 - IT MAY BE A GOOD IDEA ONLY STOP DMA TRANSACTION AND NOT MESS WITH 
   END POINT 0 TRANSACTION  
*/

// USB_Boolean USB_abortTransaction(USB_EpHandle hEp)
USB_Boolean vUSB_abortTransaction(USB_EpHandle hEp)
{
  USB_DMA_Context *USB_DMA     = (USB_DMA_Context *)hEp->DMA_SAddr;
  USB_EP0_RegBlk  *USB_EP0Reg  = (USB_EP0_RegBlk *)hEp->EDReg_SAddr;
  Uint16          Endpt        = (Uint16)hEp->EpNum;

// return if the endpoint is servicing host port
if(hEp->XferType == USB_HPORT) return(USB_FALSE);

  // if other than EP0
  if(Endpt % 8)
  {
    /*
    set STP bit of DMA control reg,
    init X/Y buffer Count reg ,
    */
    /*
    USB_EDB->X_EPBCNT  = (Endpt / 8) ? USB_EPBCNT_NAK : 0;
    USB_EDB->Y_EPBCNT  = (Endpt / 8) ? USB_EPBCNT_NAK : 0;
    */
    if((USB_DMA->DCTL && USBDCTL_GO) | (USB_DMA->DCTL && USBDCTL_RLD))
        USB_DMA->DCTL    = USBDCTL_STP;
        
    USB_DMA->DSIZ    = 0;
    USB_DMA->DADL    = 0;
    USB_DMA->DADH    = 0;
    USB_DMA->DCT     = 0;
    USB_DMA->DRSZ    = 0;
    USB_DMA->DRAL    = 0;
    USB_DMA->DRAH    = 0;
  }
  
  else
  {
    /* 
    set NAK bit of EP0 BCNT reg
    clear TotBytCnt field of EP0 handle
    */
    
    USB_EP0Reg->CT = USBEPCT_NAK;
  }
  
  /* 
  clear TotBytCnt field of endpoint handle
  clear Flags field of endpoint handle
  */
  hEp->DataFlags    = 0x00;
  hEp->Status       = 0x00;
  hEp->TotBytCnt    = 0x00;
  hEp->BytInThisSeg = 0x00;
  hEp->pBuffer      = NULL;
  hEp->pNextBuffer  = NULL;
  hEp->EventFlag    = 0x00;

  return(USB_TRUE);
}

#endif
