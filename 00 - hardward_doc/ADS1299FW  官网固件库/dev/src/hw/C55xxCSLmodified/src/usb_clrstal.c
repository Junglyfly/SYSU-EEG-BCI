/******************************************************************************/
/* Name     : USB_clearEndptStall                                             */
/*                                                                            */
/* Catagory : Device Control                                                  */
/*                                                                            */
/* Purpose  : Clear an endpoint stall                                         */
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
/* The endpoint handle determines the endpoint to bring out of stall          */
/*                                                                            */
/*============================================================================*/
/* History:                                                                   */
/*                                                                            */
/* Created:    28 Nov 2000                                                    */
/*                                                                            */
/******************************************************************************/
#define _USB_MOD

#include <csl_chiphal.h>
#include <csl_usbhal.h>
#include <_csl_usb.h>
#include <csl_usb.h>

#if (_USB_SUPPORT)

#pragma CODE_SECTION (vUSB_clearEndptStall, ".text:vUSB_clearEndPtStall")

/* ----------------------------------------------------------------------------
 Clear the STALL on the endpoint. Requests to the endpoint will be handled
 after the call returns.
 11/29/00$MH$: Modified
 12/01/00 $MH$: 
 Replaced by the vUSB_clearEndpointStall(USB_DevNum DevNum,Uint16 EpNum)
 12/18/00 $MH$ reinstated
 ----------------------------------------------------------------------------*/
// void USB_clearEndptStall(USB_EpHandle hEp)
void vUSB_clearEndptStall(USB_EpHandle hEp)
{
  USB_EDRegBlk   *USB_EDB      = (USB_EDRegBlk *)hEp->EDReg_SAddr;
  USB_EP0_RegBlk *USB_EP0Reg   = (USB_EP0_RegBlk *)hEp->EDReg_SAddr;
  Uint16         EpIndex       = (Uint16)hEp->EpNum;

  if(EpIndex % 8)       //clear STALLed Endpoints other than OUT EP0 and IN EP0 
  {
    USB_EDB->CNF &= ~USBEPCNF_STALL;
  }
  else                  // clear STALLed EP0 (IN or OUT)
  {
    USB_EP0Reg->CNF &= ~USBEPCNF_STALL;
  }

  return;
}

#endif

