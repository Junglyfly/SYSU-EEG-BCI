/******************************************************************************/
/* usb_drvr.c                                                                 */
/*                                                                            */
/* This is proprietary information, not to be published -- TI INTERNAL DATA   */
/* Copyright (C) 1999, Texas Instruments, Inc.  All Rights Reserved.          */
/*                                                                            */
/* USB event dispatcher and generic endpoint data handler routines.           */
/*                                                                            */
/* Author: MH                                                                 */
/* Date: 24 Nov 2000                                                          */
/* Based on : USB code by Jason Kridner(IA) and Venkat Raghavan (TI India)    */
/*                                                                            */
/******************************************************************************/
/*                                                                            */
/*    Modified:                                                               */
/*    11/30/00 $MH$                                                           */
/*    02/20/01 $MH$                                                           */
/*    Added following linked list handling features to the Control and DMA    */
/*    endpoint drivers.                                                       */
/*    Data Buffer Supported:  a. Single buffer                                */
/*                            b. Multiple buffer in the form of linked list   */
/*                                                                            */
/*    End of transfer at:     a. TotBytCnt = 0 (with/without a short packet   */
/*                            b. End of linked list(         "             )  */
/*                            c. Either of the above two condition happens    */
/*                               first                                        */
/*                                                                            */
/*    Linked list handling:   a. Treat every linked list as a single data     */
/*                               buffer.                                      */
/*                            b. Concatenate all the linked list to create    */
/*                               a large data buffer                          */
/*                                                                            */
/* 03/05/01 $MH$ Added code to keep track of actual number of bytes xferred   */
/* specially for OUT xfers terminated short.  DMA keeps track of actual number*/
/* of bytes moved in from a OUT transfer, for the rest of the endpoints       */
/* software will update the 1st word of a buffer with the actual number bytes */
/* moved in(out) of the data buffer                                           */
/*                                                                            */
/* 03 May 2001 $MH$                                                           */
/* Added #pragma directive to allocate code in ".usb_drvr" section instead of */
/* default .text section                                                      */
/*                                                                            */
/* 28 Sep 2001 $MH$                                                           */
/* Converted INTERRUPT _vUSB_isr() to void vUSB_evDispatch(void), so the      */
/* module can be used with DSP/BIOS                                           */
/*                                                                            */
/******************************************************************************/
#define _USB_MOD

#include <_csl.h>

#if (_USB_SUPPORT)

#include <csl_usb.h>
#include <_csl_usb.h>
#include <csl_usbhal.h>
#include <_csl_usbdrvr.h>



#pragma CODE_SECTION (_vUSB_procCtrlOutTransaction, ".text:usb_drvr")

