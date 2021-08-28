/******************************************************************************/
/* Name     : USB_setRemoteWakeup                                             */
/*                                                                            */
/* Catagory : Software Control                                                */
/*                                                                            */
/* Purpose  : Set of clear Remote Wakeup Feature                              */
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
/* RmtWkpStat:  If USB_TRUE the driver will set remote wakeup feature         */
/*              and a subsequent call to USB_issueRemoteWakeup( ) will cause  */
/*              the driver to generate a remote signal on the bus             */
/*                                                                            */
/*              If USB_FALSE the driver will clear remote wakeup feature      */
/*              and a subsequent call to USB_issueRemoteWakeup( ) will not    */
/*              generate a remote signal on the bus                           */
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
/* Host must set the remote wake up feature first. An application must chech  */
/* if the remote wake-up feature is before generating a remote wake-up signal */
/*                                                                            */
/*============================================================================*/
/* History:                                                                   */
/*                                                                            */
/* Created:    31 Mar 2001                                                    */
/*                                                                            */
/******************************************************************************/
#define _USB_MOD

#include <csl_chiphal.h>

#if (_USB_SUPPORT)

#include <csl_usbhal.h>
#include <_csl_usb.h>
#include <csl_usb.h>


extern USB_GlobalVars_struct USB_GlobalVars;

#pragma CODE_SECTION (vUSB_setRemoteWakeup, ".text:vUSB_setRemoteWakeup")

void vUSB_setRemoteWakeup(USB_DevNum DevNum, USB_Boolean RmtWkpStat)
{
   /* USB_GlobalVars_struct *USB_GlobalVars = (USB_GlobalVars_struct *)USB_GLOBALVAR_BASE; */
   USB_GlobalVars.RemoteWkpStat         = RmtWkpStat;
}
#endif

