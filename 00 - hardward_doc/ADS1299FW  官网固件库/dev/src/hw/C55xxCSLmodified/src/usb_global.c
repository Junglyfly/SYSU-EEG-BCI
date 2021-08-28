/******************************************************************************/
/* Name     : usb_global.c                                                    */
/*                                                                            */
/* Catagory :                                                                 */
/*                                                                            */
/* Purpose  : To relocate USB Global Vars from USB RAM to DSP RAM to          */
/*            redure USB and DMA+McBSP contension                             */
/*                                                                            */
/* Author   : $mh$                                                            */
/*            Modified: 03/19/2003 to avoid compile if USB is not supported   */
/* Based on :                                                                 */
/*                                                                            */
/*                                                                            */
/*============================================================================*/
#include <csl_chiphal.h>

#if _USB_SUPPORT
  #include <_csl_usb.h>

  #pragma DATA_SECTION (USB_GlobalVars, ".csldata:USB_data")

  USB_GlobalVars_struct USB_GlobalVars;
#endif

