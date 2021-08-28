/******************************************************************************/
/* Name     : USB_epNumToHandle                                               */
/*                                                                            */
/* Catagory : Misc                                                            */
/*                                                                            */
/* Purpose  : Retrieve an handle to an endpoint                               */
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
/* Endpt    : 8-bit endpoint number as specified in the USB specification     */
/*            0x00 -> Endpt 0 Out, 0x01 -> Endpt 1 Out ....                   */
/*            0x80 -> Endpt 0 In,  0x81 -> Endpt 1 In  ....                   */
/*                                                                            */
/*============================================================================*/
/* Return Value:                                                              */
/*                                                                            */
/* An handle to the endpoint object if a valid endpoint object exists, else   */
/* a NULL handle                                                              */
/*                                                                            */
/*============================================================================*/
/* Comments:                                                                  */
/*                                                                            */
/* This routine is helpful when the application does not have any prior       */
/* knowledge of the endpoint it is dealing with.  For example, if the host    */
/* requests to stall an endpoint the application can read the endpoint number */
/* from the setup packet, retrieve the handle to this endpoint by calling     */
/* USB_epNumToHandle(..), and call USB_stallEndpt(..) with the handle to      */
/* stall the ednpoint;                                                        */
/*                                                                            */
/*============================================================================*/
/* History:                                                                   */
/*                                                                            */
/* Created:    12 Dec 2000                                                    */
/*                                                                            */
/******************************************************************************/
#define _USB_MOD

#include <csl_chiphal.h>
#include <csl_usbhal.h>
#include <_csl_usb.h>
#include <csl_usb.h>


#if (_USB_SUPPORT)

extern USB_GlobalVars_struct USB_GlobalVars;

#pragma CODE_SECTION (vUSB_epNumToHandle, ".text:vUSB_epNumToHandle.usb_api")

/*
12/12/00 $MH$
Function to retrieve endpoint handle by passing actual endpoint
number as a parameter.  This function may come handy during USB SETUP
request handling
*/
// USB_EpHandle USB_epNumToHandle(USB_DevNum DevNum, Uchar Endpt)
USB_EpHandle vUSB_epNumToHandle(USB_DevNum DevNum, Uchar Endpt)
{
  USB_EpHandle          hEp;
  /* USB_GlobalVars_struct *USB_GlobalVars = (USB_GlobalVars_struct *)USB_GLOBALVAR_BASE; */

  // generate endpoint index from actual endpoint number
  Uint16 EpIndex = ((Endpt & 0x80)>>4)|(Endpt & 0x07);

  hEp = (USB_EpHandle )(((Uint32)(USB_GlobalVars.EpHandleTable[EpIndex][2] & 0xFF)) |
                        (((Uint32)(USB_GlobalVars.EpHandleTable[EpIndex][1] & 0xFF)) << 8) |
                        (((Uint32)(USB_GlobalVars.EpHandleTable[EpIndex][0] & 0xFF)) << 16));
  return(hEp);
}
#endif
