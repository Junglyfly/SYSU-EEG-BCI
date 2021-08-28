/******************************************************************************/
/* Name     : USB_stallEndpt                                                  */
/*                                                                            */
/* Catagory : Device Control                                                  */
/*                                                                            */
/* Purpose  : Stall an endpoint                                               */
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
/* None                                                                       */
/*                                                                            */ 
/*============================================================================*/
/* Comments:                                                                  */
/*                                                                            */
/* The endpoint handle determines the endpoint to stall                       */
/*                                                                            */
/*============================================================================*/
/* History:                                                                   */
/*                                                                            */
/* Created:    28 Nov 2000                                                    */
/*                                                                            */
/******************************************************************************/


/* ---------------------------------------------------------------------------
 STALL the corresponding endpoint. Further requests to the endpoint will
 see a STALL on the endpoint.
 11/29/00 $MH$: Modified
 12/01/00 $MH$: 
 Replaced by the vUSB_stallEndpoint(USB_DevNum DevNum,Uint16 EpNum)
 12/18/00 $MH$ reinstated
 
 Last Modified: 6th May 2002
 Modified by  : Ruchika
 Modification : The TOGGLE bit in the EDB Configuration register is cleared to 
 				indicate the next buffer to be the X buffer
  	            The USBALT register set to indicate that DMA points to the DATA0 
  	            buffer.
 ---------------------------------------------------------------------------*/
#define _USB_MOD

#include <csl_chiphal.h>

#if (_USB_SUPPORT)

#include <csl_usbhal.h>
#include <_csl_usb.h>
#include <csl_usb.h>


#pragma CODE_SECTION (vUSB_stallEndpt, ".text:vUSB_stallEndpt")

void vUSB_stallEndpt(USB_EpHandle hEp)
{
  USB_EDRegBlk *USB_EDB       = (USB_EDRegBlk *)hEp->EDReg_SAddr;
  USB_EP0_RegBlk *USB_EP0Reg  = (USB_EP0_RegBlk *)hEp->EDReg_SAddr;
  Uint16 EpIndex              = (Uint16)hEp->EpNum;
#ifndef DISABLE_USB_TOGGLE_BUGFIX
  Uint16 AltMask;
#endif

  if(EpIndex % 8)       /* STALL endpoints other than OUT EP0 and IN EP0 */
  {
    USB_EDB->CNF |= USBEPCNF_STALL;
    
#ifndef DISABLE_USB_TOGGLE_BUGFIX
    /* USB DMA should point to the DATA0 (X buffer) */
    /* Check if the register value at 0x5840 is clear or not 
       If not then the bit can be cleared by writing a 1 to it */

    USB_EDB->CNF &=0x00DF ;

    AltMask = 0x1 << hEp->EpNum; /* Compute the Mask Value*/

    /*If TRUE then the DMA is currently pointing to the Y buffer */
    if (USBALT & AltMask)
    {
      USBALT|=AltMask;           
    }
#endif         
  }
  else                  /* STALL the EP0 (IN or OUT) */
  {
    USB_EP0Reg->CNF |= USBEPCNF_STALL;
  }
  
  return;
}
#endif
