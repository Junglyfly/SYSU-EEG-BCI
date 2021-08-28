/******************************************************************************/
/* Name     : USB_peekEvents                                                  */
/*                                                                            */
/* Catagory : Status / Query                                                  */
/*                                                                            */
/* Purpose  : Read all the pending USB_EVENTS associated with a particular    */
/*            USB endpoint                                                    */
/*                                                                            */
/* Author   : MH (partially)                                                  */
/*                                                                            */
/* Based on : USB code by Jason Kridner(IA) and Venkat Raghavan (TI India)    */
/*                                                                            */
/*                                                                            */
/*============================================================================*/
/* Arguments:                                                                 */
/*                                                                            */
/* hEp      : Handle to an initialized endpoint object                        */
/*                                                                            */
/*                                                                            */
/*                                                                            */
/*                                                                            */
/*                                                                            */
/*                                                                            */
/*                                                                            */
/*============================================================================*/
/* Return Value:                                                              */
/*                                                                            */
/* ORed combination of all the pending USB_EVENTS associated with the         */
/* particular endpoint                                                        */
/*                                                                            */
/*============================================================================*/
/* Comments:                                                                  */
/*                                                                            */
/* Calling this routine does not clear the pending USB_ENENTS associated      */
/* the particular endpoint                                                    */
/*                                                                            */
/*============================================================================*/
/* History:                                                                   */
/*                                                                            */
/* Created:       28 Nov 2000                                                 */
/*                                                                            */
/* Last Modified: 21 Fev 2001                                                 */
/*                                                                            */
/******************************************************************************/
#define _USB_MOD

#include <csl_chiphal.h>
#include <csl_usbhal.h>
#include <_csl_usb.h>
#include <csl_usb.h>

#if (_USB_SUPPORT)

#pragma CODE_SECTION (vUSB_peekEvents, ".text:vUSB_peekEvents")

// USB_EVENT_MASK USB_peekEvents(USB_EpHandle hEp)
USB_EVENT_MASK vUSB_peekEvents(USB_EpHandle hEp)
{
  return(hEp->EventFlag);
}

#endif

