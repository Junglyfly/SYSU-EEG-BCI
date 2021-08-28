

/******************************************************************************/
/* Name     : USB_getRemoteWakeupStat                                         */
/*                                                                            */
/* Catagory : Status / Query                                                  */
/*                                                                            */
/* Purpose  : get the status of the Remote Wakeup Feature (whether the        */
/*            the feature is set or clear in the software)                    */
/*                                                                            */
/*                                                                            */
/* Author   : MH                                                              */
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
/*============================================================================*/
/* Return Value:                                                              */
/*                                                                            */
/* USB_TRUE if the Remote Wakeup Feature is enabled in the software.          */
/* USB_FALSE if the Remote Wakeup Feature is disabled in the software.        */
/*                                                                            */
/*============================================================================*/
/* Comments:                                                                  */
/*                                                                            */
/* An application must chech if the remote wakeup feature is set by the host  */
/* before calling USB_issueRemoteWakeup( )                                    */
/*                                                                            */
/*============================================================================*/
/* History:                                                                   */
/*                                                                            */
/* Created:    31 Mar 2001                                                    */
/*                                                                            */
/******************************************************************************/
#define _USB_MOD

#include <csl_chiphal.h>
#include <csl_usbhal.h>
#include <_csl_usb.h>
#include <csl_usb.h>

#if (_USB_SUPPORT)

extern USB_GlobalVars_struct USB_GlobalVars;

#pragma CODE_SECTION (vUSB_getRemoteWakeupStat, ".text:vUSB_getRemoteWakeupStat")

USB_Boolean vUSB_getRemoteWakeupStat(USB_DevNum DevNum)
{
  /* USB_GlobalVars_struct *USB_GlobalVars = (USB_GlobalVars_struct *)USB_GLOBALVAR_BASE; */
  return(USB_GlobalVars.RemoteWkpStat);
}
#endif

