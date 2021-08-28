/****************************************************************/
/* ADS1299 ADCPro Firmware Version 1.0 for the TMS320C5500      */
/* Copyright (C) 2009 - 2010 Texas Instruments Incorporated     */
/* All Rights Reserved                                          */
/****************************************************************/
/*
Permission is hereby granted, free of charge, to any person
obtaining a copy of this software and associated documentation
files (the "Software"), to deal in the Software without
restriction, including without limitation the rights to use, copy,
modify, merge, publish, distribute, sublicense, and/or sell copies
of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT.  IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
DEALINGS IN THE SOFTWARE.
*/
/****************************************************************/
#ifndef __TADS1299_FN_H
#define __TADS1299_FN_H

/* this define is needed for the API 2.x                        */
#define __CSL_AVAILABLE

/* include the data converter api interface file                */
#include "tidc_api.h"

/* include the interface files for the chip support library     */
#include <csl_dma.h>
//#include <csl_mcbsp.h>

/****************************************************************/
/* the following register definitions mirror the version given  */
/* the datasheet dated 11/16/09                                 */
/****************************************************************/
/** Structure definition for configuration register 0 (ID)      */
/****************************************************************/
typedef union
{
   unsigned int value;
   struct
   {
       unsigned int reserved           :8;
       unsigned int rev7               :4;
       unsigned int devid              :4;
   } control_bit;
} TADS1299ID;


/****************************************************************/
/** Structure definition for configuration register 1 (CONFIG1) */
/****************************************************************/
typedef union
{
   unsigned int value;
   struct
   {
       unsigned int reserved           :8;
       unsigned int res7               :1;
       unsigned int daisyen            :1;
       unsigned int clken              :1;
       unsigned int rsv4               :2;
       unsigned int dr                 :3;
   } control_bit;
} TADS1299CONFIG1;


/****************************************************************/
/** Structure definition for configuration register 2 (CONFIG2) */
/****************************************************************/
typedef union
{
   unsigned int value;
   struct
   {
       unsigned int reserved           :8;
       unsigned int rsv7               :3;
       unsigned int inttest            :1;
       unsigned int rsv3               :1;
       unsigned int testamp            :1;
       unsigned int testfreq           :2;
   } control_bit;
} TADS1299CONFIG2;


/****************************************************************/
/** Structure definition for configuration register 3 (CONFIG3) */
/****************************************************************/
typedef union
{
   unsigned int value;
   struct
   {
       unsigned int reserved           :8;
       unsigned int pdbrefbuf          :1;
       unsigned int rsv6               :2;
       unsigned int biasmeas           :1;
       unsigned int biasrefint         :1;
       unsigned int pdbbias            :1;
       unsigned int biasloff           :1;
       unsigned int biasstat           :1;
   } control_bit;
} TADS1299CONFIG3;


/****************************************************************/
/** Structure definition for lead-off control register (LOFF)   */
/****************************************************************/
typedef union
{
   unsigned int value;
   struct
   {
       unsigned int reserved           :8;
       unsigned int compth             :3;
       unsigned int rsv4               :1;
       unsigned int ileadoff           :2;
       unsigned int fleadoff           :2;
   } control_bit;
} TADS1299LOFF;


/****************************************************************/
/** Structure definition for channel settings register (CHSET)  */
/****************************************************************/
typedef union
{
   unsigned int value;
   struct
   {
       unsigned int reserved           :8;
       unsigned int pd                 :1;
       unsigned int gain               :3;
       unsigned int srb2               :1;
       unsigned int mux                :3;
   } control_bit;
} TADS1299CHASET;


/****************************************************************/
/** Structure definition for RLD_SENSP register (RLDSENSP)      */
/****************************************************************/
typedef union
{
   unsigned int value;
   struct
   {
       unsigned int reserved           :8;
       unsigned int BIAS8p             :1;
       unsigned int BIAS7p             :1;
       unsigned int BIAS6p             :1;
       unsigned int BIAS5p             :1;
       unsigned int BIAS4p             :1;
       unsigned int BIAS3p             :1;
       unsigned int BIAS2p             :1;
       unsigned int BIAS1p             :1;
   } control_bit;
} TADS1299BIASSENSP;


