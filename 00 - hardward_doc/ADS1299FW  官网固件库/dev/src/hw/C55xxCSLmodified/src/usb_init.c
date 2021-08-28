/******************************************************************************/
/* Name     : USB_init                                                        */
/*                                                                            */
/* Catagory : Device Initialization                                           */
/*                                                                            */
/* Purpose  : Configure the USB module                                        */
/*                                                                            */
/*                                                                            */
/* Author   : MH                                                              */
/*                                                                            */
/* Based on : None                                                            */
/*                                                                            */
/*                                                                            */
/*============================================================================*/
/* Arguments:                                                                 */
/*                                                                            */
/* DevNum     : USB device number, enumerated data type of USB_DevNum.        */
/*              Only USB0 is active currently                                 */
/*                                                                            */
/* hEpObj[]   : Pointer to a NULL terminated array of handles of initialized  */
/*              endpoint objects.  Max number of handles in the array can not */
/*              be more than 16 (excluding the NULL handle)                   */
/*                                                                            */
/* PSofTmrCnt : 8-bit counter value for the pre SOF timer                     */
/*                                                                            */
/*============================================================================*/
/* Return Value:                                                              */
/*                                                                            */
/* USB_TRUE if the device configuration is successful, else USB_FALSE         */
/*                                                                            */
/*============================================================================*/
/* Comments:                                                                  */
/*                                                                            */
/* Upon sucessful return from the function call the USB module is completely  */
/* for operation (all the regs are configured and unmasked interrupts         */
/* are enabled).  Once the USB module is configured the user's code needs to  */
/* unmask the USB interrupt mask bit in the IER0 reg of DSP and enable the    */
/* DSP global interrupt.  Finally the user's code should call the macro       */
/* USB_CONNECT(USB0) to connect the usb module to the USB bus                 */
/*                                                                            */
/*============================================================================*/
/* History:                                                                   */
/*                                                                            */
/* Created:    24 Nov 2000                                                    */
/*                                                                            */
/* 28 Feb 01 $MH$                                                             */
/* Changed name from USB_config( ) to USB_setParams( ) to comply with CSL     */
/* standard                                                                   */
/*                                                                            */
/* 18 Jul 01 $MH$                                                             */
/* Changed name from USB_setParams( ) to USB_init( ) to comply with CSL       */
/* standard                                                                   */
/*                                                                            */
/* 27 Aug 02 $MH$                                                             */
/* added capability to handle non multiple of 16-byte ISO max endpt size      */
/*                                                                            */
/******************************************************************************/


 /* --------------------------11/27/00 $MH$-----------------------------------
 Configure the USB interface. Initialize all the endpoints in the linked list.
 For endpoint 0, it would be done by configuring the CONF and BYTE COUNT
 registers.  For all other endpoints, initialize the Endpoint Descriptor Blocks
 and DMA Contexts.  - works
 Once this routine is called, to add another interface to the current device
 call _USB_configInterface( )
 ----------------------------------------------------------------------------*/
#define _USB_MOD

#include <csl_chiphal.h>
#include <csl_usbhal.h>
#include <_csl_usb.h>
#include <csl_usb.h>

#if (_USB_SUPPORT)

extern USB_GlobalVars_struct USB_GlobalVars;

#pragma CODE_SECTION (vUSB_init, ".text:vUSB_init")

//USB_Boolean USB_init(USB_DevNum   DevNum,
//                     USB_EpHandle hEp[],
//                     Uchar        PSofTmrCnt)

