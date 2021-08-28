/******************************************************************************\
*           Copyright (C) 1999 Texas Instruments Incorporated.
*                           All Rights Reserved
*------------------------------------------------------------------------------
* MODULE.NAME... RTC
* FILENAME...... rtc_bcdtodec.c
* DATE CREATED.. 1/10/01
* PROJECT....... RTC
* COMPONENT..... 
* IMPORTS....... 
*------------------------------------------------------------------------------
* HISTORY:
*   1/10/01 (AF) Created
*        Modified:      9/17/2003  Modified RTC_decToBcd() and RTC_bcdToDec()
*                                      to avoid rts55 library calls.
*
*------------------------------------------------------------------------------
* DESCRIPTION:  (body file for the RTC module)
*
*
*
\******************************************************************************/
#define _RTC_MOD_

/****************************************\
* include files
\****************************************/
#include <csl_chiphal.h>

#if (_RTC_SUPPORT)

#include <csl_rtc.h>

/******************************************************************************\
*                         L O C A L   S E C T I O N
\******************************************************************************/

/****************************************\
* RTC static macro declarations
\****************************************/

/****************************************\
* RTC static typedef declarations
\****************************************/

/****************************************\
* RTC static function declarations
\****************************************/

/****************************************\
* RTC static variable definitions
\****************************************/

/****************************************\
* RTC static function definitions
\****************************************/

/******************************************************************************\
*                        G L O B A L   S E C T I O N
\******************************************************************************/

/****************************************\
* RTC global variable definitions
\****************************************/

/****************************************\
* RTC global function definitions
\****************************************/

/*-------------------------------------------------------*\
 int RTC_decToBcd(int value):
 Changes decimal value to BCD value which is what RTC needs
 --------------------------------------------------*/



/*
RTC_decToBcd() => 16 * (dec_value / 10) + (dec_value % 10);
               => 6 * (dec_value / 10) + 10 * (dec_value / 10) + (dec_value % 10);
               => 6 * (dec_value / 10) + dec_value;
               Now (dec_value / 10) ~= (dec_value * A + B) >> C;
               For division by 10;
                  "(dec_value * 103) >> 10"  seems most optimal.
*/

#pragma CODE_SECTION(RTC_decToBcd,".text:RTC_decToBcd")

int RTC_decToBcd(int dec_value)
{
    /* note: designed to work for 100 > dec_value >= 0; */
    return (dec_value + 6 * (dec_value * 103 >> 10));
}

/* RTC_bcdToDec() =>  (10 * hex_value/16) + (hex_value % 16)
                  =>  hex_value - (6* hex_value/16)
*/

#pragma CODE_SECTION(RTC_bcdToDec,".text:RTC_bcdToDec")

int RTC_bcdToDec(int hex_value)
{
     /* note: designed to work for 0x100 > hex_value >= 0x00; */
   return (hex_value - 6 * (hex_value >> 4));
}

/*----------------------------------------------------------------------------*/

#endif /* RTC_SUPPORT */
/******************************************************************************\
* End of rtc_bcdtodec.c
\******************************************************************************/