/****************************************************************/
/** Structure definition for RLD_SENSN register (RLDSENSN)      */
/****************************************************************/
typedef union
{
   unsigned int value;
   struct
   {
       unsigned int reserved           :8;
       unsigned int BIAS8n             :1;
       unsigned int BIAS7n             :1;
       unsigned int BIAS6n             :1;
       unsigned int BIAS5n             :1;
       unsigned int BIAS4n             :1;
       unsigned int BIAS3n             :1;
       unsigned int BIAS2n             :1;
       unsigned int BIAS1n             :1;

   } control_bit;
} TADS1299BIASSENSN;


/****************************************************************/
/** Structure definition for LOFF_SENSP register (LOFFSENSP)    */
/****************************************************************/
typedef union
{
   unsigned int value;
   struct
   {
       unsigned int reserved           :8;
       unsigned int loff8p             :1;
       unsigned int loff7p             :1;
       unsigned int loff6p             :1;
       unsigned int loff5p             :1;
       unsigned int loff4p             :1;
       unsigned int loff3p             :1;
       unsigned int loff2p             :1;
       unsigned int loff1p             :1;
   } control_bit;
} TADS1299LOFFSENSP;


/****************************************************************/
/** Structure definition for LOFF_SENSN register (LOFFSENSN)    */
/****************************************************************/
typedef union
{
   unsigned int value;
   struct
   {
       unsigned int reserved           :8;
       unsigned int loff8n             :1;
       unsigned int loff7n             :1;
       unsigned int loff6n             :1;
       unsigned int loff5n             :1;
       unsigned int loff4n             :1;
       unsigned int loff3n             :1;
       unsigned int loff2n             :1;
       unsigned int loff1n             :1;
   } control_bit;
} TADS1299LOFFSENSN;


/****************************************************************/
/** Structure definition for LOFF_FLIP register (LOFFFLIP)      */
/****************************************************************/
typedef union
{
   unsigned int value;
   struct
   {
       unsigned int reserved           :8;
       unsigned int loffflip8          :1;
       unsigned int loffflip7          :1;
       unsigned int loffflip6          :1;
       unsigned int loffflip5          :1;
       unsigned int loffflip4          :1;
       unsigned int loffflip3          :1;
       unsigned int loffflip2          :1;
       unsigned int loffflip1          :1;
   } control_bit;
} TADS1299LOFFFLIP;


/****************************************************************/
/** Structure definition for lead off status P register
    (LOFFSTATP)                                                 */
/****************************************************************/
typedef union
{
   unsigned int value;
   struct
   {
       unsigned int reserved           :8;
       unsigned int in8poff            :1;
       unsigned int in7poff            :1;
       unsigned int in6poff            :1;
       unsigned int in5poff            :1;
       unsigned int in4poff            :1;
       unsigned int in3poff            :1;
       unsigned int in2poff            :1;
       unsigned int in1poff            :1;
   } control_bit;
} TADS1299LOFFSTATP;


/****************************************************************/
/** Structure definition for lead off status N register
    (LOFFSTATN)                                                 */
/****************************************************************/
typedef union
{
   unsigned int value;
   struct
   {
       unsigned int reserved           :8;
       unsigned int in8noff            :1;
       unsigned int in7noff            :1;
       unsigned int in6noff            :1;
       unsigned int in5noff            :1;
       unsigned int in4noff            :1;
       unsigned int in3noff            :1;
       unsigned int in2noff            :1;
       unsigned int in1noff            :1;
   } control_bit;
} TADS1299LOFFSTATN;


/****************************************************************/
/** Structure definition for GPIO register (GPIO)               */
/****************************************************************/
typedef union
{
   unsigned int value;
   struct
   {
       unsigned int reserved           :8;
       unsigned int gpiod4             :1;
       unsigned int gpiod3             :1;
       unsigned int gpiod2             :1;
       unsigned int gpiod1             :1;
       unsigned int gpioc4             :1;
       unsigned int gpioc3             :1;
       unsigned int gpioc2             :1;
       unsigned int gpioc1             :1;
   } control_bit;
} TADS1299GPIO;


/****************************************************************/
/** Structure definition for PACE detect register (PACE)        */
/****************************************************************/
typedef union
{
   unsigned int value;
   struct
   {
       unsigned int reserved           :8;
       unsigned int rsv7               :2;
       unsigned int srb1               :1;
       unsigned int rsv4               :5;
  } control_bit;
} TADS1299MISC1;


/****************************************************************/
/** Structure definition for respiration control reg (RESP)     */
/****************************************************************/
typedef union
{
  unsigned int value;
  struct
  {
      unsigned int reserved           :8;
      unsigned int rsv                :8;
 } control_bit;
} TADS1299MISC2;


