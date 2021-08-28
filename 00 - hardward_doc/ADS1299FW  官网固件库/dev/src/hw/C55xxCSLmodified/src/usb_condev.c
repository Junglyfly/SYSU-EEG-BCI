/******************************************************************************/
/* Name     :  USB_connectDev                                                 */
/*                                                                            */
/* Catagory :  Device Control                                                 */
/*                                                                            */
/* Purpose  :  Connect the USB module to upstream port (D+ pullup enabled)    */
/*                                                                            */
/*                                                                            */
/* Author   :  MH                                                             */
/*                                                                            */
/* Based on :                                                                 */
/*                                                                            */
/*                                                                            */
/*============================================================================*/
/* Arguments:                                                                 */
/*                                                                            */
/* DevNum:    USB device number, enumerated data type of USB_DevNum.          */
/*            Only USB0 is active currently                                   */
/*                                                                            */
/*                                                                            */
/*                                                                            */
/*                                                                            */
/*                                                                            */
/*                                                                            */
/*============================================================================*/
/* Return Value:                                                              */
/*                                                                            */
/* None                                                                       */
/*                                                                            */
/*============================================================================*/
/* Comments:                                                                  */
/*                                                                            */
/*                                                                            */
/*                                                                            */
/*============================================================================*/
/* History:                                                                   */
/*                                                                            */
/* Created:    30 Mar 2001                                                    */
/*                                                                            */
/******************************************************************************/
#define _USB_MOD

#include <csl_chiphal.h>
#include <csl_usbhal.h>
#include <_csl_usb.h>
#include <csl_usb.h>

#if (_USB_SUPPORT)

#pragma CODE_SECTION (vUSB_connectDev, ".text:vUSB_connectDev")

void vUSB_connectDev(USB_DevNum DevNum)
{
  USBCTL |= USBCTL_CONN;
}

#endif
