/******************************************************************************/
/* Name     : _USB_configEndpt                                                */
/*                                                                            */
/* Catagory : Initialization (private function)                              */
/*                                                                            */
/* Purpose  : Configure an endpoint descriptor block and DMA registers        */
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
/* hEp          : Handle to an endpoint object to be initialized              */
/*                                                                            */
/* EPxConfRegVal: 8-bit value to be written to Endpoint Config regsiter       */
/*                                                                            */
/* BufBaseAddrX : Xbuffer base address in the USB buffer RAM                  */
/*                                                                            */
/* BufBaseAddrY : Xbuffer base address in the USB buffer RAM                  */
/*                                                                            */
/*============================================================================*/
/* Return Value:                                                              */
/*                                                                            */
/* USB_TRUE if the initialization is successful, else USB_FLASE               */
/*                                                                            */
/*============================================================================*/
/* Comments:                                                                  */
/*                                                                            */
/* The event handler should be in void Fxn(void) form. Using the INTERRUPT    */
/* is not necessary.  Once the program control branches to Fxn the user's     */
/* code is free to call other functions or event post DSP?BIOS SWI            */
/*                                                                            */
/*============================================================================*/
/* History:                                                                   */
/*                                                                            */
/* Created:       24 Nov 2000                                                 */
/*                                                                            */
/******************************************************************************/


/* --------------------------11/24/00 $MH$: working --------------------------
 Configure the USB interface. Initialize all the endpoints. For endpoint 0,
 it would be done by configuring the CONF and BYTE COUNT registers. For all
 other endpoints, the Endpoint Descriptor Blocks and DMA Contexts need to be
 initialized.

 02/26/01 $MH$ added host endpt config code
 
 08/26/02 $MH$ fixed ISO endpt size reg configuration (3 msb of 10 bit value)
 08/27/02 $MH$ enable ISO endpt DMA interrupt - bug fix 
 ----------------------------------------------------------------------------*/
#define _USB_MOD

#include <csl_chiphal.h>
#include <csl_usbhal.h>
#include <_csl_usb.h>
#include <csl_usb.h>

#if (_USB_SUPPORT)

extern USB_GlobalVars_struct USB_GlobalVars;

#pragma CODE_SECTION (_vUSB_configEndpt, ".text:_vUSB_configEndpt")

// USB_Boolean _USB_configEndpt(USB_EpHandle hEp,
//                            Uint16 EPxConfRegVal,
//                            Uint16 BufBaseAddrX,
//                            Uint16 BufBaseAddrY)