/****************************************************************/
/** Structure definition for config 4 control reg (CONFIG4)     */
/****************************************************************/
typedef union
{
   unsigned int value;
   struct
   {
       unsigned int reserved           :8;
       unsigned int rsv7               :4;
       unsigned int singleshot         :1;
       unsigned int rsv2               :1;
       unsigned int pdbloffcomp        :1;
       unsigned int rsv0               :1;
  } control_bit;
} TADS1299CONFIG4;

/****************************************************************/
/** Data converter register structure definition.
 *  This structure can be used to access the initialization
 *  values for the ADS1299 from the data converter object.      */
/****************************************************************/
typedef struct
{
    TADS1299ID          devid;
    TADS1299CONFIG1     config1;
    TADS1299CONFIG2     config2;
    TADS1299CONFIG3     config3;
    TADS1299LOFF        loff;
    TADS1299CHASET      ch1set;
    TADS1299CHASET      ch2set;
    TADS1299CHASET      ch3set;
    TADS1299CHASET      ch4set;
    TADS1299CHASET      ch5set;
    TADS1299CHASET      ch6set;
    TADS1299CHASET      ch7set;
    TADS1299CHASET      ch8set;
    TADS1299BIASSENSP   biassensp;
    TADS1299BIASSENSN   biassensn;
    TADS1299LOFFSENSP   loffsensp;
    TADS1299LOFFSENSN   loffsensn;
    TADS1299LOFFFLIP    loffflip;
    TADS1299LOFFSTATP   loffstatp;
    TADS1299LOFFSTATN   loffstatn;
    TADS1299GPIO        gpio;
    TADS1299MISC1       misc1;
    TADS1299MISC2       misc2;
    TADS1299CONFIG4     config4;
} TADS1299REGS;

/****************************************************************/
/** Buffer object definition for dual buffering                 */
/****************************************************************/
typedef struct
{
    void             (*ptrCallBack)(void *);/**< ptr to callback*/
    unsigned long    *ulBufPtr;      /**< read buffer pointer   */
    unsigned long    ulBufSize;      /**< read buffer size      */
    volatile unsigned int uiStatus;  /**<  status of the buffer */
} TADS1299BUFOBJ;


/****************************************************************/
/** \struct TADS1299
 *  Definition of the data converter object. This is the main
 *  object to work with.                                        */
/****************************************************************/
typedef struct
{
/** \var f
 *   Structure for the API function pointers.
 *   They are declared in tidc_api.h and defined in tidc_api.c. */
/** \var  DCP_ SERIAL *serial
 *   Pointer to the McBSP configuration
 *   structure as defined in the tidc_api.h.                    */
/** \var regs
 *  Structure for the registers of the ADS1299. This structure
 *  will be initialized by the code and the values there will
 *  be sent to the device during dc_configure(). Any change
 *  the register values need to be submitted to the device
 *  using the dc_control() function.                            */
/** \var initRegs
 *  This structure is the backup structure for TADS1299REGS regs.
 *  At startup, it is initialized to the same values as regs, but
 *  will not be changed during the program. The function
 *  dc_close() will use these values to re-initialize the
 *  converter with the original register settings.              */
/** \var xferBuffer
 *  Holds the structure of the tye TADS1299BUFOBJ, which is used
 *  to hold information about the submitted buffer              */
/** \var hDmaRcv
 *  Handle to the DMA channel for the data transfer from the
 *  serial port to the memory array. This handle will be created
 *  by the function DMA_open(), which is called during
 *  dc_configure().                                             */
/** \var hDmaXmt
 *  Handle to the DMA channel for the data transfer from the
 *  dummy memory location to the McBSP. This transfer is necessary
 *  to trigger the activity on the SCLK line. The handle will be
 *  created by the function DMA_open(), which is called during
 *  dc_configure().                                             */
/** \var iDmaChanNumber
 *  Number of the DMA channel to be used to collect the data
 *  from the data converter. This channel will be used to create
 *  the hDmRcv handle.                                          */
/** \var uiDrdyIntNum
 *  This variable holds the number of the interrupt line of the
 *  DSP, where the DRDY line is connected to.                   */
/** \var uiMcbspPeriod
 *  Value for the McBSP Period Register. Will be used to set the
 *  speed of the serial port during dc_configure().             */
/** \var iXferInProgress
 *  This value indicates if there is a data transfer ongoing and
 *  can have the following values:
 *      - 0: No transfer is ongoing
 *      - 1: One transfer is ongoing                            */
/** \var uiRcvCcrValue
 *  Backup for the receive DMA Channel Control Register.        */
/** \var uiXmtCcrValue
 *  Backup for the receive DMA Channel Control Register.        */
/****************************************************************/
    TTIDC3             f;
    DCP_SERIAL         *serial;
    TADS1299REGS       regs;
    TADS1299REGS       initRegs;
    TADS1299BUFOBJ     xferBuffer;
    DMA_Handle         hDmaRcv;
    DMA_Handle         hDmaXmt;
    int                iDmaChanNumber;
    unsigned int       uiDrdyIntNum;
    unsigned int       uiMcbspPeriod;
    volatile int       iXferInProgress;
    unsigned int       uiRcvCcrValue;
    unsigned int       uiXmtCcrValue;
} TADS1299;


