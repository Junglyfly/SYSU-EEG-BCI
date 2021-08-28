/******************************************************************************/
/* Name     : USB_abortAllTransaction                                         */
/*                                                                            */
/* Catagory : Software Control                                                */
/*                                                                            */
/* Purpose  : Terminate all the data transfer in progress and free up the     */
/*            the endpoints to post new data transfer requests                */
/*                                                                            */
/* Author   : MH                                                              */
/*                                                                            */
/* Based on : From scratch                                                    */
/*                                                                            */
/*                                                                            */
/*============================================================================*/
/* Arguments:                                                                 */
/*                                                                            */
/* DevNum     : USB device number, enumerated data type of USB_DevNum.        */
/*              Only USB0 is active currently                                 */
/*                                                                            */
/*============================================================================*/
/* Return Value:                                                              */
/*                                                                            */
/* USB_TRUE if successful, else USB_FALSE                                     */
/*                                                                            */
/*============================================================================*/
/* Comments:                                                                  */
/*                                                                            */
/*                                                                            */
/*============================================================================*/
/* History:                                                                   */
/*                                                                            */
/* Created:    10 Jan 2001                                                    */
/*                                                                            */
/* Modified:   02/20/01 $MH$                                                  */
/*                                                                            */
/* Renamed from  USB_killAllTransaction( ) to USB_abortAllTransaction( )      */
/*                                                                            */
/******************************************************************************/
#define _USB_MOD

#include <csl_chiphal.h>
#include <csl_usbhal.h>
#include <_csl_usb.h>
#include <csl_usb.h>

#if (_USB_SUPPORT)

extern USB_GlobalVars_struct USB_GlobalVars;

#pragma CODE_SECTION (vUSB_abortAllTransaction, ".text:vUSB_abortAllTransaction")

/*
01/10/01 $MH$
Stop all transactions in progress and reset the endpoint handles
*/

// USB_Boolean USB_abortAllTransaction(USB_DevNum DevNum)
USB_Boolean vUSB_abortAllTransaction(USB_DevNum DevNum)
{
  Uint16                ii;
  USB_EpHandle          hEpx;
  USB_Boolean           ret_stat        = USB_TRUE;
  /* USB_GlobalVars_struct *USB_GlobalVars = (USB_GlobalVars_struct *)USB_GLOBALVAR_BASE; */

  for(ii = 0; ii < 16; ii++)
  {
      hEpx = (USB_EpHandle )(((Uint32)USB_GlobalVars.EpHandleTable[ii][2]) |
                             (((Uint32)USB_GlobalVars.EpHandleTable[ii][1]) << 8) |
                             (((Uint32)USB_GlobalVars.EpHandleTable[ii][0]) << 16));

      if(hEpx != (USB_EpHandle )0)
      {
        if(!USB_abortTransaction(hEpx))
        ret_stat = USB_FALSE;
      }
    }

  return(ret_stat);
}

#endif