// void _USB_procCtrlOutTransaction(USB_EpHandle hOutEp0)
void _vUSB_procCtrlOutTransaction(USB_EpHandle hOutEp0)
{
  Uint16 ii, bytes_in;
  USB_EP0_RegBlk *USB_EP0Reg  = (USB_EP0_RegBlk *)hOutEp0->EDReg_SAddr;
  ioport Uchar   *ptr         = (ioport Uchar *)hOutEp0->DMA_SAddr;

  /* -----------------12/13/99 1:34AM------------------
  if ready receive data
   --------------------------------------------------*/
  if((hOutEp0->DataFlags & USB_IOFLAG_DATAREQ) && (USB_EP0Reg->CT & USBEPCT_NAK))
  {
    bytes_in = USB_EP0Reg->CT & ~USBEPCT_NAK;

    if(hOutEp0->DataFlags & USB_IOFLAG_0BYTREQ)
    {

      if(bytes_in < hOutEp0->MaxPktSiz)
      {
         hOutEp0->DataFlags &= ~USB_IOFLAG_0BYTREQ;

        if((hOutEp0->TotBytCnt == 0)||((hOutEp0->BytInThisSeg == 0) &&
                                      (hOutEp0->pNextBuffer == NULL)))
          {
            hOutEp0->DataFlags &= ~USB_IOFLAG_DATAREQ;
            hOutEp0->EventFlag |= USB_EVENT_EOT;
          }
      }

    }

    else
    { // 03/06/01 $MH$ added this if statement to terminate transfer if data
      // block terminated unexpectedly with a short packet in CAT mode
      if((bytes_in < hOutEp0->MaxPktSiz)&&(hOutEp0->DataFlags & USB_IOFLAG_CAT))
      {
        hOutEp0->DataFlags &= ~USB_IOFLAG_DATAREQ;
        hOutEp0->EventFlag |= USB_EVENT_EOT;
      }

      for(ii=0; ii<= bytes_in; ii++, ptr++)
      {
        if((hOutEp0->TotBytCnt == 0) || ((hOutEp0->pNextBuffer == NULL) && (hOutEp0->BytInThisSeg == 0)))
        {
          if((ii == hOutEp0->MaxPktSiz) && !(hOutEp0->DataFlags & USB_IOFLAG_NOSHORT))
            {
              hOutEp0->DataFlags |= USB_IOFLAG_0BYTREQ;
            }
          else
            {
              hOutEp0->DataFlags &= ~USB_IOFLAG_DATAREQ;
              hOutEp0->EventFlag |= USB_EVENT_EOT;
            }
          break;
        }
        else if(hOutEp0->BytInThisSeg == 0)
        {
          if(ii == hOutEp0->MaxPktSiz)
          {
            if(!(hOutEp0->DataFlags & USB_IOFLAG_NOSHORT) && !(hOutEp0->DataFlags & USB_IOFLAG_CAT))
              {
                hOutEp0->DataFlags |= USB_IOFLAG_0BYTREQ;
              }
          }
          _USB_gotoNextList(hOutEp0);

          if(!(hOutEp0->DataFlags & USB_IOFLAG_CAT))
              break;
        }
        else if(ii == bytes_in)
             {
               if((bytes_in < hOutEp0->MaxPktSiz) && !(hOutEp0->DataFlags & USB_IOFLAG_CAT))
                 {
                   if(hOutEp0->pNextBuffer == NULL)
                   {
                     hOutEp0->DataFlags &= ~USB_IOFLAG_DATAREQ;
                     hOutEp0->EventFlag |= USB_EVENT_EOT;
                   }
                   else
                     _USB_gotoNextList(hOutEp0);
                 }
               break;
             }

           if(hOutEp0->pBuffer != NULL)
           {

             if(hOutEp0->DataFlags & USB_IOFLAG_BYTE)        /* USB second byte                      */
               {
                 if(hOutEp0->DataFlags & USB_IOFLAG_SWAP)
                    *(hOutEp0->pBuffer) |= (*ptr)&0x00FF;       /* lo byte - swap                       */
                 else
                    *(hOutEp0->pBuffer) |= (*ptr)<<8;           /* hi byte - no swap                    */

                 hOutEp0->pBuffer++;
                 hOutEp0->DataFlags &= ~USB_IOFLAG_BYTE;     /* flag next byte as USB first byte     */
               }
             else                                                 /* USB first byte                       */
               {
                 if(hOutEp0->DataFlags & USB_IOFLAG_SWAP)
                    *(hOutEp0->pBuffer) = ((*ptr)<<8);          /* hi byte - swap                       */
                 else
                    *(hOutEp0->pBuffer) = (*ptr)&0x00FF;        /* lo byte - no swap                    */

                 hOutEp0->DataFlags |= USB_IOFLAG_BYTE;      /* flag next byte as USB second byte    */
               }

             }                                    // if(hOutEp0->pBuffer != NULL)

           /* -----------------11/30/00 $MH$------------------
             if BytInThisSeg = 0 then call link list handle routine
             instead of calling the routine and let it test BytInThisSeg
             counter
           --------------------------------------------------*/
           hOutEp0->BytInThisSeg--;         // decrement # bytes remained to transfer

           if(!(hOutEp0->DataFlags & USB_IOFLAG_EOLL))
                hOutEp0->TotBytCnt--;

           // 03/13/01 $MH$ moved the following instructions inside the if(hOutEp0->pBuffer != NULL)
           // above
           if(hOutEp0->XferBytCnt != NULL)
           (*hOutEp0->XferBytCnt)++;          // increment # bytes actually transfer

      }  /*for(ii=0; ii<= bytes_in; ii++, ptr++) */
    }    /* else  */


  } /* if(ready to receive data) */


 /* 01/08/01 $MH$: if New transaction yet clear EP0 OUT BCNT reg and NAK bit       to start data receiving */

  if(hOutEp0->DataFlags & USB_IOFLAG_NEW)
    {
       hOutEp0->DataFlags &= ~USB_IOFLAG_NEW;
       hOutEp0->DataFlags |= USB_IOFLAG_DATAREQ;
       if(hOutEp0->TotBytCnt == 0)
          hOutEp0->DataFlags |= USB_IOFLAG_DATAREQ;
    }

  if(hOutEp0->DataFlags & USB_IOFLAG_DATAREQ)
    {
       USB_EP0Reg->CT  = 0x00;
    }
}



/* -----------------11/30/00 $MH$  ----------------
Replaced USB_in_out_struct with USB_EpHandle
 --------------------------------------------------*/

#pragma CODE_SECTION (_vUSB_procCtrlInTransaction, ".text:usb_drvr")