/****************************************************************/
/* Define external DC object pointer(s)                         */
/****************************************************************/
extern TADS1299 Ads1299_1;
extern DCP_SERIAL serial1;


/****************************************************************/
/* Deklarations for the exported functions                      */
/****************************************************************/
TTIDCSTATUS ADS1299_configure(void *pDC);
TTIDCSTATUS ADS1299_control(void *pDC, int iCmd, void *iValue);
TTIDCSTATUS ADS1299_readsample(void *pDC, long *lData);
TTIDCSTATUS ADS1299_writesample(void *pDC, long lData);
TTIDCSTATUS ADS1299_readblock(void *pDC, void *pData,
                             unsigned long ulCount,
                             void (*callback) (void *));
TTIDCSTATUS ADS1299_writeblock(void *pDC, void *pData,
                              unsigned long ulCount,
                              void (*callback) (void *));
TTIDCSTATUS ADS1299_close(void *pDC);
interrupt void ADS1299_rblockFinished(void);
interrupt void ADS1299_drdyIsr(void);

/****************************************************************/
/* Define commands for the dc_control() function                */
/****************************************************************/
/* the defines for the read register and the write register
   commands are shifted by 16 bits, as these are 24-bit commands
   (16-bit for the command and 8-bits for the content).
   All other commands are as described in the data sheet        */
#define ADS1299_CMD_RREG                        (0x2000u)
#define ADS1299_CMD_WREG                        (0x4000u)
#define ADS1299_CMD_WAKEUP                      (0x0002u)
#define ADS1299_CMD_STANDBY                     (0x0004u)
#define ADS1299_CMD_RESET                       (0x0006u)
#define ADS1299_CMD_START                       (0x0008u)
#define ADS1299_CMD_STOP                        (0x000Au)
#define ADS1299_CMD_RDATAC                      (0x0010u)
#define ADS1299_CMD_SDATAC                      (0x0011u)
#define ADS1299_CMD_INITDEVICE                  (0x0100u)
/* offset calibration was removed from the new silicon          */
//#define ADS1299_CMD_OFFCAL                      (0x001Au)


/****************************************************************/
/* Define names for the register addresses                      */
/****************************************************************/
#define ADS1299_REG_DEVID         (0x0000u)
#define ADS1299_REG_CONFIG1       (0x0001u)
#define ADS1299_REG_CONFIG2       (0x0002u)
#define ADS1299_REG_CONFIG3       (0x0003u)
#define ADS1299_REG_LOFF          (0x0004u)
#define ADS1299_REG_CH1SET        (0x0005u)
#define ADS1299_REG_CH2SET        (0x0006u)
#define ADS1299_REG_CH3SET        (0x0007u)
#define ADS1299_REG_CH4SET        (0x0008u)
#define ADS1299_REG_CH5SET        (0x0009u)
#define ADS1299_REG_CH6SET        (0x000Au)
#define ADS1299_REG_CH7SET        (0x000Bu)
#define ADS1299_REG_CH8SET        (0x000Cu)
#define ADS1299_REG_BIASSENSP     (0x000Du)
#define ADS1299_REG_BIASSENSN     (0x000Eu)
#define ADS1299_REG_LOFFSENSP     (0x000Fu)
#define ADS1299_REG_LOFFSENSN     (0x0010u)
#define ADS1299_REG_LOFFFLIP      (0x0011u)
#define ADS1299_REG_LOFFSTATP     (0x0012u)
#define ADS1299_REG_LOFFSTATN     (0x0013u)
#define ADS1299_REG_GPIO          (0x0014u)
#define ADS1299_REG_MISC1         (0x0015u)
#define ADS1299_REG_MISC2         (0x0016u)
#define ADS1299_REG_CONFIG4       (0x0017u)

