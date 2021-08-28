/******************************************************************************/
/* Name     :  USB_setDevAddr                                                 */
/*                                                                            */
/* Catagory :  Device Control                                                 */
/*                                                                            */
/* Purpose  :  Set USB device address                                         */
/*                                                                            */
/* Author   :  MH                                                             */
/*                                                                            */
/* Based on :                                                                 */
/*                                                                            */
/*                                                                            */
/*============================================================================*/
/* Arguments:                                                                 */
/*                                                                            */
/* DevNum   : USB device number, enumerated data type of USB_DevNum.          */
/*            Only USB0 is active currently                                   */
/*                                                                            */
/* addr     : 7-bit USB device address                                        */
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

#if (_USB_SUPPORT)

#include <csl_usb.h>
#include <_csl_usb.h>
#include <csl_usb.h>


#pragma CODE_SECTION (vUSB_setDevAddr, ".text:vUSB_setDevAddr")

void vUSB_setDevAddr(USB_DevNum DevNum, Uchar addr)
{
  USBADDR = addr & 0x7F;
}  
#endif