//void _USB_procCtrlInTransaction(USB_EpHandle hInEp0)
void _vUSB_procCtrlInTransaction(USB_EpHandle hInEp0)
{
  int ii;
  USB_EP0_RegBlk *USB_EP0Reg = (USB_EP0_RegBlk *)hInEp0->EDReg_SAddr;

  /* ----------------11/30/00 $MH$------------------------
     since there is no DMA for EP0, use hOutEp0->DMA_SAddr
     to hold EP0 data buffer start addr
  -------------------------------------------------------*/
  ioport Uint16 *ptr = (ioport Uint16 *)hInEp0->DMA_SAddr;

  if(hInEp0->DataFlags & USB_IOFLAG_NEW)
  {
     hInEp0->DataFlags &= ~USB_IOFLAG_NEW;
     hInEp0->DataFlags |=  USB_IOFLAG_DATAREQ;
     if(hInEp0->TotBytCnt == 0)
        hInEp0->DataFlags |=  USB_IOFLAG_0BYTREQ;
  }

  /* -----------------12/9/99 3:16PM-------------------
  Transmit any data, if available
   --------------------------------------------------*/
  if((hInEp0->DataFlags & USB_IOFLAG_DATAREQ) && (USB_EP0Reg->CT & USBEPCT_NAK))
  {
    if(hInEp0->DataFlags & USB_IOFLAG_0BYTREQ)
    {
      ii = 0;
      hInEp0->DataFlags &= ~USB_IOFLAG_0BYTREQ;

      if((hInEp0->TotBytCnt == 0) || ((hInEp0->BytInThisSeg == 0) &&
                                      (hInEp0->pNextBuffer == NULL)))
      {
        hInEp0->DataFlags &= ~USB_IOFLAG_DATAREQ;
      }

    }
    else
    {
      for(ii=0; ii <= hInEp0->MaxPktSiz; ii++, ptr++)
      {

        if((hInEp0->TotBytCnt == 0) || ((hInEp0->pNextBuffer == NULL) && (hInEp0->BytInThisSeg == 0)))
        {
          if((ii == hInEp0->MaxPktSiz) && !(hInEp0->DataFlags & USB_IOFLAG_NOSHORT))
            {
              hInEp0->DataFlags |= USB_IOFLAG_0BYTREQ;
              //break;
            }
          else
            {
              hInEp0->DataFlags &= ~USB_IOFLAG_DATAREQ;
              //break;
            }
          break;
        }
        else if(hInEp0->BytInThisSeg == 0)
        {
          if(ii == hInEp0->MaxPktSiz)
          {
            if(!(hInEp0->DataFlags & USB_IOFLAG_NOSHORT) && !(hInEp0->DataFlags & USB_IOFLAG_CAT))
              {
                hInEp0->DataFlags |= USB_IOFLAG_0BYTREQ;
                //break;
              }
            //break;
          }
          _USB_gotoNextList(hInEp0);
                                                      // 03/26/01 $MH$ - bug fix
          if(!(hInEp0->DataFlags & USB_IOFLAG_CAT) || (ii == hInEp0->MaxPktSiz))
              break;
        }
        else if(ii == hInEp0->MaxPktSiz)
                break;


        if(hInEp0->DataFlags & USB_IOFLAG_BYTE)    /* USB second byte - MSB */
        {
           if (hInEp0->DataFlags & USB_IOFLAG_SWAP)
             *ptr = (*(hInEp0->pBuffer))&0x00FF;
           else
             *ptr = (*(hInEp0->pBuffer))>>8;       /* hi byte - no swap */

           hInEp0->pBuffer++;
           hInEp0->DataFlags &= ~USB_IOFLAG_BYTE;
        }
        else      /* USB first byte - LSB */
        {
           if (hInEp0->DataFlags & USB_IOFLAG_SWAP)
             *ptr = (*(hInEp0->pBuffer))>>8;       /* hi byte - swap */
           else
             *ptr = (*(hInEp0->pBuffer))&0x00FF;    /* lo byte - no swap */

           hInEp0->DataFlags |= USB_IOFLAG_BYTE;
        }

        /* -----------------11/30/00 $MH$------------------
        if BytInThisSeg = 0 then call link list handle routine
        instead of calling the routine and let it test BytInThisSeg
        counter
        --------------------------------------------------*/
        hInEp0->BytInThisSeg--;          // decrement # bytes remained to transfer

        if(!(hInEp0->DataFlags & USB_IOFLAG_EOLL))
           hInEp0->TotBytCnt--;

        if(hInEp0->XferBytCnt != NULL)      // 03/13/01 $MH$ to handle 0 byte ACK
           (*hInEp0->XferBytCnt)++;      // increment # bytes actually transfer



      }        /* for   */

    }          /* else */


    /* -----------------10/27/2000 3:24AM----------------
    Write the byte count to be transmitted.
     --------------------------------------------------*/
    USB_EP0Reg->CT = ii;

#ifdef DEBUG
    LOG_printf(&LOG_debug,"USB_proc_in: wrote %d bytes, %d bytes left\n",
      ii,USB_bytesRemaining(hInEp0));
#endif
  }   /* if xmit data is available */
}

/*
12/05/00 $MH$
function to process DMA endpoint transactions
needs work
*/

