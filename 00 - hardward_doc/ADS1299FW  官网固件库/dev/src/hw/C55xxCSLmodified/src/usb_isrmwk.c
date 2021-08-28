/******************************************************************************/
/* Name     : USB_issueRemoteWakeup                                           */
/*                                                                            */
/* Catagory : Device Control                                                  */
/*                                                                            */
/* Purpose  : Issue a remote wakeup signal to the host.                       */
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
/* Host must set the remote wake up feature first. An application must chech  */
/* if the remote wake-up feature is before generating a remote wake-up signal */
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

extern USB_GlobalVars_struct USB_GlobalVars;

#pragma CODE_SECTION (vUSB_issueRemoteWakeup, ".text:vUSB_issueRemoteWakeup")

USB_Boolean vUSB_issueRemoteWakeup(USB_DevNum DevNum)
{
   USB_Boolean ret_stat;
   /* USB_GlobalVars_struct *USB_GlobalVars = (USB_GlobalVars_struct *)USB_GLOBALVAR_BASE; */

   if(USB_GlobalVars.RemoteWkpStat)
   {
      USBCTL |= USBCTL_RWUP;
      ret_stat = USB_TRUE;
   }

   else
      ret_stat = USB_TRUE;

   return(ret_stat);
}

#endif
