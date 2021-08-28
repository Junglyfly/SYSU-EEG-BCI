/******************************************************************************/
/* Name     : _USB_addInterface                                               */
/*                                                                            */
/* Catagory : Device Initialization                                           */
/*                                                                            */
/* Purpose  : Include additional endpoint to the existing configuration       */
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
/* hEpObj[] : Pointer to a NULL terminated array of handles of initialized    */
/*            endpoint objects.  Max number of handles in the array can not   */
/*            be more than 16 (excluding the NULL handle)                     */
/*                                                                            */
/* NumOfEp  : Number of elements in the hEpObj[] array                        */
/*                                                                            */
/*============================================================================*/
/* Return Value:                                                              */
/*                                                                            */
/* USB_TRUE if additional endpoints are successfully added to the existing    */
/* device configuration, else USB_FALSE                                       */
/*                                                                            */
/*============================================================================*/
/* Comments:                                                                  */
/*                                                                            */
/* Once USB_config is called, to add another interface to the current device  */
/* call _USB_addInterface( ). This routine only initialize the endpoint       */
/* register and set endpoint dma intrpt mask bits, and updates the            */
/* USB_GlobalVars with endpoint related information.  This routine does not   */
/* reconfig EP0 regs                                                          */
/*                                                                            */
/*============================================================================*/
/* History:                                                                   */
/*                                                                            */
/* Created:    22 Dec 2000                                                    */
/*                                                                            */
/******************************************************************************/
#define _USB_MOD

#include <csl_chiphal.h>
#include <csl_usbhal.h>
#include <_csl_usb.h>
#include <csl_usb.h>


#if (_USB_SUPPORT)

extern USB_GlobalVars_struct USB_GlobalVars;

#pragma CODE_SECTION (_vUSB_addInterface, ".text:_vUSB_addInterface")

//USB_Boolean _USB_addInterface(USB_DevNum   DevNum,
//                              USB_EpHandle hEpObj[],
//                              Uint16       NumOfEp)

USB_Boolean _vUSB_addInterface(USB_DevNum   DevNum,
                              USB_EpHandle hEpObj[],
                              Uint16       NumOfEp)
{
  Uint16 ii, EpIndex, BufBaseAddrX, BufBaseAddrY,EpBuffEndAddr;

  /* USB_GlobalVars_struct *USB_GlobalVars = (USB_GlobalVars_struct *)USB_GLOBALVAR_BASE; */

  /* initialize EDB   */

  for(ii=1; ii<=NumOfEp; ii++)
  {
    EpIndex = (Uint16)(hEpObj[ii-1]->EpNum);
    if(EpIndex % 8)          /* if not EP0 config EP regs      */
    {
     BufBaseAddrX  = (Uint16)((USB_GlobalVars.NextEpBuffStartAddr[0]<<8)|
                              (USB_GlobalVars.NextEpBuffStartAddr[1]&0xFF));
     BufBaseAddrY  = BufBaseAddrX + hEpObj[ii-1]->MaxPktSiz;
     EpBuffEndAddr = BufBaseAddrY + hEpObj[ii-1]->MaxPktSiz;
     if(EpBuffEndAddr <= USB_SRAM_END)         /* if not endpoint buf addr overflow */
       {
          _USB_configEndpt(hEpObj[ii-1], USBEPCNF_INIT, BufBaseAddrX, BufBaseAddrY);
          // start address of next endpoint buffer
          // USB_GlobalVars->NextEpBuffStartAddr = EpBuffEndAddr;
          USB_GlobalVars.NextEpBuffStartAddr[0] = (EpBuffEndAddr>>8) & 0xFF;
          USB_GlobalVars.NextEpBuffStartAddr[1] = EpBuffEndAddr & 0xFF;
       }
     else
          return(USB_FALSE);                   /* if endpoint buf addr overflow */
    }  /* end of if */
    else
    {
      return(USB_FALSE);                   /* if endpoint buf addr overflow */
    }  /* end of if-else */
  }    /* end of for(ii=1; ii<=NumOfEp; ii++) */

  return(USB_TRUE);
}

#endif