//*******************   03/08/01 $MH$ LIMITATIONS  ********************
//
//  This limitation apply only for Out Endpoints if and only if the
//  the data buffer is a linked list and USB_IOFLAG_CAT is set.  In
//  such condition if the host prematurely terminates the data transfer
//  (with or without a short packet) the driver fills up the data buffer
//  of the current node of the linked list and then waits for more data
//  from host to fill up the data buffers of the rest of the linked list.
//  This means the posted transation is still in progress and a call to
//  the routine USB_isTransactionDone(..) will return USB_FALSE.
//  If the current node happen to be the very last node of the linked
//  list the driver will treat this as a termination of transfer and
//  a call to the routine USB_isTransactionDone(..) will return USB_TRUE
//
//  Since the driver programs the DMA GO and RLD registers at the same time
//  (whenever possible) in order to move data more efficiently, if is beyond
//  the scope of the driver to anticipate an early termination of data
//  transfer and not program the DMA RLD registers
//
//  WORK AROUND:  If you have doubt that the host may prematurely termainate
//  the transfer then do not use the USB_IOFLAG_CAT flag with a linked list
//  for OUT transfers.
//
//  Affected Endpoints:  OUT[1..7]
//
//  THIS DOES NOT EFFECT ENDPOINT 0 IN/OUT TRANSFERS,  since endpoint0
//  trnasfer are not done by the USB dedicated DMA
//
//***********************************************************************

#pragma CODE_SECTION (_vUSB_procDmaTransaction, ".text:usb_drvr")

// USB_Boolean _USB_procDmaTransaction(USB_EpHandle hEp)

