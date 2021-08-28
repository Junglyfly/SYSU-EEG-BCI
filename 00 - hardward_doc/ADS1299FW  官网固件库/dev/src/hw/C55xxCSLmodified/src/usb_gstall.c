/******************************************************************************/
/* Name     : USB_getEndptStall                                               */
/*                                                                            */
/* Catagory : Status / Query                                                  */
/*                                                                            */
/* Purpose  : Find out if an endpoint is stalled                              */
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
/* USB_TRUE if the endpoint is stalled, otherwise USB_FALSE                   */
/*                                                                            */ 
/*============================================================================*/
/* Comments:                                                                  */
/*                                                                            */
/* The endpoint handle selects the endpoint                                   */
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

#pragma CODE_SECTION (vUSB_getEndptStall, ".text:vUSB_getEndptStall")

/* -----------------------------------------------------------------------
   Get the status of the STALL on the endpoint. If the endpoint is STALLed,
   the function returns USB_TRUE.
   11/29/00$MH$: Modified
   12/01/00 $MH$: 
   Replaced by the vUSB_getEndpointStall(USB_DevNum DevNum,Uint16 EpNum)
   12/18/00 $MH$ reinstated
 ----------------------------------------------------------------------------*/
// USB_Boolean USB_getEndptStall(USB_EpHandle hEp)
USB_Boolean vUSB_getEndptStall(USB_EpHandle hEp)
{
  USB_EDRegBlk   *USB_EDB    = (USB_EDRegBlk *)hEp->EDReg_SAddr;
  USB_EP0_RegBlk *USB_EP0Reg = (USB_EP0_RegBlk *)hEp->EDReg_SAddr;
  Uint16         EpIndex     = (Uint16)hEp->EpNum;
  USB_Boolean    ret_stat    = USB_FALSE;

  if(EpIndex % 8)       // for endpoints other than OUT EP0 and IN EP0 
  {
    if(USB_EDB->CNF & USBEPCNF_STALL) 
       ret_stat= USB_TRUE;
  }
  else                    // for EP0 (IN or OUT) 
  {
    if(USB_EP0Reg->CNF & USBEPCNF_STALL)
       ret_stat= USB_TRUE;
  }

  return(ret_stat);
}

#endif
