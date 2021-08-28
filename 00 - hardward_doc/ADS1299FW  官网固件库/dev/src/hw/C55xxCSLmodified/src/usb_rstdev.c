/******************************************************************************/
/* Name     :  USB_resetDev                                                   */
/*                                                                            */
/* Catagory :  Device Control                                                 */
/*                                                                            */
/* Purpose  :  Reset the USB module.                                          */
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
/* Once the module has been reset, all the control and status registers are   */
/* returned to powerup reset values and the USB module is diconnected from    */
/* the upstream port.                                                         */
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

#pragma CODE_SECTION (vUSB_resetDev, ".text:vUSB_resetDev")

void vUSB_resetDev(USB_DevNum DevNum)
{
    (USBGCTL |= USBGCTL_RST);
}
#endif
      