USB_Boolean _vUSB_procDmaTransaction(USB_EpHandle hEp)
{
  USB_DMA_Context *USB_DMA   = (USB_DMA_Context *)hEp->DMA_SAddr;
  Uint16 dma_ctrl_mask       = 0x00;
  Uint16 bytes_to_move = 0x00;


  /* -------------------------------------------------
  Following code is for DMA-enabled endpoints.
  --------------------------------------------------*/

    /* Is the byte order Big Endian? */
  if(hEp->DataFlags & USB_IOFLAG_SWAP) dma_ctrl_mask |= USBDCTL_END;

  /* Check to see if CAT must be set */
  if(hEp->DataFlags & USB_IOFLAG_CAT) dma_ctrl_mask |= USBDCTL_CAT;

  /* Check to see if SHT must be set */
  if(!(hEp->DataFlags & USB_IOFLAG_NOSHORT)) dma_ctrl_mask |= USBDCTL_SHT;

  switch(hEp->XferType)
  {
  /* -------------------------------------------------
      process bulk or interrupt transaction
  --------------------------------------------------*/
   case USB_BULK:
   case USB_INTR:
   case USB_ISO:
   {
        /* if both DMA GO and RLD bits are set then return false */
        if((USB_DMA->DCTL & USBDCTL_GO) && (USB_DMA->DCTL & USBDCTL_RLD))
            return(USB_FALSE);

        /* if DMA is free and a transaction is waiting */
        if(!(USB_DMA->DCTL & USBDCTL_GO) && (hEp->DataFlags & USB_IOFLAG_DATAREQ))
        {
          /* if buffer sent in incorrect format */
          // if(hEp->TotBytCnt < hEp->BytInThisSeg) return(USB_FALSE);

          /* Start a new DMA transfer */
          USB_DMA->DCT = 0;           /* shouldn't setting the GO bit clears the counter */
          /* how does Bytes work if it is not the 1st transaction of a xfer */
              /* ?? */

          /* convert word addr to byte addr and then load DMA regs */
          USB_DMA->DADL  = (((unsigned long)hEp->pBuffer) << 1) & 0xFFFF;
          USB_DMA->DADH  = (((unsigned long)hEp->pBuffer) >> 15) & 0xFFFF;
          /* USB_DMA->DMACTL = 0; - ?? */

          /* Set the GO bit to initiate DMA - may get interrupt by the time
             program sets the RLD, so it is better to GO and RLD at the same
             time
          USB_DMA->DMACTL = dma_ctrl_mask | USB_DMACTL_GO;
          */
          dma_ctrl_mask |= USBDCTL_GO;
          /* Check to see if GO was successful */
          /*if(!(USB_DMA->DMACTL & USB_DMACTL_GO)) return(USB_FALSE);*/

          if(hEp->DataFlags & USB_IOFLAG_EOLL)
            {
              bytes_to_move = hEp->BytInThisSeg;
            }
          else
            {
              bytes_to_move = (hEp->TotBytCnt < hEp->BytInThisSeg) ?
                               hEp->TotBytCnt : hEp->BytInThisSeg;
              hEp->TotBytCnt -= bytes_to_move;
            }

          USB_DMA->DSIZ = bytes_to_move;

          if(hEp->EpNum / 8)                     // if IN Endpt
             *hEp->XferBytCnt = bytes_to_move;   // increment # bytes actually transfer

          /* process linked list */

          // 02/14/01 $MH$ - replaced
          //if(hEp->DataFlags & USB_IOFLAG_LNK)
          //{
          //   if(hEp->TotBytCnt > 0)
          //      _USB_gotoNextList(hEp);
          //   else
          //      dma_ctrl_mask &= ~USB_DMACTL_CAT;
          //}

          // 02/15/01 $MH$ - replaced
          //if((hEp->pNextBuffer != NULL) && (hEp->TotBytCnt > 0))
          //   _USB_gotoNextList(hEp);
          //else
          //    dma_ctrl_mask &= ~USB_DMACTL_CAT;
          //
          if((hEp->pNextBuffer == NULL) || (hEp->TotBytCnt <= 0))
            {
              hEp->DataFlags &= ~USB_IOFLAG_DATAREQ;   /* flag EOT          */
              dma_ctrl_mask &= ~USBDCTL_CAT;             /* clear CAT if set  */
            }
          else
              _USB_gotoNextList(hEp);                       /* goto next link    */

        }


        /* How about DMA reload -  is it available, specially when hEp->TotBytCnt > 0  */
        if(!(USB_DMA->DCTL & USBDCTL_RLD) && (hEp->DataFlags & USB_IOFLAG_DATAREQ))
        {
          /* if buffer sent in incorrect format */
          // if(hEp->TotBytCnt < hEp->BytInThisSeg) return(USB_FALSE);
          /* check if link list and number of links are even then skip reload
             and clear the USB_IOFLAG_LNK from hEp->DataFlags so the _USB_evDispatch()will
             call this function agian to program the DMA without setting the CAT bit
          */
          if(((hEp->TotBytCnt <= hEp->BytInThisSeg) ||(hEp->pNextBuffer == NULL))
            && (hEp->DataFlags & USB_IOFLAG_CAT))
          {
             hEp->DataFlags &= ~USB_IOFLAG_CAT;
          }
          else
          {


             /* convert word addr to byte addr and then load DMA regs */
             USB_DMA->DRAL = (((unsigned long)hEp->pBuffer) << 1) & 0xFFFF;
             USB_DMA->DRAH = (((unsigned long)hEp->pBuffer) >> 15) & 0xFFFF;

             /* Enable the next transfer
             USB_DMA->DMACTL = (dma_ctrl_mask | USB_DMACTL_RLD);
             */
             dma_ctrl_mask |= USBDCTL_RLD;
             /* Check to see if RLD was successful */
             /* if(!(USB_DMA->DMACTL & USB_DMACTL_RLD)) */

             /* Reload failed. Increment the reload fail counter. */
             /* -----------------10/24/2000 6:01PM----------------
             The compiler doesn't like this.  Do we really need a
             reload fail counter?  I do see how it could be useful...
                            -- JDK
             USB_GlobalVars->ReloadCount[Endpoint]++;
             --------------------------------------------------*/
             /*  return(USB_FALSE);                             */

             if(hEp->DataFlags & USB_IOFLAG_EOLL)
               {
                 bytes_to_move = hEp->BytInThisSeg;
               }
             else
               {
                 bytes_to_move = (hEp->TotBytCnt < hEp->BytInThisSeg) ?
                                  hEp->TotBytCnt : hEp->BytInThisSeg;
                 hEp->TotBytCnt -= bytes_to_move;
               }

             /* Setup Reload transfer       */
             USB_DMA->DRSZ = bytes_to_move;

             if(hEp->EpNum / 8)                      // if IN Endpt
                *hEp->XferBytCnt = bytes_to_move;    // increment # bytes actually transfer

             /* process linked list */

             // 02/14/01 $MH$ - replaced
             // if((hEp->DataFlags & USB_IOFLAG_LNK) && (hEp->TotBytCnt > 0))

             // 02/14/01 $MH$ - replaced
             //if((hEp->pNextBuffer != NULL) && (hEp->TotBytCnt > 0))
             //    _USB_gotoNextList(hEp);

             if((hEp->pNextBuffer == NULL) || (hEp->TotBytCnt <= 0))
                hEp->DataFlags &= ~USB_IOFLAG_DATAREQ;          /* flag EOT          */
             else
                _USB_gotoNextList(hEp);                              /* goto next link    */
          }
        }

        USB_DMA->DCTL = dma_ctrl_mask;
        return(USB_TRUE);

   } /* case INTR  */

//   case USB_ISO:
//           return(USB_FALSE);

   default:            /* error if other xfer req */
           return(USB_FALSE);
 } /* end of switch(hEp->XferType)  */
}




#pragma CODE_SECTION (_vUSB_gotoNextList, ".text:usb_drvr")