USB_Boolean vUSB_init(USB_DevNum   DevNum,
                      USB_EpHandle hEp[],
                      Uchar        PSofTmrCnt)
{
  USB_Boolean           ret_stat        = USB_FALSE;
  USB_DMA_Context       *USB_DMA        = (USB_DMA_Context *)((Uint32)USB_DMA_REG_START);
  /* USB_GlobalVars_struct *USB_GlobalVars = (USB_GlobalVars_struct *)USB_GLOBALVAR_BASE; */

  Uint16 ii, EpIndex, BufBaseAddrX, BufBaseAddrY, EpBuffEndAddr;

  /*
  12/21/00 $MH$:   reset USB firmware internal variables
  */
  _USB_resetFirmwareEnv(DevNum);

  /* -----------------10/26/2000 11:01PM---------------
  Take the USB module out of reset.
   --------------------------------------------------*/
  USBIDLCTL |= USBIDLCTL_USBEN;
  USBCTL       &= 0x90;                /* reset USBCTL reg */


  /*
  12/24/00 $MH$:   Enable USB PLL and wait until PLL locked
  01/16/01 $NH$:   move USB PLL init code to USB_initPLL( )
  */

  /*
  Reset the device address to 0
  */
  USBADDR = 0x00;

    /*
  Clear all the USB intrpt mask reg bits
  */
  USBIDIE   = 0x00;
  USBODIE   = 0x00;
  USBIEPIE  = 0x00;
  USBOEPIE  = 0x00;

  /*
  Clear USB interrupt status reg - specially for clearing SETUP and STPOW bit
  */
  USBIF = 0xFF;

  /* Clear all pending USB interrupts
   - use DevNum in the future to select the vect reg
   */
  while(USBINTSRC);

  /* Initialize DMA context - in future use DevNum to calculate the DMA context block*/
  for(ii=1; ii<MAX_ENDPT; ii++)
  {
    USB_DMA[ii].DCTL      = 0;
    USB_DMA[ii].DSIZ      = 0;
    USB_DMA[ii].DADL      = 0;
    USB_DMA[ii].DADH      = 0;
    USB_DMA[ii].DCT       = 0;
    USB_DMA[ii].DRSZ      = 0;
    USB_DMA[ii].DRAL      = 0;
    USB_DMA[ii].DRAH      = 0;
  }

  USBPSOFTMR = (PSofTmrCnt & 0xFF);      /* update PreSOF timer Count */

  /* initialize EDB   */

  for(ii=0; ((ii < MAX_ENDPT) && (hEp[ii] != NULL)); ii++)
  {
    ret_stat = USB_TRUE;
    EpIndex = (Uint16)(hEp[ii]->EpNum);
    if(!(EpIndex % 8))          /* if endpoint0                          */
    {
      _USB_configEndpt(hEp[ii], USBCNF0_INIT, 0x00, 0x00);
      USB_GlobalVars.ActiveEndptNos += 1;
    }
    else                        /* Initialize Endpoint Descriptor Blocks */
    {
     BufBaseAddrX  = (Uint16)((USB_GlobalVars.NextEpBuffStartAddr[0]<<8)|
                              (USB_GlobalVars.NextEpBuffStartAddr[1]&0xFF));
                              
     BufBaseAddrY  = BufBaseAddrX + hEp[ii]->MaxPktSiz;
     
     // 08/27/02 $MH$ fix for non multiple of 16-byte ISO max endpt size
     if((hEp[ii]->XferType == USB_ISO)&&(hEp[ii]->MaxPktSiz & 0x0F))
        BufBaseAddrY  = (BufBaseAddrY + 0x10)&0xFFF0;
     
     EpBuffEndAddr = BufBaseAddrY + hEp[ii]->MaxPktSiz;
     
     // 08/27/02 $MH$ fix for non multiple of 16-byte ISO max endpt size
     if((hEp[ii]->XferType == USB_ISO)&&(hEp[ii]->MaxPktSiz & 0x0F))
        EpBuffEndAddr  = (EpBuffEndAddr + 0x10)&0xFFF0;
        
     if(EpBuffEndAddr <= USB_SRAM_END)         /* if not endpoint buf addr overflow */
       {
          _USB_configEndpt(hEp[ii], USBEPCNF_INIT, BufBaseAddrX, BufBaseAddrY);
          // start address of next endpoint buffer
          // USB_GlobalVars->NextEpBuffStartAddr = EpBuffEndAddr;
          USB_GlobalVars.NextEpBuffStartAddr[0] = (EpBuffEndAddr>>8) & 0xFF;
          USB_GlobalVars.NextEpBuffStartAddr[1] = EpBuffEndAddr & 0xFF;
          // increment active endpt count
          USB_GlobalVars.ActiveEndptNos += 1;
       }
     else
       {
         // reset number of active endpoints
         USB_GlobalVars.ActiveEndptNos = 0x00;
         ret_stat = USB_FALSE;
         break;
       }  /* if endpoint buf addr overflow */
    }     /* end of if-else */

  }       /* end of for(ii=1; ii<=NumOfEp; ii++) */


  /*
  Enable USB bus event interrupts - use DevNum in the future.
  USB intrpt mask reg bits are set in the USB_postTransaction( )
  and the mask bits are reset USB_isr at the end of each transaction.
  */
  if(ret_stat == USB_TRUE)          /* if the configuration went well so far */
  {
    USBIE = USBIE_INIT;             /* enable USB interrupts only  */

    if(USB_GlobalVars.FlagSOF)
       USBIE |= USB_INT_SOF;

    if((PSofTmrCnt & 0xFF) != 0x00)  /* preSOF timer count > 0                */
                                     /* enable USB interrupts along with PSOF */
       USBIE |= USB_INT_PSOF;

    USBCTL |= USBCTL_INIT;           /* enable USB function         */
  }

  return(ret_stat);
}
#endif

