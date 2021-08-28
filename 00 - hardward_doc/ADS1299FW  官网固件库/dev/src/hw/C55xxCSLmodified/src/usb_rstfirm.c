/******************************************************************************/
/* Name     : _USB_resetFirmwareEnv                                           */
/*                                                                            */
/* Catagory : Firmware Initialization (private function)                     */
/*                                                                            */
/* Purpose  : Reset the internal USB firmware variable to a known state       */
/*                                                                            */
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
/*============================================================================*/
/* Return Value:                                                              */
/*                                                                            */
/* USB_TRUE if the internal firmware variables are reset to a known state,    */
/* else USB_FALSE                                                             */
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

extern USB_GlobalVars_struct USB_GlobalVars;

#pragma CODE_SECTION (_vUSB_resetFirmwareEnv, ".text:_vUSB_resetFirmwareEnv")

// USB_Boolean _USB_resetFirmwareEnv(USB_DevNum DevNum)
USB_Boolean _vUSB_resetFirmwareEnv(USB_DevNum DevNum)
{
  Uint16 ii;
  USB_Boolean ret_stat = USB_TRUE;
  /* USB_GlobalVars_struct *USB_GlobalVars = (USB_GlobalVars_struct *)USB_GLOBALVAR_BASE; */

  /*
  init endpoint handle table, endpoint event table, and endpoint reload count table
  */
  for(ii = 0; ii < 16; ii++)
  {
    /*
    init endpoint handle table
    */
    USB_GlobalVars.EpHandleTable[ii][2] = 0x00;
    USB_GlobalVars.EpHandleTable[ii][1] = 0x00;
    USB_GlobalVars.EpHandleTable[ii][0] = 0x00;

    /*
    init endpoint reload count table

    USB_GlobalVars->ReloadCount[ii] = 0x00;
    */
  }


  /*
  Mark the buffer (USB SRAM) start addr for next endpoint to be initialized.
  The variable is used to check USB SRAM overflow in USB_config and _USB_configInterface
  routine

  */
  USB_GlobalVars.NextEpBuffStartAddr[0] = ((USB_SRAM_START - USB_DMA_REG_START)>>8)&0xFF;
  USB_GlobalVars.NextEpBuffStartAddr[1] =  (USB_SRAM_START - USB_DMA_REG_START)&0xFF;


  // init USB remore wakeup status
  USB_GlobalVars.RemoteWkpStat = USB_FALSE;

  // clear USB SOF interrupt option flag
  USB_GlobalVars.FlagSOF = USB_FALSE;

  // reset number of active endpoints
  USB_GlobalVars.ActiveEndptNos = 0x00;

  return(ret_stat);
}
#endif