/****************************************************************/
/* ADS1299 defines for control registers bit set/clear          */
/****************************************************************/

/* other useful definitions                                     */
#define ADS1299_MAX_BUFFERS       (0x0002)


/****************************************************************/
/* Definitions for the basic data mover                         */
/****************************************************************/
#define ADS1299_DMA_INPUT   (0)      /* read from converter     */
#define ADS1299_DMA_OUTPUT  (1)
#define ADS1299_DMA_ENDPROG (0x0800u)/* mask for the ENDPROG bit*/
#define ADS1299_DMA_SYNC    (0x001Fu)/* mask for the SYNC bits  */


/****************************************************************/
/* Definitions for the DMA channel registers                    */
/****************************************************************/
/****************************************************************/
/* Define for the channel control register DMACCR               */
/* operating mode: Input                                        */
/* Value: 0x4104 = 0100 0001 0000 0101                          */
/*                 |||| |||| |||| ||||                          */
/*                 |||| |||| |||+ ++++- SYNC (rcv event 1)      */
/*                 |||| |||| ||+------- FS   (element)          */
/*                 |||| |||| |+-------- PRIO (low)              */
/*                 |||| |||| +--------- EN   (disable)          */
/*                 |||| |||+----------- AUTOINIT (on)           */
/*                 |||| ||+------------ REPEAT (off)            */
/*                 |||| |+------------- RESERVED                */
/*                 |||| +-------------- ENDPROG (off)           */
/*                 ||++---------------- SRCAMODE (constant)     */
/*                 ++------------------ DSTAMODE (autoincr.)    */
/****************************************************************/
#define ADS1299_DMACCR_INPUT_VALUE (0x4105u)

/****************************************************************/
/* define for the channel control register DMACCR               */
/* operating mode: Output                                       */
/* Value: 0x0150 = 0000 0001 0101 0000                          */
/*                 |||| |||| |||| ||||                          */
/*                 |||| |||| |||+ ++++- SYNC (xmt event 1)      */
/*                 |||| |||| ||+------- FS   (element)          */
/*                 |||| |||| |+-------- PRIO (high)             */
/*                 |||| |||| +--------- EN   (disable)          */
/*                 |||| |||+----------- AUTOINIT (on)           */
/*                 |||| ||+------------ REPEAT (off)            */
/*                 |||| |+------------- RESERVED                */
/*                 |||| +-------------- ENDPROG (off)           */
/*                 ||++---------------- SRCAMODE (constant)     */
/*                 ++------------------ DSTAMODE (constant)     */
/****************************************************************/
#define ADS1299_DMACCR_OUTPUT_VALUE   (0x0150u)

/****************************************************************/
/* define for the channel control register DMACCR               */
/* operating mode: Input / dummy transfer                       */
/* Value: 0x08C0 = 0000 1000 1100 0000                          */
/*                 |||| |||| |||| ||||                          */
/*                 |||| |||| |||+ ++++- SYNC (set by function)  */
/*                 |||| |||| ||+------- FS   (Element)          */
/*                 |||| |||| |+-------- PRIO (high)             */
/*                 |||| |||| +--------- EN   (enable)           */
/*                 |||| |||+----------- AUTOINIT (off)          */
/*                 |||| ||+------------ REPEAT (off)            */
/*                 |||| |+------------- RESERVED                */
/*                 |||| +-------------- ENDPROG (on)            */
/*                 ||++---------------- SRCAMODE (constant)     */
/*                 ++------------------ DSTAMODE (constant)     */
/****************************************************************/
#define ADS1299_DMACCR_VALUE_DUMMY (0x08C0u)


/****************************************************************/
/* define for the channel interrupt control register DMACICR    */
/* operating mode: input                                        */
/* Value: 0x0020 = 0000 0000 0010 0000                          */
/*                 |||| |||| |||| ||||                          */
/*                 |||| |||| |||| |||+- TIMEOUTIE (off)         */
/*                 |||| |||| |||| ||+-- DROPIE (off)            */
/*                 |||| |||| |||| |+--- HALFIE (off)            */
/*                 |||| |||| |||| +---- FRAMIE (off)            */
/*                 |||| |||| |||+------ LASTIE (off)            */
/*                 |||| |||| ||+------- BLOCKIE (on)            */
/*                 ++++ ++++ ++-------- RESERVED                */
/****************************************************************/
#define ADS1299_DMACICR_INPUT_VALUE   (0x0020u)


