/******************************************************************************/
/* Name     : USB_postTransaction                                             */
/*                                                                            */
/* Catagory : Data Transfer                                                   */
/*                                                                            */
/* Purpose  : Transmit and receive USB data through an endpoint               */
/*                                                                            */
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
/* ByteCnt  : Total number of bytes in the buffer pointed by *Data            */
/*                                                                            */
/* *Data    : A pointer to a data buffer or to a linked list of type          */
/*            USB_DataStruct                                                  */
/*                                                                            */
/* Flags    : ORed combination of USB Data Transfer Flags                     */
/*                                                                            */
/*============================================================================*/
/* Return Value:                                                              */
/*                                                                            */
/* USB_FALSE, if the previously posted transfer is not completed. USB_TRUE    */
/* if the data transfer request was posted successfully.                      */
/*                                                                            */
/*============================================================================*/
/* Comments:                                                                  */
/*                                                                            */
/* The endpoint handle determines the endpoint data moving in or out of the   */
/* USB module. At the end of the data transfer the USB event dispatcher will  */
/* call the associated event handler routine if the USB_TRANSACTIN_DONE event */
/* mask is set and an event handler routine is supplied during the endpoint   */
/* object initialization                                                      */
/*                                                                            */
/*============================================================================*/
/* History:                                                                   */
/*                                                                            */
/* Created:        28 Nov 2000                                                */
/* Last Modified:  10 Jan 2001                                                */
/*                                                                            */
/* 03/05/01 $MH$ Added code to keep track of actual number of bytes xferred   */
/* specially for OUT xfers terminated short.  DMA keeps track of actual number*/
/* of bytes moved in from a OUT transfer, for the rest of the endpoints       */
/* software will update the 1st word of a buffer with the actual number bytes */
/* moved in(out) of the data buffer                                           */
/*                                                                            */
/******************************************************************************/
#define _USB_MOD

#include <csl_chiphal.h>
#include <csl_usbhal.h>
#include <_csl_usb.h>
#include <csl_usb.h>

#if (_USB_SUPPORT)

extern USB_GlobalVars_struct USB_GlobalVars;

#pragma CODE_SECTION (vUSB_postTransaction, ".text:vUSB_postTransaction")

/* ----------------------------------------------------------------------------
 Prepare the endpoint for the next transaction. If the endpoint is anything
 other than 0, prepare DMA transaction, or else, prepare transaction via the
 DMA. The data can be just a set of bytes or a linked list, in which case, each
 data parameter belongs to a USB_DataStruct structure.
 Modified 11/28/00 $MH$:
 ----------------------------------------------------------------------------*/
//USB_Boolean USB_postTransaction(USB_EpHandle      hEp,
//                                     Uint16       Bytes,
//                                     void         *Data,
//                                     USB_IOFLAGS  Flags)

USB_Boolean vUSB_postTransaction(USB_EpHandle     hEp,
                                     Uint16       Bytes,
                                     void         *Data,
                                     USB_IOFLAGS  Flags)
{
  USB_EpHandle hEp0In, hEp0Out;

  /* USB_GlobalVars_struct *USB_GlobalVars = (USB_GlobalVars_struct *)USB_GLOBALVAR_BASE; */

  /* do not prepare a new transaction if previous transaction in progress */
  // 03/05/01 $MH$ added check for endpt obj belongs to HPORT
  if((hEp->XferType == USB_HPORT) || (!USB_isTransactionDone(hEp))) return(USB_FALSE);

  hEp->DataFlags = USB_IOFLAG_NEW | (Flags & (USB_IOFLAG_LNK | USB_IOFLAG_CAT |
                   USB_IOFLAG_EOLL | USB_IOFLAG_NOSHORT | USB_IOFLAG_SWAP));

  hEp->EventFlag &= ~USB_EVENT_EOT;

  if(hEp->DataFlags & USB_IOFLAG_EOLL)
     hEp->TotBytCnt = 0xFFFF;
  else
     hEp->TotBytCnt = Bytes;

  // If a linked list being transferred
  if(Flags & USB_IOFLAG_LNK)
  {
    hEp->BytInThisSeg   = ((USB_DataStruct *)Data)->Bytes;
    hEp->pBuffer        = ((USB_DataStruct *)Data)->pBuffer;
    hEp->pNextBuffer    = ((USB_DataStruct *)Data)->pNextBuffer;
  }

  else
  {
    hEp->BytInThisSeg   = Bytes;
    hEp->pBuffer        = Data;
    hEp->pNextBuffer    = NULL;
  }

  // 03/05/01 $MH$ keep track of the 1st word of the buffer to store actual number
  // of bytes moved in (out) and also advance the pBuffer to the next word which is
  // in fact the start of the actual data.  For OUT DMA endpoints this is done by
  // the DMA so for the OUT DMA endpoints we will decrement the pointer before
  // posting the DMA endpoint transfer

  hEp->XferBytCnt = hEp->pBuffer;
  if(hEp->pBuffer != NULL)            // to handle NULL data passed
  {                                   // only handled for EP0 IN/OUT
    hEp->pBuffer++;                   // NULL data pointer is not supported for any other endpts
    *hEp->XferBytCnt = 0x0000;        // clear XferBytCnt field
  }

  switch(hEp->EpNum)
  {
    case USB_OUT_EP0:                 // OUT_EP0 transaction

         hEp0In  = (USB_EpHandle )(((Uint32)USB_GlobalVars.EpHandleTable[8][2]) |
                                   (((Uint32)USB_GlobalVars.EpHandleTable[8][1]) << 8) |
                                   (((Uint32)USB_GlobalVars.EpHandleTable[8][0]) << 16));

         if(USB_isTransactionDone(hEp0In) && (!(hEp0In->DataFlags & USB_IOFLAG_DATAREQ)))
         _USB_procCtrlOutTransaction(hEp);

         break;

    case USB_IN_EP0:                  // IN_EP0 transaction

         hEp0Out = (USB_EpHandle )(((Uint32)USB_GlobalVars.EpHandleTable[0][2]) |
                                   (((Uint32)USB_GlobalVars.EpHandleTable[0][1]) << 8) |
                                   (((Uint32)USB_GlobalVars.EpHandleTable[0][0]) << 16));

         if(USB_isTransactionDone(hEp0Out))
         _USB_procCtrlInTransaction(hEp);

         break;

    default:
         hEp->DataFlags &= ~USB_IOFLAG_NEW;
         hEp->DataFlags |= USB_IOFLAG_DATAREQ;

         // if OUT DMA endpoint decrement the buffer pointer to the XferBytCnt
         // so the DMA can update this field
         if(!(hEp->EpNum / 8))  hEp->pBuffer--;
         _USB_procDmaTransaction(hEp);     /* rest - DMA transaction required */
         break;                            /* should we test the DMA endpoint instead of relying on default */
  }                                        /* end of switch( )  */
  return(USB_TRUE);
}

#endif