// void _USB_gotoNextList(USB_EpHandle hEp)
void _vUSB_gotoNextList(USB_EpHandle hEp)
{
 /* -----------------11/30/00 $MH$--------------------------------------
  following two instructins are moved in the caller function
  hEp->BytInThisSeg--;
  if(hEp->BytInThisSeg == 0)
  {
  --------------------------------------------------*/
// 02/14/01 $MH$
//    if(hEp->pNextBuffer != NULL)
//    {
      hEp->BytInThisSeg = hEp->pNextBuffer->Bytes;
      hEp->pBuffer = hEp->pNextBuffer->pBuffer;
      hEp->pNextBuffer = hEp->pNextBuffer->pNextBuffer;

      hEp->XferBytCnt = hEp->pBuffer;
      *hEp->XferBytCnt = 0x0000;            // clear XferBytCnt field

      // if not OUT DMA endpoint advance the buffer pointer to the begining
      // of the actual data buffer
      if((hEp->EpNum == 0) || (hEp->EpNum == 8) || (hEp->EpNum / 8))
          hEp->pBuffer++;

      if(hEp->DataFlags & USB_IOFLAG_SWAP)
      {
        hEp->DataFlags |= USB_IOFLAG_BYTE;
      }
      else
      {
        hEp->DataFlags &= ~USB_IOFLAG_BYTE;
      }
//    }
}

/******************************************************************************/
/*  _USB_isr.c                                                                */
/*                                                                            */
/*  Renamed:   vUSB_evDispatch                                                */
/*                                                                            */
/*  This is the USB task dispatcher. It sets the events based on the eventss  */
/*  and calls the respective event handler routine pointed by the approprite  */
/*  endpoint object.  The event handler routines are supplied to the endpoint */
/*  object during the endpoint object initialization phase                    */
/*                                                                            */
/* Author: MH                                                                 */
/* Date: 24 Nov 2000                                                          */
/* Based on : Venkat Raghavan (TI India)                                      */
/*                                                                            */
/******************************************************************************/
/*                                                                            */
/*    Modified: 10 Jan 01 $MH$                                                */
/*                                                                            */
/*    Replaced USB_in_out_structs with USB_EpHandles.  Use USB_DevNum if      */
/*    multiple USB module is supported in the future.                         */
/*                                                                            */
/*    Last modified: 02 Apr 01 $MH$                                           */
/*                                                                            */
/*    Moved _USB_isr( ) from usb_disptch.c to put all the USB driver code in  */
/*    single file                                                             */
/*                                                                            */
/* 28 Sep 2001 $MH$                                                           */
/*                                                                            */
/* Converted INTERRUPT _vUSB_isr() to void _vUSB_evDispatch(void), so the     */
/* module can be used with DSP/BIOS                                           */
/*                                                                            */
/******************************************************************************/

#define USBCTL_ACTIVE (USBCTL_CONN | USBCTL_FEN | USBCTL_FRSTE)

/*
12/13/00 $MH$:  replaced
extern USB_GlobalVars_struct USB_GlobalVars;
with USB_GlobalVars_struct *USB_GlobalVars = (USB_GlobalVars_struct *)((unsigned long)USB_SRAM_END);
*/

extern USB_GlobalVars_struct USB_GlobalVars;

// extern USB_GlobalVars_struct *USB_GlobalVars;

// #pragma CODE_SECTION (_vUSB_isr, ".text:usb_drvr")
// interrupt void _USB_isr()
// interrupt void _vUSB_isr()

#pragma CODE_SECTION (vUSB_evDispatch, ".text:usb_drvr")

