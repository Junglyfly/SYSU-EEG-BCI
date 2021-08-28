/******************************************************************************/
/* Name     : _USB_enableEpIntrpt                                             */
/*                                                                            */
/* Catagory : Device Initialization (private function)                       */
/*                                                                            */
/* Purpose  : Enable the endpoint interrupt.  Affected endpoint is determined */
/*            by the endpoint objects passed to the function               */
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
/* hEp      : Handle to an initialized endpoint object                        */
/*                                                                            */
/*============================================================================*/
/* Return Value:                                                              */
/*                                                                            */
/* USB_TRUE if the endpoint interrupt is successfully enabled, else USB_FALSE */
/*                                                                            */ 
/*============================================================================*/
/* Comments:                                                                  */
/*                                                                            */
/*                                                                            */
/*============================================================================*/
/* History:                                                                   */
/*                                                                            */
/* Created:    21 Dec 2000                                                    */
/*                                                                            */
/******************************************************************************/
#define _USB_MOD

#include <csl_chiphal.h>
#include <csl_usbhal.h>
#include <_csl_usb.h>
#include <csl_usb.h>

#if (_USB_SUPPORT)

#pragma CODE_SECTION (_vUSB_enableEpIntrpt, ".text:_vUSB_enableEpIntrpt")

// USB_Boolean _USB_enableEpIntrpt(USB_DevNum DevNum, USB_EpHandle hEp)
USB_Boolean _vUSB_enableEpIntrpt(USB_DevNum DevNum, USB_EpHandle hEp)
{
  Uint16 EpIndex    = (Uint16)hEp->EpNum;
  Uint16 bit_addr;
  
  bit_addr = EpIndex & 0x07; /* addr of endpoint intrpt mask bit */
  
  if(EpIndex & 0x08)         /* test for in-endpoints */
  {  
    USBIEPIE |= (1 << bit_addr);
  }
  else                       /* out-endpoint */
  {  
    USBOEPIE |= (1 << bit_addr);
  }
  return(USB_TRUE);        
}

#endif