USB_Boolean _vUSB_configEndpt(USB_EpHandle hEp,
                            Uint16 EPxConfRegVal,
                            Uint16 BufBaseAddrX,
                            Uint16 BufBaseAddrY)
{

  USB_Boolean           ret_stat        = USB_FALSE;
  Uint16                ii              = (Uint16)hEp->EpNum;
  USB_EDRegBlk          *USB_EDB        = (USB_EDRegBlk *)hEp->EDReg_SAddr;
  /* USB_GlobalVars_struct *USB_GlobalVars = (USB_GlobalVars_struct *)USB_GLOBALVAR_BASE; */

//#ifdef usb_phase3
  USB_IsoInRegBlk  *USB_EDBISOIN  = (USB_IsoInRegBlk *)hEp->EDReg_SAddr;
  USB_IsoOutRegBlk *USB_EDBISOOUT = (USB_IsoOutRegBlk *)hEp->EDReg_SAddr;
//#endif

  USB_EP0_RegBlk   *USB_EP0Reg    = (USB_EP0_RegBlk *)hEp->EDReg_SAddr;


  if(ii % 8)   /* Initialize Endpoint Descriptor Blocks */
  {
    switch(hEp->XferType)
    {
      case USB_BULK :
      case USB_INTR :
      case USB_HPORT:

           USB_EDB->CNF  = (EPxConfRegVal & 0xBF);  /* make sure ISO-bit = 0 */
           USB_EDB->BAX = (BufBaseAddrX>>4);
    /* -------------------------------------------------
     NAK is set for IN endpoints, but OUT endpoints are
     made ready for data by clearing the NAK bit.
     --------------------------------------------------*/
           USB_EDB->CTX  = (ii / 8) ? USBEPCT_NAK : 0;
     /*      USB_EDB->X_EPBCNT  = USB_EPBCNT_NAK;              */
           USB_EDB->BAY    = (BufBaseAddrY>>4);
           USB_EDB->CTY  = (ii / 8) ? USBEPCT_NAK : 0;
     /*      USB_EDB->Y_EPBCNT  = USB_EPBCNT_NAK;              */
           USB_EDB->SIZ   = (hEp->MaxPktSiz & 0x7F);

           if(hEp->XferType != USB_HPORT)         // if bulk or intr endpt
           {
             //_USB_enableDmaIntrpt(USB0, hEp);    /* enable USB DMA interrupt */
             _USB_EN_DMA_INTR(USB0, hEp->EpNum);
           }
//#ifdef usb_phase3
           else
           {
             if(ii / 8)                            // if in endpt
                USBHEPSEL = ((USBHEPSEL & 0x8F) | (( hEp->EpNum & 0x7) << 4));
             else                                  // if out endpt
                USBHEPSEL = ((USBHEPSEL & 0xF8) | ( hEp->EpNum & 0x7));

             // enable the host intr - intr flag are cleared when USB_REG_VECINT is
             // read in USB_init( ) routine
             if(hEp->EventMask & USB_EVENT_HINT)
                USBHCTL |= USBHCTL_HIE;     // enable host intr

             if(hEp->EventMask & USB_EVENT_HERR)
                USBHCTL |= USBHCTL_HERRIE;    // enable host err intr

             // enable host mode if both host endpts are selected
             if(((USBHEPSEL & 0x70)>>4) && (USBHEPSEL & 0x07))
                USBHCTL |= USBHCTL_EN;
           }
//#endif  // usb_phase3
             ret_stat = USB_TRUE;
           break;

   /* --------------------$MH$---------------------------
     Need code for phase III iso support
     make sure ISO-bit = 1
     ----------------------------------------------------*/
      case USB_ISO:

//#ifdef usb_phase3
           if(ii / 8)                            // ISO IN
           {
             USB_EDBISOIN->CNF = ((EPxConfRegVal & 0xC0)|0x40);   // set ISO-bit
           // $MH$ 08/26/02 USB ISO endpt size bug fix  
           //USB_EDBISOIN->SIZH = ((hEp->MaxPktSiz & 0x0380)>>6); // load upper 3 bits of max pkt siz
             USB_EDBISOIN->SIZH = ((hEp->MaxPktSiz & 0x0380)>>7); // load upper 3 bits of max pkt siz
           }
           else                                  // ISO OUT
           {
             USB_EDBISOOUT->CNF = (((EPxConfRegVal & 0xC0)|0x40)    // set ISO-bit
           // $MH$ 08/26/02 USB ISO endpt size bug fix
           //                     |((hEp->MaxPktSiz & 0x0380)>>6)); // upper 3 bits of max pkt siz
                                  |((hEp->MaxPktSiz & 0x0380)>>7)); // upper 3 bits of max pkt siz

             USB_EDBISOOUT->CTXH &= 0xF8;         // clr high 3 bits of X_BCNT reg
             USB_EDBISOOUT->CTYH &= 0xF8;         // clr upper 3 bits of Y_BCNT reg
           }

           // following registers are same for iso and non-iso mode
           USB_EDB->BAX    = (BufBaseAddrX>>4);
           USB_EDB->BAY    = (BufBaseAddrY>>4);
           USB_EDB->SIZ   = (hEp->MaxPktSiz & 0x7F);
           // set NAK for IN Endpts but clear NAK for OUT Endpts
           // so they are ready to receive data
           USB_EDB->CTX  = (ii / 8) ? USBEPCT_NAK : 0;
           USB_EDB->CTY  = (ii / 8) ? USBEPCT_NAK : 0;
           
           // $MH$ 08/27/02  enable USB DMA interrupt - bug fix
             _USB_EN_DMA_INTR(USB0, hEp->EpNum);
             
//#endif // usb_phase3

           break;

      default:
           break;
    }                                        /* end of switch(EPxType)  */
  }                                          /* end of if(ii % 8)       */
  else                                       /* config control endpoint */
  {
    USB_EP0Reg->CNF  = EPxConfRegVal;
    USB_EP0Reg->CT = USBEPCT_NAK;
    //_USB_enableEpIntrpt(USB0, hEp);            /* enable endpoint intrpt */
    _USB_EN_EP_INTR(USB0, hEp->EpNum);
    ret_stat = USB_TRUE;
  }                                           /* end of else            */

  /*
  register endpoint handle in the endpoint handle table
  */
  USB_GlobalVars.EpHandleTable[ii][2] = (Uint16)((Uint32)hEp & 0xFF);
  USB_GlobalVars.EpHandleTable[ii][1] = (Uint16)(((Uint32)hEp >> 8) & 0xFF);
  USB_GlobalVars.EpHandleTable[ii][0] = (Uint16)(((Uint32)hEp >> 16) & 0xFF);

  // if SOF interrupt selected then set flag
  if(hEp->EventMask & USB_EVENT_SOF)
       USB_GlobalVars.FlagSOF = USB_TRUE;

  return(ret_stat);
}                                  /* end of _USB_configEndpt(..)  */

#endif