void vUSB_evDispatch(void)
{
  // Uchar jj;
  Uint16 ii;
  Uint16 Vector, EpIndex;
  Uint16 usb_bus_events = 0;
  /*12/01/00 $MH$:
  Replaced USB_in_out_structs with USB_EpHandles
  USB_INOUT_STRUCT *USB_in, *USB_out;
  */
  USB_EpHandle hEpx, hEp0In, hEp0Out;
  USB_EP0_RegBlk *USB_EP0InReg, *USB_EP0OutReg;

  /* SETUP packet can be found at 0x6700 in Amadeus */
  ioport Uint16 *USB_SetupPkt = (ioport Uint16 *)USBSUP_BUF;
  /* USB_GlobalVars_struct *USB_GlobalVars = (USB_GlobalVars_struct *)USB_GLOBALVAR_BASE; */

  hEp0In  = (USB_EpHandle )(((unsigned long)USB_GlobalVars.EpHandleTable[8][2]) |
                            (((unsigned long)USB_GlobalVars.EpHandleTable[8][1]) << 8) |
                            (((unsigned long)USB_GlobalVars.EpHandleTable[8][0]) << 16));

  hEp0Out = (USB_EpHandle )(((unsigned long)USB_GlobalVars.EpHandleTable[0][2]) |
                            (((unsigned long)USB_GlobalVars.EpHandleTable[0][1]) << 8) |
                            (((unsigned long)USB_GlobalVars.EpHandleTable[0][0]) << 16));

  USB_EP0InReg  = (USB_EP0_RegBlk *)hEp0In->EDReg_SAddr;
  USB_EP0OutReg = (USB_EP0_RegBlk *)hEp0Out->EDReg_SAddr;


  /* Read the vector to be serviced next */
  Vector = USBINTSRC;

  /* init hEp with EP0 OUT handle */




  /* -----------------10/27/2000 0:38AM----------------
  Record the event in the EventTable.
   --------------------------------------------------*/
  if((Vector >= USBINTSRC_ODMARLD1) && (Vector <= USBINTSRC_IDMAGO7))
  {
    EpIndex = (Vector - USBINTSRC_DMA_START) >> 1;

    hEpx = (USB_EpHandle )(((unsigned long)USB_GlobalVars.EpHandleTable[EpIndex][2]) |
                           (((unsigned long)USB_GlobalVars.EpHandleTable[EpIndex][1]) << 8) |
                           (((unsigned long)USB_GlobalVars.EpHandleTable[EpIndex][0]) << 16));

    /*
    12/24/00 $MH$
    change proc_in proc_out and proc_DMA to set USB_EVENT_EOT
    before exiting the routines - else the following code won't work
    */

    if(!USB_isTransactionDone(hEpx))
        _USB_procDmaTransaction(hEpx);

    else    /* transaction done so call the event isr */
    {
      hEpx->EventFlag |= USB_EVENT_EOT;
      if((hEpx->EventMask & USB_EVENT_EOT) && (hEpx->Fxn != (USB_EVENT_ISR)0))
         hEpx->Fxn();
    }
  }

  else
  {
    switch(Vector)
    {
      case USBINTSRC_IEP0:
      {
        /*
        01/09/01 $MH$: call USB_proc_in again if the transaction is not done or a 0-byte
                       short packet is necessary to end the transfer,  if USB_isTransactionDone(hEp0In)
                       returns TRUE and USB_IOFLAG_NEW means 0-byte short packet required.
                       USB_IOFLAG_NEW can be replaced with USB_IOFLAG_DATAREQ to make
                       the code more sensible to the reader.
        */
        //if((!USB_isTransactionDone(hEp0In)) || (hEp0In->DataFlags & (USB_IOFLAG_DATAREQ | USB_IOFLAG_NEW)))
        //if((!USB_isTransactionDone(hEp0In)) || (hEp0In->DataFlags & USB_IOFLAG_DATAREQ))
        if(hEp0In->DataFlags & USB_IOFLAG_DATAREQ)
           _USB_procCtrlInTransaction(hEp0In);

        else
        {
          hEp0In->EventFlag |= USB_EVENT_EOT;

          /*
          01/10/01 $MH$:
          if EP0 OUT transfer pending then start the transfer - this code is necessary to
          by pass the USB module restrictions:- clearing both EP0 IN and OUT NAK at the same
          causes the module to fail data transfer
          */
          //if(USB_isTransactionDone(hEp0Out)) _USB_procCtrlOutTransaction(hEp0Out);

          if(hEp0Out->DataFlags & (USB_IOFLAG_DATAREQ | USB_IOFLAG_NEW))
          _USB_procCtrlOutTransaction(hEp0Out);

          /*
          01/10/01 $MH$:
          if EventMask is set call function associated with USB_EVENT_EOT
          */
          if((hEp0In->EventMask & USB_EVENT_EOT) && (hEp0In->Fxn != (USB_EVENT_ISR)0))
             hEp0In->Fxn();
        }

        break;
      }

      /*
      12/12/00 $MH$:
      if one of the following events took place no need for retrieving
      EP0 handle since hEp has already been initialized with EP0 OUT
      handle at the beginning of this ISR
      */
      /*
      12/24/00 $MH$
      change proc_out to set USB_EVENT_EOT
      before exiting the routines - else the following code won't work
      */
      case USBINTSRC_OEP0:
      {
        if(hEp0Out->DataFlags & USB_IOFLAG_DATAREQ)
           _USB_procCtrlOutTransaction(hEp0Out);

        if(hEp0Out->EventFlag & USB_EVENT_EOT)
        {
          /*
          01/10/01 $MH$:
          if EP0 IN transfer pending then start the transfer - this code is necessary to
          by pass the USB module restrictions:- clearing both EP0 IN and OUT NAK at the same
          causes the module to fail data transfer
          */
          //if((!USB_isTransactionDone(hEp0In)) || (hEp0In->DataFlags & (USB_IOFLAG_DATAREQ | USB_IOFLAG_NEW)))
          if(hEp0In->DataFlags & (USB_IOFLAG_DATAREQ | USB_IOFLAG_NEW))
             _USB_procCtrlInTransaction(hEp0In);

          /*
          01/10/01 $MH$:
          if EventMask is set call function associated with USB_EVENT_EOT
          */
          if((hEp0Out->EventMask & USB_EVENT_EOT) && (hEp0Out->Fxn != (USB_EVENT_ISR)0))
             hEp0Out->Fxn();
        }

        break;
      }

      case USBINTSRC_RSTR:
      {
        usb_bus_events = USB_EVENT_RESET;
        break;
      }

      case USBINTSRC_SUSR:
      {
        usb_bus_events = USB_EVENT_SUSPEND;
        break;
      }

      case USBINTSRC_RESR:
      {
        usb_bus_events = USB_EVENT_RESUME;
        break;
      }

      case USBINTSRC_SETUP:
      case USBINTSRC_STPOW:
      {
        /* -----------------10/27/2000 1:57AM----------------
        Do we need not to call the SETUP handler if STPOW is
         set and we got a SETUP (not STPOW) event?  Having
         STPOW set would mean that we'll get here again.
          -- JDK
         --------------------------------------------------*/

        /*
        01/05/01 $MH$:  prepare endpoint0 in and out for handling setup packet
        both endpoint NAK bits have to set for handling early hand shake
        */
        usb_bus_events = USB_EVENT_SETUP;
        USBCTL = USBCTL_ACTIVE | USBCTL_SETUP;

        /*
        01/08/01 $MH$:  check bmRequest Type, if data IN set DIR for handshake
                        else data in OUT, let the DIR be 0 in order to avoid
                        getting interrupt on the 0 byte handshake packet
        */

        if(USB_SetupPkt[0] & 0x80)
           USBCTL |= USBCTL_DIR;
        /*
        set NAK bit of EP0 IN BCNT reg
        clear TotBytCnt field of EP0 IN handle
        clear Flags field of EP0 IN handle
        */

        USB_EP0InReg->CT     = USBEPCT_NAK;
        hEp0In->DataFlags    = 0x00;
        hEp0In->Status       = 0x00;
        hEp0In->TotBytCnt    = 0x00;
        hEp0In->BytInThisSeg = 0x00;
        hEp0In->pBuffer      = NULL;
        hEp0In->pNextBuffer  = NULL;
        hEp0In->EventFlag    = 0x00;

        /*
        set NAK bit of EP0 IN BCNT reg
        clear TotBytCnt field of EP0 IN handle
        clear Flags field of EP0 IN handle
        */

        USB_EP0OutReg->CT     = USBEPCT_NAK;
        hEp0Out->DataFlags    = 0x00;
        hEp0Out->Status       = 0x00;
        hEp0Out->TotBytCnt    = 0x00;
        hEp0Out->BytInThisSeg = 0x00;
        hEp0Out->pBuffer      = NULL;
        hEp0Out->pNextBuffer  = NULL;
        hEp0Out->EventFlag    = 0x00;

        break;
      }

      case USBINTSRC_SOF:
      {
        usb_bus_events = USB_EVENT_SOF;
        break;
      }

      case USBINTSRC_PRESOF:
      {
        usb_bus_events = USB_EVENT_PSOF;
        break;
      }

      case USBINTSRC_HINT:
      {
        usb_bus_events = USB_EVENT_HINT;
        break;
      }

      case USBINTSRC_HERR:
      {
        usb_bus_events = USB_EVENT_HERR;
        break;
      }

      default: break;
    }
  }

  /* -----------------01/05/00 $MH$ ----------------
  if event is other than endpoint event
   --------------------------------------------------*/
  if(usb_bus_events)
  {
    // 04/14/01 $MH$: bug fix
    // jj = USB_GlobalVars->ActiveEndptNos;

    for(ii = 0; ii < MAX_ENDPT; ii++)
    {
      hEpx = (USB_EpHandle )(((unsigned long)USB_GlobalVars.EpHandleTable[ii][2]) |
                             (((unsigned long)USB_GlobalVars.EpHandleTable[ii][1]) << 8) |
                             (((unsigned long)USB_GlobalVars.EpHandleTable[ii][0]) << 16));

      if((hEpx != (USB_EpHandle )0) && (hEpx->EventMask & usb_bus_events) &&
         (hEpx->Fxn != (USB_EVENT_ISR)0))
      {
         hEpx->EventFlag |= usb_bus_events;
         hEpx->Fxn();
      }
    }
  }

  /*
  01/08/01 $MH$:  if returning from SETUP packet handler clear the SETUP and STPOW
                  flag by writing 1 to the USB STATUS register
                  ** NOT A GOOD IDEA IF THE EVENTS ARE HANDLED BY SWI **  for now
                  leaving the burden of clearing the SETUP and STPOW flag on the
                  users.
  */
  // clear setup and setup overwrite flag if set
  if(usb_bus_events == USB_EVENT_SETUP)
     USBIF |= USB_INT_SETUP|USB_INT_STPOW;

  return;
}

#endif
