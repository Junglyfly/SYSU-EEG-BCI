/******************************************************************************/
/* Name     : USB_getSetupPacket                                              */
/*                                                                            */
/* Catagory : Data Transfer                                                   */
/*                                                                            */
/* Purpose  : Read the setup packet from the setup data buffer                */
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
/* DevNum     : USB device number, enumerated data type of USB_DevNum.        */
/*              Only USB0 is active currently                                 */
/*                                                                            */
/* *USB_Setup : Pointer to a structure of type USB_SetupStruct.               */
/*                                                                            */
/*                                                                            */
/*============================================================================*/
/* Return Value:                                                              */
/*                                                                            */
/* USB_TRUE if successful else USB_FALSE. If successful successful USB_Setup  */
/* structure holds the new setup packet                                       */
/*                                                                            */
/*============================================================================*/
/* Comments:                                                                  */
/*                                                                            */
/*                                                                            */
/*                                                                            */
/*============================================================================*/
/* History:                                                                   */
/*                                                                            */
/* Created:    28 Nov 2000                                                    */
/*                                                                            */
/******************************************************************************/

/* ---------------------------------------------------------------------------
 Read the SETUP packet. 
 11/28/00 $MH$ Added DevNum for future use for DSP with multiple USB module
 ----------------------------------------------------------------------------*/
#define _USB_MOD

#include <csl_chiphal.h>

#if (_USB_SUPPORT)

#include <csl_usbhal.h>
#include <_csl_usb.h>
#include <csl_usb.h>


#pragma CODE_SECTION (vUSB_getSetupPacket, ".text:vUSB_getSetupPacket")

// USB_Boolean USB_getSetupPacket(USB_DevNum DevNum, USB_SetupStruct *USB_Setup)
USB_Boolean vUSB_getSetupPacket(USB_DevNum DevNum, USB_SetupStruct *USB_Setup)
{
  // SETUP packet can be found at 0x6700 of 5509 i/o space
  ioport Uint16 *Setup_pkt = (ioport Uint16 *)USBSUP_BUF;

  // Read the SETUP packet */
  USB_Setup->New           = 1;
  USB_Setup->bmRequestType = Setup_pkt[0];
  USB_Setup->bRequest      = Setup_pkt[1];
  USB_Setup->wValue        = Setup_pkt[2] | (Setup_pkt[3]<<8);
  USB_Setup->wIndex        = Setup_pkt[4] | (Setup_pkt[5]<<8);
  USB_Setup->wLength       = Setup_pkt[6] | (Setup_pkt[7]<<8);

  return(USB_TRUE);
}

#endif
