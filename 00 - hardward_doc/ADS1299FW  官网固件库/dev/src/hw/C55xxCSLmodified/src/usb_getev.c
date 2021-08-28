/******************************************************************************/
/* Name     : USB_getEvents                                                   */
/*                                                                            */
/* Catagory : Status / Query                                                  */
/*                                                                            */
/* Purpose  : Get all the pending USB_EVENTS associated with a particular     */
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
/* Calling this routine also clears all the pending USB_ENENTS associated     */
/* with the particular endpoint                                               */
/*                                                                            */
/*============================================================================*/
/* History:                                                                   */
/*                                                                            */
/* Created:    28 Nov 2000                                                    */
/* Modified    01/10/01 $MH$                                                  */
/*                                                                            */
/******************************************************************************/
#define _USB_MOD

#include <csl_chiphal.h>
#include <csl_usbhal.h>
#include <_csl_usb.h>
#include <csl_usb.h>


#if (_USB_SUPPORT)

#pragma CODE_SECTION (vUSB_getEvents, ".text:vUsb_getEvents")

// USB_EVENT_MASK USB_getEvents(USB_EpHandle hEp)
USB_EVENT_MASK vUSB_getEvents(USB_EpHandle hEp)
{
  Uint16 Events;
  Events         = hEp->EventFlag;
  hEp->EventFlag = 0;
  return(Events);
}
#endif
