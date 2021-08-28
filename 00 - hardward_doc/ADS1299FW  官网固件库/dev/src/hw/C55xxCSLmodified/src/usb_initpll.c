/******************************************************************************/
/* Name     : USB_initPLL                                                     */
/*                                                                            */
/* Catagory : Device Control                                                  */
/*                                                                            */
/* Purpose  : Initailize USB PLL to generate clock for the USB module         */
/*                                                                            */
/* Author   : MH                                                              */
/*                                                                            */
/* Based on : C55x PLL init routine                                           */
/*                                                                            */
/*============================================================================*/
/* Arguments:                                                                 */
/*                                                                            */
/* inclk      : Input clock (supplied at CLKIN pin) frequency ( in MHz)       */
/*                                                                            */
/* outclk     : Desired clock frequency (in MHz) for the USB moduel, the      */
/*              outclk must be 48 MHz for the proper operation of the USB     */
/*              module.                                                       */
/*                                                                            */
/* plldiv     : Input clock (supplied at CLKIN pin) devide down value, used   */
/*              for USB PLL enable as well as USB PLL bypass mode             */
/*                                                                            */
/*============================================================================*/
/* Return Value:   None                                                       */
/*                                                                            */
/*                                                                            */
/*============================================================================*/
/* Comments:                                                                  */
/*                                                                            */
/* pllmult = (outclk * (plldiv+1)) / inclk                                    */
/*                                                                            */
/* if pllmult > 1                                                             */
/*		outclk = (pllmult / (plldiv + 1)) * inclk                         */
/*                                                                            */
/* if pllmult < 1                                                             */
/*		outclk = (1 / (plldiv + 1)) * inclk                               */
/*                                                                            */
/*============================================================================*/
/* History:                                                                   */
/*                                                                            */
/* Created:    16 Jan 2001                                                    */
/*                                                                            */
/******************************************************************************/
#define _USB_MOD

#include <csl_chiphal.h>
#include <csl_usbhal.h>
#include <_csl_usb.h>
#include <csl_usb.h>

#if (_USB_SUPPORT)

#pragma CODE_SECTION (vUSB_initPLL, ".text:vUsb_initPLL")

// void USB_initPLL(Uint16 inclk, Uint16 outclk, Uint16 plldiv)
void vUSB_initPLL(Uint16 inclk, Uint16 outclk, Uint16 plldiv)
{
  Uint16 pllmult = (outclk * (plldiv+1)) / inclk;

  plldiv &= 0x3u;

  USB_CLKMD &= ~(1<<USB_PLL_ENABLE);        // force into BYPASS mode (b4=0)

  while (USB_CLKMD & (1<<USB_PLL_LOCK));    // wait for BYPASS mode to be active

  USB_CLKMD = ((1<<USB_PLL_IOB)|(pllmult<<USB_PLL_MULT)|(plldiv<<USB_PLL_DIV)|
	          (plldiv<<USB_PLL_BYPASS_DIV)|(1<<USB_PLL_ENABLE));

	if (pllmult > 1) // if pllmult > 1, wait for PLL mode to be active 
	{
		while (!(USB_CLKMD & (1<<USB_PLL_LOCK)));
	}
}
#endif