/****************************************************************/
/* define for the channel interrupt control register DMACICR    */
/* operating mode: output                                       */
/* Value: 0x0000 = 0000 0000 0000 0000                          */
/*                 |||| |||| |||| ||||                          */
/*                 |||| |||| |||| |||+- TIMEOUTIE (off)         */
/*                 |||| |||| |||| ||+-- DROPIE (off)            */
/*                 |||| |||| |||| |+--- HALFIE (off)            */
/*                 |||| |||| |||| +---- FRAMIE (off)            */
/*                 |||| |||| |||+------ LASTIE (off)            */
/*                 |||| |||| ||+------- BLOCKIE (off)           */
/*                 ++++ ++++ ++-------- RESERVED                */
/****************************************************************/
#define ADS1299_DMACICR_OUTPUT_VALUE   (0x0000u)


/****************************************************************/
/* define for the channel source destination parameter register */
/* DMACSDP                                                      */
/* operating mode: Input                                        */
/* Value: 0x000E = 0000 0000 0000 1110                          */
/*                 |||| |||| |||| ||||                          */
/*                 |||| |||| |||| ||++- DATATYPE (32 bit)       */
/*                 |||| |||| ||++-++--- SRC (peripheral)        */
/*                 |||| |||| |+-------- SRCPACK (off)           */
/*                 |||| |||+-+--------- SRCBEN (off)            */
/*                 |||+ +++------------ DST (set by function)   */
/*                 ||+- --------------- DSTPACK (off)           */
/*                 ++-- --------------- SRCBEN (off)            */
/****************************************************************/
#define ADS1299_DMACSDP_INPUT_VALUE  (0x000Eu)


/****************************************************************/
/* define for the channel source destination parameter register */
/* DMACSDP                                                      */
/* operating mode: Output                                       */
/* Value: 0x0602 = 0000 0110 0000 0010                          */
/*                 |||| |||| |||| ||||                          */
/*                 |||| |||| |||| ||++- DATATYPE (32 bit)       */
/*                 |||| |||| ||++-++--- SRC (set by function)   */
/*                 |||| |||| |+-------- SRCPACK (off)           */
/*                 |||| |||+-+--------- SRCBEN (off)            */
/*                 |||+ +++------------ DST (peripheral)        */
/*                 ||+- --------------- DSTPACK (off)           */
/*                 ++-- --------------- SRCBEN (off)            */
/****************************************************************/
#define ADS1299_DMACSDP_OUTPUT_VALUE  (0x0602u)


/****************************************************************/
/* define for the channel source destination parameter register */
/* DMACSDP                                                      */
/* operating mode: Output                                       */
/* Value: 0x0002 = 0000 0000 0000 0010                          */
/*                 |||| |||| |||| ||||                          */
/*                 |||| |||| |||| ||++- DATATYPE (32 bit)       */
/*                 |||| |||| ||++-++--- SRC (set by function)   */
/*                 |||| |||| |+-------- SRCPACK (off)           */
/*                 |||| |||+-+--------- SRCBEN (off)            */
/*                 |||+ +++------------ DST (set by function)   */
/*                 ||+- --------------- DSTPACK (off)           */
/*                 ++-- --------------- SRCBEN (off)            */
/****************************************************************/
#define ADS1299_DMACSDP_VALUE_DUMMY  (0x0002u)


/***************************************************************/
/* define values for the different memory areas, as they are   */
/* needed for the DMACSDP register at runtime                  */
/***************************************************************/
/* SRC/ DST  = 00 SARAM                                        */
/*           = 01 DARAM                                        */
/*           = 10 EMIF                                         */
/*           = 11 RESERVED                                     */
/***************************************************************/
#define ADS1299_DMACSDP_SARAM_SRC  (0x0000u)
#define ADS1299_DMACSDP_DARAM_SRC  (0x0004u)
#define ADS1299_DMACSDP_EMIF_SRC   (0x0008u)
#define ADS1299_DMACSDP_PERIPH_SRC (0x000Cu)


#define ADS1299_DMACSDP_SARAM_DST  (0x0000u)
#define ADS1299_DMACSDP_DARAM_DST  (0x0200u)
#define ADS1299_DMACSDP_EMIF_DST   (0x0400u)
#define ADS1299_DMACSDP_PERIPH_DST (0x0600u)


#endif /* __TADS1299_FN_H */

/****************************************************************/
/* END OF t1299_fn.h                                             */
/****************************************************************/

