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
/** \file
 * This interface software needs two interrupts to be mapped in
 * the DSP/BIOS configuration file:
 *    - Interrupt of the used DMA channel:
 *       -  _rblock_finished
 *    - Interrupt for the DRDY\ signal
 *       -  _drdyIsr
 *
 * This interface software uses two peripherals:
 *     - Two DMA channels for the data transfer
 *     - One McBSP for the data transfer
 *
 * <b>Restrictions:
 *    - This software needs the CS\ signal of the converter tied
 *      to ground permanently.                                  */
/****************************************************************/
#undef _INLINE

#include <std.h>
/* include the interface files for the data converter software  */
#include "dc_conf.h"
#include "t1299_fn.h"

/* include the interface files for the chip support library     */
#include <csl.h>
#include <csl_irq.h>
#include <csl_timer.h>
#include <string.h>
#include <hwi.h>
#include <tsk.h>
#include "mmb0.h"

/****************************************************************/
/* define and initialize data converter object                  */
/****************************************************************/
TADS1299 Ads1299_1 = {
    ADS1299_configure,
    ADS1299_control,
    ADS1299_readsample,
    ADS1299_writesample,
    ADS1299_readblock,
    ADS1299_writeblock,
    ADS1299_close,
    0, 0, 3,            /* reserved                             */
    &serial1,           /* serial port config struct            */
    ADC1_ADS1299DEVID,  /* register set                         */
    ADC1_ADS1299CONFIG1,
    ADC1_ADS1299CONFIG2,
    ADC1_ADS1299CONFIG3,
    ADC1_ADS1299LOFF,
    ADC1_ADS1299CH1SET,
    ADC1_ADS1299CH2SET,
    ADC1_ADS1299CH3SET,
    ADC1_ADS1299CH4SET,
    ADC1_ADS1299CH5SET,
    ADC1_ADS1299CH6SET,
    ADC1_ADS1299CH7SET,
    ADC1_ADS1299CH8SET,
    ADC1_ADS1299BIASSENSP,
    ADC1_ADS1299BIASSENSN,
    ADC1_ADS1299LOFFSENSP,
    ADC1_ADS1299LOFFSENSN,
    ADC1_ADS1299LOFFFLIP,
    ADC1_ADS1299LOFFSTATP,
    ADC1_ADS1299LOFFSTATN,
    ADC1_ADS1299GPIO,
    ADC1_ADS1299MISC1,
    ADC1_ADS1299MISC2,
    ADC1_ADS1299CONFIG4,
    ADC1_ADS1299DEVID,     /* backup regsiter set               */
    ADC1_ADS1299CONFIG1,
    ADC1_ADS1299CONFIG2,
    ADC1_ADS1299CONFIG3,
    ADC1_ADS1299LOFF,
    ADC1_ADS1299CH1SET,
    ADC1_ADS1299CH2SET,
    ADC1_ADS1299CH3SET,
    ADC1_ADS1299CH4SET,
    ADC1_ADS1299CH5SET,
    ADC1_ADS1299CH6SET,
    ADC1_ADS1299CH7SET,
    ADC1_ADS1299CH8SET,
    ADC1_ADS1299BIASSENSP,
    ADC1_ADS1299BIASSENSN,
    ADC1_ADS1299LOFFSENSP,
    ADC1_ADS1299LOFFSENSN,
    ADC1_ADS1299LOFFFLIP,
    ADC1_ADS1299LOFFSTATP,
    ADC1_ADS1299LOFFSTATN,
    ADC1_ADS1299GPIO,
    ADC1_ADS1299MISC1,
    ADC1_ADS1299MISC2,
    ADC1_ADS1299CONFIG4,
    0, 0, 0, 0,         /* Data Buffer Object                   */
    0,                  /* Dma handle (receive channel)         */
    0,                  /* Dma handle (transmit channel         */
    ADC1_DMACHAN,       /* used DMA Channel                     */
    ADC1_INTNUM,        /* Interrupt used for BUSY Signal       */
    ADC1_MCBSP_CLKDIV,  /* McBsp Period (to generate McBsp clock)*/
    0,                  /* Transfer-In-Progress Flag            */
    0,                  /* Shadow for DMACCR receive register   */
    0                   /* Shadow for DMACCR transmit register  */
};

TADS1299* hAdsDeviceHandle = &Ads1299_1;

/****************************************************************/
/* define and initialize the serial port object                 */
/****************************************************************/
DCP_SERIAL serial1 =
{
    0,                                /* McBSP handle            */
    ADC1_SERPORT,                    /* serial port number      */
    0                               /* INT used by serial port */
};


/****************************************************************/
/* DSP function prototypes                                      */
/****************************************************************/
static TTIDCSTATUS InitDevice(TADS1299 *pADS);
static TTIDCSTATUS InitMcbsp(DCP_SERIAL *serial,unsigned int clkdiv);
static TTIDCSTATUS InitRcvDma(const MCBSP_Handle hMcbsp,int iChanNum,DMA_Handle *hDma,unsigned int *pCcr);
static TTIDCSTATUS InitXmtDma(const MCBSP_Handle hMcbsp,DMA_Handle *hDma,unsigned int *pCcr);
static unsigned int ReadRegister(const TADS1299 *pADS,unsigned int uiRegister);
static inline TTIDCSTATUS SendCommand(MCBSP_Handle hMcbsp,unsigned long ulCmdValue);
static void SendReset(void);
static TTIDCSTATUS SetSclkCmdMode(const DCP_SERIAL *serial, unsigned int mode);
static TTIDCSTATUS SetWordLength(const MCBSP_Handle hMcbsp,unsigned int uiLength);
static TTIDCSTATUS SubmitBlock(const DMA_Handle hDmaRcv,const DMA_Handle hDmaXmt,unsigned int uiCcrRcv,unsigned int uiCcrXmt,unsigned long ulCnt,unsigned long ulDst,unsigned int uiDr);
static TTIDCSTATUS Wait(unsigned int uiWaitCount);
static TTIDCSTATUS WriteRegister(const TADS1299 *pADS,unsigned int uiRegister,const unsigned int *uiValue);


/****************************************************************/
/* global variables                                             */
/****************************************************************/
/* define a global pointer to the data converter object in use  */
static TADS1299 *pgADS;
static unsigned long ulDummy = 0;   /* needed for the DMA xfer  */
/* a is the semaphore of the startup of the DMA channel         */
int a=0;



/****************************************************************/
/* definitions used by the code                                 */
/****************************************************************/
#define ADS1299_WORDLENGTH_8BIT     (0x0000u)
#define ADS1299_WORDLENGTH_16BIT    (0x0002u)
#define ADS1299_WORDLENGTH_24BIT    (0x0004u)
#define ADS1299_WORDLENGTH_32BIT    (0x0005u)

#define ADS1299_CMDMODE_OFF         (0)
#define ADS1299_CMDMODE_ON          (1)
#define ADS1299_SCLK_DIV_CMD        (52)
#define ADS1299_SCLK_DIV_ACQ        (12)//(9)

#define ADS1299_RCV_SYNC            (0x0005u)

#define DIVIDER                     (65250)

/****************************************************************/
/* ADS1299_configure()                                          */
/**
 * Operation:     The following operations are performed:
 *     - Open, configure and start the McBSP the converter is
 *       connected to; return on error
 *     - Open and pre-configure the DMA channels used for the
 *       data transfer; return on error.
 *     - Initialize the registers of he ADS1299 to the values
 *       given in dc_conf.h; return on error.
 *
 * Parameters:
 *     - void* pDC: Data converter object
 *
 * Return values: Status code of type TTIDCSTATUS
 *     - TIDC_NO_ERR: Call was successful
 *     - TIDC_ERR_NODEVICE: No object was passed to the function
 *     - TIDC_ERR_MCBSP: The McBSP couldn't be opened
 *     - TIDC_ERR_DMA: The DMA channel could not be opened
 *     - TIDC_ERR_XFERPROG: dc_configure was called during an
 *       ongoing transfer
 *     - TIDC_ERR_BADARGS: An invalid argument was passed to the
 *       function
 *     - TIDC_ERR_REGS: The initialization of the registers failed
 *
 * Globals modified:
 *     - Serial port object in the data converter object
 *     - Backup for the receive channel DMA control register
 *     - Backup for the transmit channel DMA control register
 *
 * Resources used:
 *     - One McBSP
 *     - Two DMA channels
*/
/****************************************************************/
TTIDCSTATUS ADS1299_configure(void *pDC)
{
    TADS1299 *pADS = pDC;
    TTIDCSTATUS iStatus = TIDC_NO_ERR;

    /* return, if no device object available                    */
    if (pADS == 0)
        return TIDC_ERR_NODEVICE;

    /* be sure no block transfer is in progress                 */
    if (pADS->iXferInProgress != 0)
        return TIDC_ERR_XFERPROG;

    /* open & configure the McBSP needed for the data transfer  */
    iStatus = InitMcbsp(pADS->serial, pADS->uiMcbspPeriod);
    if (iStatus != TIDC_NO_ERR)
        return iStatus;

    /* open the DMA channel for the transmit and receive side   */
    /* and do a basic configuration for them                    */
    iStatus = InitRcvDma(pADS->serial->hMcbsp,
                         pADS->iDmaChanNumber,
                         &(pADS->hDmaRcv),
                         &(pADS->uiRcvCcrValue));
    if (iStatus != TIDC_NO_ERR)
    {
        (void)ADS1299_close(pADS);
        return iStatus;
    }

    iStatus = InitXmtDma(pADS->serial->hMcbsp,
                         &(pADS->hDmaXmt),
                         &(pADS->uiXmtCcrValue));
    if (iStatus != TIDC_NO_ERR)
    {
        (void)ADS1299_close(pADS);
        return iStatus;
    }

    /* take the device out of reset; this has to happen after   */
    /* McBSP was configured to avoid the spike on the SCLK line,*/
    /* which happens during the configuration of the serial port*/
    /* to propagate                                             */
    pin_set_GPIO2(1);
    (void)Wait(18);

    /* configure the registers of the device                    */
    iStatus = InitDevice(pADS);
    if (iStatus != TIDC_NO_ERR)
    {
        (void) ADS1299_close(pADS);
        return iStatus;
    }

    return TIDC_NO_ERR;
}


/****************************************************************/
/* ADS1299_control()                                            */
/** Operation:
 *     - Translates the command macros to the proper commands for
 *       the ADS1299.
 *     - Sets the word-length of the McBSP before sending the
 *       command.
 *     - Notes:
 *         * The RDATA command is not supported by this function.
 *           It will return with an error message.
 *         * The offset calibration routine does not wait until
 *           the command has finished. It only waits for a small
 *           amount of time and returns. This is due to the fact
 *           that otherwise the USB routines will time out.
 *
 * Parameters:
 *     - void* pDC: Data converter object
 *     - int iCmd:  Action to be performed. Valid are:
 *           - ADS1299_CMD_RREG:
 *                 Reads the register specified by the parameter
 *                 pValue and returns the read value in pValue.
 *           - ADS1299_CMD_WREG:
 *                 The register specified by the parameter pValue
 *                 will be initialized with the value in the
 *                 register structure of the pDC object. The
 *                 register will be read back and compared to the
 *                 sent value.
 *           - ADS1299_CMD_RDATAC:
 *                 Sends the "read data continuous" command to the
 *                 ADS1299.
 *           - ADS1299_CMD_SDATAC:
 *                 Sends the "stop read data continuous" command
 *                 to the converter
 *           - ADS1299_CMD_RESET:
 *                 Sends the "reset" command to the ADS1299.
 *           - ADS1299_CMD_START:
 *                 Sends the start conversion command.
 *           - ADS1299_CMD_STOP:
 *                 Sends the stop conversion command
 *           - ADS1299_CMD_STANDBY:
 *                 Puts the device in standby mode
 *           - ADS1299_CMD_WAKEUP:
 *                 Wakes the device from standby mode
 *     - void *pValue: Additional parameters for the command
 *                 (see above).
 *
 * Return values: Status code of type TTIDCSTATUS:
 *     - TIDC_NO_ERR:   No problem occurred.
 *     - TIDC_ERR_BADARGS: Either the command or the value has
 *       been invalid.
 *     - TIDC_ERR_REGS: The register could not be written. The
 *       compare between the intended value and the value read
 *       back failed.
 *
 * Globals modified:
 *     - None
 *
 * Resources used:
 *     - None                                                   */
/****************************************************************/
 TTIDCSTATUS ADS1299_control(void *pDc, int iCmd, void *pValue)
{
    TADS1299 *pADS = (TADS1299 *)pDc;
    unsigned int uiRegister;
    unsigned int uiValue;

    /* decide on the command                                    */
    switch (iCmd)
    {
        /* these are the simple 8-bit commands, which require no*/
        /* additional interaction                               */
        /* offset calibration was removed                       */
        /* case ADS1299_CMD_OFFCAL:                             */
        case ADS1299_CMD_RDATAC:
        case ADS1299_CMD_SDATAC:
        case ADS1299_CMD_STANDBY:
        case ADS1299_CMD_START:
        case ADS1299_CMD_STOP:
            (void)SetWordLength(pADS->serial->hMcbsp,
                                ADS1299_WORDLENGTH_8BIT);
            (void)SendCommand(pADS->serial->hMcbsp,
                              (unsigned long)iCmd);
            break;

        /* reset the device                                     */
        case ADS1299_CMD_RESET:
            pADS->regs = pADS->initRegs;
            /* make sure the device does not output data        */
            (void)SendCommand(pADS->serial->hMcbsp,(unsigned long)ADS1299_CMD_SDATAC);
            SendReset();
            /* stop the converter,as it was restarted by the reset      */
            (void)SendCommand(pADS->serial->hMcbsp,(unsigned long)ADS1299_CMD_SDATAC);
            (void)SendCommand(pADS->serial->hMcbsp,(unsigned long)ADS1299_CMD_STOP);
            break;

        /* reset the device and re-initialize it                */
        case ADS1299_CMD_INITDEVICE:
            pADS->regs = pADS->initRegs;
            (void)InitDevice(pADS);
            break;
            
        /* wakeup requires a waiting time after sending the cmd */
        case ADS1299_CMD_WAKEUP:
            (void)SetWordLength(pADS->serial->hMcbsp,
                                ADS1299_WORDLENGTH_8BIT);
            (void)SendCommand(pADS->serial->hMcbsp,
                              (unsigned long)iCmd);
            (void)Wait(8);
            break;

        /********************************************************/
        /* RREG: read from register                             */
        /********************************************************/
        case ADS1299_CMD_RREG:
            uiRegister = *(unsigned int *)pValue;
            *(unsigned int*)pValue = ReadRegister(pADS,
                                      uiRegister);
            break;

        /********************************************************/
        /* WREG write to register                               */
        /********************************************************/
        case ADS1299_CMD_WREG:
            uiRegister = *(unsigned int *)pValue;

            switch (uiRegister)
            {
                case ADS1299_REG_CONFIG1:
                    uiValue = pADS->regs.config1.value;
                    (void)WriteRegister(pADS, uiRegister, &uiValue);
                    break;
                case ADS1299_REG_CONFIG2:
                    uiValue = pADS->regs.config2.value;
                    (void)WriteRegister(pADS, uiRegister, &uiValue);
                    break;
                case ADS1299_REG_CONFIG3:
                    uiValue = pADS->regs.config3.value;
                    (void)WriteRegister(pADS, uiRegister, &uiValue);
                    break;
                case ADS1299_REG_LOFF:
                    uiValue = pADS->regs.loff.value;
                    (void)WriteRegister(pADS, uiRegister, &uiValue);
                    break;
                case ADS1299_REG_CH1SET:
                    uiValue = pADS->regs.ch1set.value;
                    (void)WriteRegister(pADS, uiRegister, &uiValue);
                    break;
                case ADS1299_REG_CH2SET:
                    uiValue = pADS->regs.ch2set.value;
                    (void)WriteRegister(pADS, uiRegister, &uiValue);
                    break;
                case ADS1299_REG_CH3SET:
                    uiValue = pADS->regs.ch3set.value;
                    (void)WriteRegister(pADS, uiRegister, &uiValue);
                    break;
                case ADS1299_REG_CH4SET:
                    uiValue = pADS->regs.ch4set.value;
                    (void)WriteRegister(pADS, uiRegister, &uiValue);
                    break;
                case ADS1299_REG_CH5SET:
                    uiValue = pADS->regs.ch5set.value;
                    (void)WriteRegister(pADS, uiRegister, &uiValue);
                    break;
                case ADS1299_REG_CH6SET:
                    uiValue = pADS->regs.ch6set.value;
                    (void)WriteRegister(pADS, uiRegister, &uiValue);
                    break;
                case ADS1299_REG_CH7SET:
                    uiValue = pADS->regs.ch7set.value;
                    (void)WriteRegister(pADS, uiRegister, &uiValue);
                    break;
                case ADS1299_REG_CH8SET:
                    uiValue = pADS->regs.ch8set.value;
                    (void)WriteRegister(pADS, uiRegister, &uiValue);
                    break;
                case ADS1299_REG_BIASSENSP:
                    uiValue = pADS->regs.biassensp.value;
                    (void)WriteRegister(pADS, uiRegister, &uiValue);
                    break;
                case ADS1299_REG_BIASSENSN:
                    uiValue = pADS->regs.biassensn.value;
                    (void)WriteRegister(pADS, uiRegister, &uiValue);
                    break;
                case ADS1299_REG_LOFFSENSP:
                    uiValue = pADS->regs.loffsensp.value;
                    (void)WriteRegister(pADS, uiRegister, &uiValue);
                    break;
                case ADS1299_REG_LOFFSENSN:
                    uiValue = pADS->regs.loffsensn.value;
                    (void)WriteRegister(pADS, uiRegister, &uiValue);
                    break;
                case ADS1299_REG_LOFFFLIP:
                    uiValue = pADS->regs.loffflip.value;
                    (void)WriteRegister(pADS, uiRegister, &uiValue);
                    break;
                case ADS1299_REG_GPIO:
                    uiValue = pADS->regs.gpio.value;
                    (void)WriteRegister(pADS, uiRegister, &uiValue);
                    break;
                case ADS1299_REG_MISC1:
                    uiValue = pADS->regs.misc1.value;
                    (void)WriteRegister(pADS, uiRegister, &uiValue);
                    break;
                case ADS1299_REG_MISC2:
                   uiValue = pADS->regs.misc2.value;
                   (void)WriteRegister(pADS, uiRegister, &uiValue);
                   break;
                case ADS1299_REG_CONFIG4:
                    uiValue = pADS->regs.config4.value;
                    (void)WriteRegister(pADS, uiRegister, &uiValue);
                    break;                                      
                default:
                    return TIDC_ERR_BADARGS;
            }
            break;
        /********************************************************/
        /* default: if you end up here, something got wrong     */
        /********************************************************/
        default:
            return TIDC_ERR_BADARGS;
    }

    return TIDC_NO_ERR;
}


/****************************************************************/
/* ADS1299_readsample()                                         */
/** Operation:
 *     - This function is not implemented
 *
 * Parameters:
 *     - void* pDC: Data converter object.
 *     - long *lData: Memory location used to return the
 *                    conversion result (out-parameter).
 *
 * Return value:  Status code of type TTIDCSTATUS
 *     - TIDC_ERR_FXNNULL: The function is not implemented
 *
 * Globals modified:
 *     - None
 *
 * Resources used:
 *     - None
 */
/****************************************************************/
TTIDCSTATUS ADS1299_readsample(void *pDC, long *lData)
{
    return TIDC_ERR_FXNNULL;
}


/****************************************************************/
/* ADS1299_readblock()                                          */
/** Operation:
 *     - Initializes the transfer of a block of data from the data
 *       converter. Once the transfer is started, the function
 *       returns to the application level.
 *     - It uses two DMA channels for the data transfer: One to
 *       trigger the SPI port and one to collect the conversion
 *       results. The DMA channel for the SPI port trigger will be
 *       selected by the code, while the DMA channel for the
 *       sample-read is fixed and was selected in the GUI of
 *       the DCP
 *     - Once the DMA transfer for the conversion results
 *       finishes, the rblock_finished ISR will be
 *       triggered , which will reset the iXferInProgress
 *       semaphore and will call the callback function, if
 *       requested.
 *
 * Parameters:
 *     - void* pDC: Data converter object
 *     - void pData: Pointer to the data buffer.
 *           - This buffer must be a multiple of the nine,
 *             otherwise not all channels will be read.
 *     - unsigned long ulCount: Size of the buffer in samples.
 *     - void *callback: Optional pointer to the callback function.
 *
 * Return value:  Status code of type TTIDCSTATUS
 *     - TIDC_NO_ERR: Call was successfull.
 *     - TIDC_ERR_NODEVICE: No object was passed to the function.
 *     - TIDC_ERR_BADARGS: An invalid argument was passed to the
 *       function
 *
 * Globals modified:
 *     - In data converter object:
 *           - *ulBufPtr
 *           - uiBufSize
 *           - ptrCallBack
 *           - iferInProgress
 *           - hDmaRcv
 *           - hDmaXmt
 *     - TADS1299* pgADS: File scoped pointer to data converter
 *                        object
 *
 * Remarks: Only one data transfer is allowed to be in progress
 *          at one time
 *
 * Resources used:
 *     - The two DMA channels opened by ADS1299_configure
 */
/****************************************************************/
TTIDCSTATUS ADS1299_readblock(void *pDC,
                              void *pData,
                              unsigned long ulCount,
                              void (*callback) (void *))
{
    TADS1299 *pADS = pDC;
    TTIDCSTATUS iStatus;

    /* return, if no device object available                    */
    if (pADS == 0)
        return TIDC_ERR_NODEVICE;

    if (ulCount < (unsigned long)0x00000009u)
        return TIDC_ERR_BADARGS;

    (void)IRQ_globalDisable();

    /* set tranfer in progress flag                             */
    if (pADS->iXferInProgress == 1)
        return TIDC_ERR_XFERPROG;
    else
        pADS->iXferInProgress = 1;

    /* set the global data converter object pointer             */
    pgADS = pADS;

    pADS->xferBuffer.ptrCallBack = callback;
    pADS->xferBuffer.ulBufPtr = (unsigned long *)pData;
    pADS->xferBuffer.ulBufSize = ulCount;
    pADS->xferBuffer.uiStatus = 1;

    (void)ADS1299_control(pADS,ADS1299_CMD_START, 0);
    (void)ADS1299_control(pADS,ADS1299_CMD_RDATAC, 0);

    /* set the wordlength to 24 bits                            */
    (void)SetWordLength(pADS->serial->hMcbsp,
                                ADS1299_WORDLENGTH_24BIT);

    (void)SetSclkCmdMode(pADS->serial, ADS1299_CMDMODE_OFF);

    /* make sure _really_ nothing goes on beside this       */
    (void)HWI_disable();
    TSK_disable();

    /* configure the DMA channels for the transfer          */
    iStatus = SubmitBlock(pADS->hDmaRcv,
                          pADS->hDmaXmt,
                          pADS->uiRcvCcrValue,
                          pADS->uiXmtCcrValue,
                          ulCount,
                          (unsigned long)pData,
                          pADS->regs.config1.control_bit.dr);

    if (iStatus != TIDC_NO_ERR)
    {
        pADS->iXferInProgress -= 1;
        (void)SetSclkCmdMode(pADS->serial, ADS1299_CMDMODE_ON);
        (void)ADS1299_control(pADS,ADS1299_CMD_SDATAC, 0);
        (void)ADS1299_control(pADS,ADS1299_CMD_STOP, 0);
        return iStatus;
    }

    TSK_enable();
    HWI_enable();
    (void)IRQ_enable(DMA_getEventId(pADS->hDmaRcv));
    IRQ_clear(IRQ_EVT_INT0);
    (void)IRQ_enable(IRQ_EVT_INT0);

    (void)IRQ_globalEnable();
    return TIDC_NO_ERR;
}


/****************************************************************/
/* ADS1299_writesample()                                        */
/** Operation:     Not implemented. This function just returns.
 *
 * Parameters:
 *     - void* pDC: Data converter object
 *     - long lData: Data to be written to the converter
 *
 * Return value:  Status code of type TTIDCSTATUS
 *     - TIDC_ERR_FXNNULL: This function is not implemented
 *
 * Globals modified:
 *     - None
 *
 * Resources used:
 *     - None
 */
/****************************************************************/
TTIDCSTATUS ADS1299_writesample(void *pDC, long lData)
{
    return TIDC_ERR_FXNNULL;
}


/****************************************************************/
/**
 * ADS1299_writeblock()
 * Operation:     Not implemented. This function just returns.
 *
 * Parameters:
 *     - void* pDC: Data converter object
 *     - void* lData: Pointer to the block of data to be written
 *     - unsigned long ulCount: Number of words
 *     - void (*callback): Pointer to the callback function
 *
 * Return value:  Status code of type TTIDCSTATUS
 *     - TIDC_ERR_FXNNULL: This function is not implemented
 *
 * Globals modified:
 *     - None
 *
 * Resources used:
 *     - None
 */
/****************************************************************/
TTIDCSTATUS ADS1299_writeblock(void *pDC, void *pData,
                              unsigned long ulCount,
                              void (*callback) (void *))
{
    return TIDC_ERR_FXNNULL;
}


/****************************************************************/
/* ADS1299_close()                                              */
/** Operation:
 *     - Closes all the resources used on the DSP.
 *     - Resets the converter and initializes the register array
 *       back to the original values to ensure that the converter
 *       can be re-opened again.
 *
 * Parameters:
 *     - void* pDC: Data converter object
 *
 * Return value:  Status code of type TTIDCSTATUS
 *     - TIDC_NO_ERR: No problem occurred
 *
 * Globals modified: The following items are modified inside
 *                   data converter object:
 *     - Handle to the serial port
 *     - Handle to the DMA channel for the data xfer
 *     - Handle to the DMA channel for the SCLK generation
 *     - The register structure for the data converter
 *     - iXferInProgress
 *
 * Resources used:
 *     - Two DMA channels (closed)
 *     - One McBSP channel (closed)
 */
/****************************************************************/
TTIDCSTATUS ADS1299_close(void *pDC)
{
    TADS1299 *pADS = pDC;

    /* return, if no device object available                    */
    if (pADS == 0)
        return TIDC_ERR_NODEVICE;

    /* stop conversions and reset the device, but only if the   */
    /* device was already opened                                */
    if ((pADS->serial->hMcbsp != 0) && (pADS->serial->hMcbsp != INV))
    {
        (void)ADS1299_control(pADS,ADS1299_CMD_SDATAC, 0);
        (void)Wait(4);
        (void)ADS1299_control(pADS,ADS1299_CMD_STOP, 0);
        (void)ADS1299_control(pADS, ADS1299_CMD_RESET, 0);
        MCBSP_close(pADS->serial->hMcbsp);
        pADS->serial->hMcbsp = 0;
    }

    /* close the transmit side DMA channel, if it was open      */
    if ((pADS->hDmaXmt != 0) && (pADS->hDmaXmt != INV))
    {
        DMA_close(pADS->hDmaXmt);
        pADS->hDmaXmt = 0;
    }

    /* close the receive side DMA channel, if it was open       */
    if ((pADS->hDmaRcv != 0) && (pADS->hDmaRcv != INV))
    {
        DMA_close(pADS->hDmaRcv);
        pADS->hDmaRcv = 0;
    }

    /* copy the original register values back to the working    */
    /* register set                                             */
    pADS->regs = pADS->initRegs;

    /* reset the transfer in progress semaphore                 */
    pADS->iXferInProgress = 0;

    return TIDC_NO_ERR;
}


/****************************************************************/
/* rblock_finished()                                    */
/**Operation:
 *     - Clears the DMA channel control register of the two
 *       channels used, if this was the last transfer in a chain.
 *     - Disables the DMA interrupt, if this was the last transfer
 *       in a chain.
 *     - Sets the status flag of the finished buffer to finished
 *     - Calls the callback-routine, if wished
 *     - Resets the status flag of the finished buffer to empty.
 *
 * Parameters:
 *     - None
 *
 * Return value:
 *     - None
 *
 * Globals modified:
 *     - iXferInProgress in the dc object
 *     - uiStatus inside the buffer object
 *
 * Resources used:
 *     - None
 */
/****************************************************************/
interrupt void rblock_finished(void)
{
    /* avoid additional synchronization events to the DMA   */
    /* by clearing the DMACCR register                      */
    DMA_RSETH(pgADS->hDmaRcv, DMACCR, 0);
    (void)IRQ_disable(DMA_getEventId(pgADS->hDmaRcv));

    (void)DMA_RGETH(pgADS->hDmaRcv, DMACSR);

    IRQ_clear(DMA_getEventId(pgADS->hDmaRcv));

    (void)SetSclkCmdMode(pgADS->serial, ADS1299_CMDMODE_ON);
    (void)ADS1299_control(pgADS,ADS1299_CMD_STOP, 0);
    (void)ADS1299_control(pgADS,ADS1299_CMD_SDATAC, 0);

    /* if there is a callback function requested, call it       */
    if (pgADS->xferBuffer.ptrCallBack)
    {
        pgADS->xferBuffer.uiStatus = 2;
        pgADS->xferBuffer.ptrCallBack(pgADS);
    }
    pgADS->xferBuffer.uiStatus = 0;

    /* signal that the transfer was successful                  */
    pgADS->iXferInProgress -= 1;
}


/****************************************************************/
/* drdyIsr()                                            */
/**Operation:
 *     - If this is the first transfer, the routine will send one
 *       word over the serial port manually. The remaining eight
 *       transfers for the first frame will then be done by the
 *       DMA controller. In the same step, the DMA controller is
 *       re-programmed to transfer nine words in the next frame.
 *     - The buffer size is automatically decremented by one frame.
 *     - This function is called on each frame, so it is necessary
 *       that it runs as fast as possible to make sure no samples
 *       are lost and the highest possible sampling rate is
 *       acheaved.
 *
 * Parameters:
 *     - None
 *
 * Return value:
 *     - None
 *
 * Globals modified:
 *     - uiBufSize in the data converter object
 *
 * Resources used:
 *     - None
 */
/****************************************************************/
#pragma CODE_SECTION(drdyIsr, ".isr")
interrupt void drdyIsr(void)
{
    if (pgADS->regs.config1.control_bit.dr < 0x0002u)
    {
        (void)SetWordLength(pgADS->serial->hMcbsp, ADS1299_WORDLENGTH_24BIT);

        IRQ_clear(pgADS->serial->intnum);

        /* get the status word, as this is the one which needs  */
        /* 24 bits                                              */
        *(volatile ioport unsigned int *)0x2C03 = 0;

        while (IRQ_test(pgADS->serial->intnum) == 0);

        /* now switch the serial port to 16 bits for the next   */
        /* 8 transfers                                          */
        if (pgADS->regs.config1.control_bit.dr < 0x0002u) {
            MCBSP_FSETH(pgADS->serial->hMcbsp, RCR1, RWDLEN1, ADS1299_WORDLENGTH_24BIT);
            MCBSP_FSETH(pgADS->serial->hMcbsp, XCR1, XWDLEN1, ADS1299_WORDLENGTH_24BIT);
        }

        /* this is the replacement for the ENDPROG bit set      */
        *(volatile ioport unsigned int *)0x0C01 = 0x09C6;

        if (a==0) {
            /* now we need to prime the serial port, as this is */
            /* the first transfer                               */
            *(volatile ioport unsigned int *)0x2C03 = 0;
            a=1;
        }
        DMA_RSETH(pgADS->hDmaXmt, DMACEN, 8);
    }
    else
    {
        if (a==0) {
            /* prime the McbSP; this is also the first data transfer*/
            *(volatile ioport unsigned int *)0x2C03 = 0;
        }

        /* this is the replacement for the ENDPROG bit set          */
        *(volatile ioport unsigned int *)0x0C01 = 0x09C6;

        if (a==0) {
            /* wait until the DMA has been reprogrammed             */
            while (DMA_FGETH(pgADS->hDmaXmt, DMACCR, ENDPROG) != 0);
            DMA_RSETH(pgADS->hDmaXmt, DMACEN, 9);
            a=1;
        }
    }

    /* while the transfer is running, there is enough time to   */
    /* take care of the counter                                 */
    pgADS->xferBuffer.ulBufSize -= 9;
    if (pgADS->xferBuffer.ulBufSize < 9)
        (void)IRQ_disable(IRQ_EVT_INT0);
}


/****************************************************************/
/* InitDevice()                                                 */
/** Operation:
 *     - Stops any ongoing transfer
 *     - Resets the device
 *     - Initializes the registers of the ADS1299 and verifies
 *       the settings.
 *     - This sequence follows the recommendations as given in
 *       the data sheet.
 *
 * Parameter:
 *     - void* pDC: Data converter object
 *
 * Return value:  Status code of type TTIDCSTATUS
 *     - TIDC_NO_ERR:   No problem occurred
 *     - TIDC_ERR_REGS: Register initialization failed
 *
 * Globals modified:
 *     - DEVID register in register structure
 *     -
 *
 * Resources used:
 *     - None
 */
/****************************************************************/
static TTIDCSTATUS InitDevice(TADS1299 *pADS)
{
    TTIDCSTATUS iStatus =  TIDC_NO_ERR;
    unsigned int uiReg;

    (void)ADS1299_control(pADS,ADS1299_CMD_SDATAC, 0);

    /* Reset the device                                         */
    SendReset();

    /* stop the converter,as it was restarted by the reset      */
    (void)ADS1299_control(pADS,ADS1299_CMD_SDATAC, 0);
    (void)ADS1299_control(pADS,ADS1299_CMD_STOP, 0);

    /* First, populate the read-only registers                  */
    uiReg = ADS1299_REG_DEVID;
    (void)ADS1299_control(pADS, ADS1299_CMD_RREG, (void *)&uiReg);
    pADS->regs.devid.value = uiReg;

    uiReg = ADS1299_REG_LOFFSTATP;
    (void)ADS1299_control(pADS, ADS1299_CMD_RREG, (void *)&uiReg);
    pADS->regs.loffstatp.value = uiReg;

    uiReg = ADS1299_REG_LOFFSTATN;
    (void)ADS1299_control(pADS, ADS1299_CMD_RREG, (void *)&uiReg);
    pADS->regs.loffstatn.value =  uiReg;

    /* now send all the register content and compare it         */
    uiReg = ADS1299_REG_CONFIG1;
    (void)ADS1299_control(pADS, ADS1299_CMD_WREG, (void *)&uiReg);
    (void)ADS1299_control(pADS, ADS1299_CMD_RREG, (void *)&uiReg);
    if ((uiReg & 0x00E7u) != (pADS->regs.config1.value & 0x00E7u))
        iStatus =   TIDC_ERR_REGS;

    uiReg = ADS1299_REG_CONFIG2;
    (void)ADS1299_control(pADS, ADS1299_CMD_WREG, (void *)&uiReg);
    (void)ADS1299_control(pADS, ADS1299_CMD_RREG, (void *)&uiReg);
    if ((uiReg & 0x0013u) != (pADS->regs.config2.value & 0x0013u))
        iStatus =   TIDC_ERR_REGS;

    uiReg = ADS1299_REG_CONFIG3;
    (void)ADS1299_control(pADS, ADS1299_CMD_WREG, (void *)&uiReg);
    (void)ADS1299_control(pADS, ADS1299_CMD_RREG, (void *)&uiReg);
    if ((uiReg & 0x00BCu) != (pADS->regs.config3.value & 0x00BCu))
        iStatus =   TIDC_ERR_REGS;

    uiReg = ADS1299_REG_LOFF;
    (void)ADS1299_control(pADS, ADS1299_CMD_WREG, (void *)&uiReg);
    (void)ADS1299_control(pADS, ADS1299_CMD_RREG, (void *)&uiReg);
    if ((uiReg) != pADS->regs.loff.value)
        iStatus =   TIDC_ERR_REGS;

    uiReg = ADS1299_REG_CH1SET;
    (void)ADS1299_control(pADS, ADS1299_CMD_WREG, (void *)&uiReg);
    (void)ADS1299_control(pADS, ADS1299_CMD_RREG, (void *)&uiReg);
    if ((uiReg & 0x00F7u) != (pADS->regs.ch1set.value & 0x00F7u))
        iStatus =   TIDC_ERR_REGS;

    uiReg = ADS1299_REG_CH2SET;
    (void)ADS1299_control(pADS, ADS1299_CMD_WREG, (void *)&uiReg);
    (void)ADS1299_control(pADS, ADS1299_CMD_RREG, (void *)&uiReg);
    if ((uiReg & 0x00F7u) != (pADS->regs.ch2set.value & 0x00F7u))
        iStatus =   TIDC_ERR_REGS;

    uiReg = ADS1299_REG_CH3SET;
    (void)ADS1299_control(pADS, ADS1299_CMD_WREG, (void *)&uiReg);
    (void)ADS1299_control(pADS, ADS1299_CMD_RREG, (void *)&uiReg);
    if ((uiReg & 0x00F7u) != (pADS->regs.ch3set.value & 0x00F7u))
        iStatus =   TIDC_ERR_REGS;

    uiReg = ADS1299_REG_CH4SET;
    (void)ADS1299_control(pADS, ADS1299_CMD_WREG, (void *)&uiReg);
    (void)ADS1299_control(pADS, ADS1299_CMD_RREG, (void *)&uiReg);
    if ((uiReg & 0x00F7u) != (pADS->regs.ch4set.value & 0x00F7u))
        iStatus =   TIDC_ERR_REGS;

    uiReg = ADS1299_REG_CH5SET;
    (void)ADS1299_control(pADS, ADS1299_CMD_WREG, (void *)&uiReg);
    (void)ADS1299_control(pADS, ADS1299_CMD_RREG, (void *)&uiReg);
    if ((uiReg & 0x00F7u) != (pADS->regs.ch5set.value & 0x00F7u))
        iStatus =   TIDC_ERR_REGS;

    uiReg = ADS1299_REG_CH6SET;
    (void)ADS1299_control(pADS, ADS1299_CMD_WREG, (void *)&uiReg);
    (void)ADS1299_control(pADS, ADS1299_CMD_RREG, (void *)&uiReg);
    if ((uiReg & 0x00F7u) != (pADS->regs.ch6set.value & 0x00F7u))
        iStatus =   TIDC_ERR_REGS;

    uiReg = ADS1299_REG_CH7SET;
    (void)ADS1299_control(pADS, ADS1299_CMD_WREG, (void *)&uiReg);
    (void)ADS1299_control(pADS, ADS1299_CMD_RREG, (void *)&uiReg);
    if ((uiReg & 0x00F7u) != (pADS->regs.ch7set.value & 0x00F7u))
        iStatus =   TIDC_ERR_REGS;

    uiReg = ADS1299_REG_CH8SET;
    (void)ADS1299_control(pADS, ADS1299_CMD_WREG, (void *)&uiReg);
    (void)ADS1299_control(pADS, ADS1299_CMD_RREG, (void *)&uiReg);
    if ((uiReg & 0x00F7u) != (pADS->regs.ch8set.value & 0x00F7u))
        iStatus =   TIDC_ERR_REGS;

    uiReg = ADS1299_REG_BIASSENSP;
    (void)ADS1299_control(pADS, ADS1299_CMD_WREG, (void *)&uiReg);
    (void)ADS1299_control(pADS, ADS1299_CMD_RREG, (void *)&uiReg);
    if (uiReg != pADS->regs.biassensp.value)
        iStatus =   TIDC_ERR_REGS;

    uiReg = ADS1299_REG_BIASSENSN;
    (void)ADS1299_control(pADS, ADS1299_CMD_WREG, (void *)&uiReg);
    (void)ADS1299_control(pADS, ADS1299_CMD_RREG, (void *)&uiReg);
    if (uiReg != pADS->regs.biassensn.value)
        iStatus =   TIDC_ERR_REGS;

    uiReg = ADS1299_REG_LOFFSENSP;
    (void)ADS1299_control(pADS, ADS1299_CMD_WREG, (void *)&uiReg);
    (void)ADS1299_control(pADS, ADS1299_CMD_RREG, (void *)&uiReg);
    if (uiReg != pADS->regs.loffsensp.value)
        iStatus =   TIDC_ERR_REGS;

    uiReg = ADS1299_REG_LOFFSENSN;
    (void)ADS1299_control(pADS, ADS1299_CMD_WREG, (void *)&uiReg);
    (void)ADS1299_control(pADS, ADS1299_CMD_RREG, (void *)&uiReg);
    if (uiReg != pADS->regs.loffsensn.value)
        iStatus =   TIDC_ERR_REGS;

    uiReg = ADS1299_REG_LOFFFLIP;
    (void)ADS1299_control(pADS, ADS1299_CMD_WREG, (void *)&uiReg);
    (void)ADS1299_control(pADS, ADS1299_CMD_RREG, (void *)&uiReg);
    if (uiReg != pADS->regs.loffflip.value)
        iStatus =   TIDC_ERR_REGS;

    uiReg = ADS1299_REG_GPIO;
    (void)ADS1299_control(pADS, ADS1299_CMD_WREG, (void *)&uiReg);
    (void)ADS1299_control(pADS, ADS1299_CMD_RREG, (void *)&uiReg);
    if (uiReg != pADS->regs.gpio.value)
        iStatus =   TIDC_ERR_REGS;

    uiReg = ADS1299_REG_MISC1;
    (void)ADS1299_control(pADS, ADS1299_CMD_WREG, (void *)&uiReg);
    (void)ADS1299_control(pADS, ADS1299_CMD_RREG, (void *)&uiReg);
    if ((uiReg & 0x001Fu) != (pADS->regs.misc1.value & 0x001Fu))
        iStatus =   TIDC_ERR_REGS;

    uiReg = ADS1299_REG_MISC2;
    (void)ADS1299_control(pADS, ADS1299_CMD_WREG, (void *)&uiReg);
    (void)ADS1299_control(pADS, ADS1299_CMD_RREG, (void *)&uiReg);
    if (uiReg != pADS->regs.misc2.value)
        iStatus =   TIDC_ERR_REGS;

    uiReg = ADS1299_REG_CONFIG4;
    (void)ADS1299_control(pADS, ADS1299_CMD_WREG, (void *)&uiReg);
    (void)ADS1299_control(pADS, ADS1299_CMD_RREG, (void *)&uiReg);
    if ((uiReg & 0x00EAu) != (pADS->regs.config4.value & 0x00EAu))
        iStatus =   TIDC_ERR_REGS;

    return iStatus;
}


/****************************************************************/
/* InitMcbsp()                                                  */
/** Operation:
 *     - Opens, initializes and starts the McBSP given by the port
 *       variable in the serial port structure.
 *
 * Parameters:
 *     - DCP_SERIAL serial: Serial port object.
 *     - unsigned int clkdiv: Divider for the sample rate
                            generator.
 *
 * Return value:  Status code of type TTIDCSTATUS
 *     - TIDC_NO_ERR:   No problem occurred.
 *     - TIDC_ERR_MCBSP: The serial port couldn't be opened.
 *
 * Globals modified:
 *     - Handle in the serial port structure.
 *     - Interrupt number in the serial port structure.
 *
 * Resources used:
 *     - None
 */
/****************************************************************/
static TTIDCSTATUS InitMcbsp(DCP_SERIAL *serial,unsigned int clkdiv)
{
    /* clear the serial port structure                          */
    memset(&serial->sConfig, 0x0000, sizeof(serial->sConfig));

    /* open the port and return on fail                         */
    serial->hMcbsp = MCBSP_open((int)serial->port,
                                MCBSP_OPEN_RESET);
    if (serial->hMcbsp == INV)
        return TIDC_ERR_MCBSP;

    /* initialize the serial port structure                     */
    /*  Serial Port Control Register 1                          */
    serial->sConfig.spcr1 =
          MCBSP_FMK(SPCR1, DLB, MCBSP_SPCR1_DLB_OFF)
        | MCBSP_FMK(SPCR1, RJUST, MCBSP_SPCR1_RJUST_RSE)
        | MCBSP_FMK(SPCR1, CLKSTP, MCBSP_SPCR1_CLKSTP_NODELAY)
        | MCBSP_FMK(SPCR1, DXENA, MCBSP_SPCR1_DXENA_OFF)
        | MCBSP_FMK(SPCR1, ABIS, MCBSP_SPCR1_ABIS_DISABLE)
        | MCBSP_FMK(SPCR1, RINTM, MCBSP_SPCR1_RINTM_RRDY)
        | MCBSP_FMK(SPCR1, RSYNCERR, MCBSP_SPCR1_RSYNCERR_NO)
        | MCBSP_FMK(SPCR1, RFULL, MCBSP_SPCR1_RFULL_NO)
        | MCBSP_FMK(SPCR1, RRDY, MCBSP_SPCR1_RRDY_NO)
        | MCBSP_FMK(SPCR1, RRST, MCBSP_SPCR1_RRST_DISABLE);

    /*  Serial Port Control Register 2                          */
    serial->sConfig.spcr2 =
        MCBSP_FMK(SPCR2, FREE, MCBSP_SPCR2_FREE_YES)
        | MCBSP_FMK(SPCR2, SOFT, MCBSP_SPCR2_SOFT_YES)
        | MCBSP_FMK(SPCR2, FRST, MCBSP_SPCR2_FRST_RESET)
        | MCBSP_FMK(SPCR2, GRST, MCBSP_SPCR2_GRST_RESET)
        | MCBSP_FMK(SPCR2, XINTM, MCBSP_SPCR2_XINTM_XRDY)
        | MCBSP_FMK(SPCR2, XSYNCERR, MCBSP_SPCR2_XSYNCERR_NO)
        | MCBSP_FMK(SPCR2, XEMPTY, MCBSP_SPCR2_XEMPTY_NO)
        | MCBSP_FMK(SPCR2, XRDY, MCBSP_SPCR2_XRDY_NO)
        | MCBSP_FMK(SPCR2, XRST, MCBSP_SPCR2_XRST_DISABLE);

    /*  Receive Control Register 1                              */
    serial->sConfig.rcr1 =
        MCBSP_FMK(RCR1, RFRLEN1, MCBSP_RCR1_RFRLEN1_OF(0))
        | MCBSP_FMK(RCR1, RWDLEN1, MCBSP_RCR1_RWDLEN1_8BIT);

    /*  Receive Control Register 2                              */
    serial->sConfig.rcr2 =
        MCBSP_FMK(RCR2, RPHASE, MCBSP_RCR2_RPHASE_SINGLE)
        | MCBSP_FMK(RCR2, RFRLEN2, MCBSP_RCR2_RFRLEN2_OF(0))
        | MCBSP_FMK(RCR2, RWDLEN2, MCBSP_RCR2_RWDLEN2_8BIT)
        | MCBSP_FMK(RCR2, RCOMPAND, MCBSP_RCR2_RCOMPAND_MSB)
        | MCBSP_FMK(RCR2, RFIG, MCBSP_RCR2_RFIG_NO)
        | MCBSP_FMK(RCR2, RDATDLY, MCBSP_RCR2_RDATDLY_1BIT);

    /*  Transmit Control Register 1                             */
    serial->sConfig.xcr1 =
        MCBSP_FMK(XCR1, XFRLEN1, MCBSP_XCR1_XFRLEN1_OF(0))
        | MCBSP_FMK(XCR1, XWDLEN1, MCBSP_XCR1_XWDLEN1_8BIT);

    /*  Transmit Control Register 2                             */
    serial->sConfig.xcr2 =
        MCBSP_FMK(XCR2, XPHASE, MCBSP_XCR2_XPHASE_SINGLE)
        | MCBSP_FMK(XCR2, XFRLEN2, MCBSP_XCR2_XFRLEN2_OF(0))
        | MCBSP_FMK(XCR2, XWDLEN2, MCBSP_XCR2_XWDLEN2_8BIT)
        | MCBSP_FMK(XCR2, XCOMPAND, MCBSP_XCR2_XCOMPAND_MSB)
        | MCBSP_FMK(XCR2, XFIG, MCBSP_XCR2_XFIG_NO)
        | MCBSP_FMK(XCR2, XDATDLY, MCBSP_XCR2_XDATDLY_1BIT);

    /*  Sample Rate Generator Register 1                        */
    serial->sConfig.srgr1 =
        MCBSP_FMK(SRGR1, FWID, MCBSP_SRGR1_FWID_OF(0))
        | MCBSP_FMK(SRGR1, CLKGDV, MCBSP_SRGR1_CLKGDV_OF(clkdiv));

    /*  Sample Rate Generator Register 2                        */
    serial->sConfig.srgr2 =
        MCBSP_FMK(SRGR2, GSYNC, MCBSP_SRGR2_GSYNC_FREE)
        | MCBSP_FMK(SRGR2, CLKSP, MCBSP_SRGR2_CLKSP_RISING)
        | MCBSP_FMK(SRGR2, CLKSM, MCBSP_SRGR2_CLKSM_INTERNAL)
        | MCBSP_FMK(SRGR2, FSGM, MCBSP_SRGR2_FSGM_DXR2XSR)
        | MCBSP_FMK(SRGR2, FPER, MCBSP_SRGR2_FPER_OF(0));

    /*  Pin Control Register                                    */
    serial->sConfig.pcr =
        MCBSP_FMK(PCR, IDLEEN, MCBSP_PCR_IDLEEN_RESET)
        | MCBSP_FMK(PCR, XIOEN, MCBSP_PCR_XIOEN_SP)
        | MCBSP_FMK(PCR, RIOEN, MCBSP_PCR_RIOEN_SP)
        | MCBSP_FMK(PCR, FSXM, MCBSP_PCR_FSXM_INTERNAL)
        | MCBSP_FMK(PCR, FSRM, MCBSP_PCR_FSRM_EXTERNAL)
        | MCBSP_FMK(PCR, SCLKME, MCBSP_PCR_SCLKME_NO)
        | MCBSP_FMK(PCR, CLKSSTAT, MCBSP_PCR_CLKSSTAT_0)
        | MCBSP_FMK(PCR, DXSTAT, MCBSP_PCR_DXSTAT_0)
        | MCBSP_FMK(PCR, DRSTAT, MCBSP_PCR_DRSTAT_0)
        | MCBSP_FMK(PCR, CLKXM, MCBSP_PCR_CLKXM_OUTPUT)
        | MCBSP_FMK(PCR, CLKRM, MCBSP_PCR_CLKRM_INPUT)
        | MCBSP_FMK(PCR, FSXP, MCBSP_PCR_FSXP_ACTIVELOW)
        | MCBSP_FMK(PCR, FSRP, MCBSP_PCR_FSRP_ACTIVELOW)
        | MCBSP_FMK(PCR, CLKXP, MCBSP_PCR_CLKXP_RISING)
        | MCBSP_FMK(PCR, CLKRP, MCBSP_PCR_CLKRP_FALLING);

    /* configure and start the port                             */
    MCBSP_config(serial->hMcbsp, &serial->sConfig);
    serial->intnum = MCBSP_getRcvEventId(serial->hMcbsp);
    (void)MCBSP_start(serial->hMcbsp,
                      MCBSP_XMIT_START
                      | MCBSP_RCV_START
                      | MCBSP_SRGR_START
                      | MCBSP_SRGR_FRAMESYNC,
                      0xE000);

    return TIDC_NO_ERR;
}


/****************************************************************/
/* InitRcvDma()                                                 */
/**Operation:
 *     - Opens the DMA channel needed for reading words from the
 *       converter; returns on error.
 *     - Pre-initializes the DMA configuration with the values
 *       which are not dependend on the buffer-address and the
 *       buffer size.
 *     - The final configuration will be done by the
 *       ADS1299_submitBlock() function.
 *     - This function does not touch the DMA Global control
 *       register and does not enable the interrupts associated
 *       with the channels.
 *     - It also does not start the DMA channel. This is the task
 *       of the ADS1299_readblock() function.
 *
 * Parameters:
 *     - MCBSP_Handle hMcbsp: Handle for the McBSP used
 *     - int iChanNum: The number of the channel to be used
 *     - DMA_Handle: Pointer to the hDmaRcv handle in the data
 *              converter structure
 *     - unsigned int *pCcr: Pointer to the DMA control register
 *              variable in the data converter object.
 *
 * Return value:
 *     - TIDC_NO_ERR in case everything was OK
 *     - TIDC_ERR_DMA if the DMA channel could not be opened.
 *
 * Globals modified: Inside the data converter object:
 *     - hDmaRcv
 *     - uiRcvCcrValue
 *
 * Resources used:
 *     - One DMA channel
 */
/****************************************************************/
static TTIDCSTATUS InitRcvDma(const MCBSP_Handle hMcbsp,
                              int iChanNum,
                              DMA_Handle *hDma,
                              unsigned int *pCcr)
{
    DMA_Config cfgDma;                /* defined in the CSL     */
    unsigned long ulSrc;              /* holds destination addr */

    *hDma = DMA_open(iChanNum, 0);
    if (*hDma == INV)
        return TIDC_ERR_DMA;

    /* get the default values of the channel registers          */
    DMA_getConfig(*hDma, &cfgDma);

    /* now create the correct settings for the DMACCR register  */
    *pCcr = ADS1299_DMACCR_INPUT_VALUE;

    /* DMACICR (channel interrupt control) setup                */
    cfgDma.dmacicr = ADS1299_DMACICR_INPUT_VALUE;

    /* DMACSSAL/AU (channel source start address low/high) setup*/
    /* type DMA_AdrPtr is defined in the CSL                    */
    ulSrc = ((unsigned long)MCBSP_ADDRH(hMcbsp, DRR2)) << 1;
    cfgDma.dmacssal = (DMA_AdrPtr)(ulSrc);
    cfgDma.dmacssau = (Uint16)((ulSrc >> 16));

    /* DMACEN (channel element number) setup                    */
    cfgDma.dmacen = 0;

    /* DMACFN (channel frame number) setup                      */
    cfgDma.dmacfn = 1; /* we will only transfer a single frame  */

    /* Now that all registers are set-up, to the actual         */
    /* configuration of the DMA channel                         */
    DMA_config(*hDma, &cfgDma);

    return TIDC_NO_ERR;
}


/****************************************************************/
/* InitXmtDma()                                                 */
/**Operation:
 *     - Opens the DMA channel needed for triggering the SCLK;
 *       returns on error.
 *     - Pre-initializes the DMA configuration with the values
 *       which are not dependend on the buffer-address and the
 *       buffer size.
 *     - The final configuration will be done by the
 *       ADS1299_submitBlock() function.
 *     - This function does not touch the DMA Global control
 *       register and does not enable the interrupts associated
 *       with the channels.
 *     - It also does not start the DMA channel. This is the task
 *       of the ADS1299_readblock() function.
 *
 * Parameters:
 *     - MCBSP_Handle hMcbsp: Handle for the McBSP used
 *     - unsigned int uiIrq: Interrupt number for the DRDY signal
 *     - DMA_Handle: Pointer to the hDmaXmt handle in the data
 *              converter structure
 *     - unsigned int *pCcr: Pointer to the DMA control register
 *              variable in the data converter object.
 *
 * Return value:
 *     - TIDC_NO_ERR in case everything was OK
 *     - TIDC_ERR_DMA if the DMA channel could not be opened.
 *
 * Globals modified: Inside the data converter object:
 *     - hDmaRcv
 *     - uiXmtCcrValue
 *
 * Resources used:
 *     - One DMA channel
 */
/****************************************************************/
static TTIDCSTATUS InitXmtDma(const MCBSP_Handle hMcbsp,
                              DMA_Handle *hDma,
                              unsigned int *pCcr)
{
    DMA_Config cfgDma;                /* defined in the CSL     */
    unsigned int uiSrcPort;           /* holds the memory type  */
    unsigned long ulDst;
    unsigned long ulSrc;

    *hDma = DMA_open(DMA_CHA_ANY, 0);
    if (*hDma == INV)
        return TIDC_ERR_DMA;

    /* get the default values of the channel registers          */
    DMA_getConfig(*hDma, &cfgDma);

    /* DMACCR (channel control register) setup                  */
    *pCcr = ADS1299_DMACCR_OUTPUT_VALUE;

    /* DMACSDP (channel source destination paramter setup       */
    ulSrc = (unsigned long)&ulDummy << 1;
    uiSrcPort = ADS1299_DMACSDP_EMIF_SRC;
    /* is the address in DARAM ?                            */
    if (ulSrc < DCP_DARAM_END)
        uiSrcPort = ADS1299_DMACSDP_DARAM_SRC;
    /* is the address in SARAM                              */
    if (ulSrc >= DCP_SARAM_START && ulSrc <= DCP_SARAM_END)
        uiSrcPort = ADS1299_DMACSDP_SARAM_SRC;

    cfgDma.dmacsdp = ADS1299_DMACSDP_OUTPUT_VALUE
                     | uiSrcPort;         /* src memory type    */

    /* DMACICR (channel interrupt control) setup                */
    /* modify value according to requested end of xfer interrupt*/
    cfgDma.dmacicr = ADS1299_DMACICR_OUTPUT_VALUE;

    /* DMACSSAL/AU (channel source start address low/high) setup*/
    /* type DMA_AdrPtr is defined in the CSL                    */
    cfgDma.dmacssal = (DMA_AdrPtr)(ulSrc);
    cfgDma.dmacssau = (Uint16)((ulSrc >> 16));

    /* DMACDSAL/AU (channel dest. start address low/high) setup */
    ulDst = ((unsigned long)MCBSP_ADDRH(hMcbsp, DXR2)) << 1;
    cfgDma.dmacdsal = (DMA_AdrPtr)(ulDst);
    cfgDma.dmacdsau = (unsigned short)(ulDst >> 16);

    /* DMACEN (channel element number) setup                    */
    cfgDma.dmacen = 0;

    /* DMACFN (channel frame number) setup                      */
    cfgDma.dmacfn = 1; /* we will only transfer a single frame  */

    /* Now that all registers are set-up, to the actual         */
    /* configuration of the DMA channel                         */
    DMA_config(*hDma, &cfgDma);

    return TIDC_NO_ERR;
}


/****************************************************************/
/* ReadRegister()                                              */
/** Operation:
 *      - Reads the register specified by the parameter uiRegister
 *        and returns it.
 *
 * Parameters:
 *      - TADS1299 *pADS:Data converter object
 *      - unsigned int uiRegister: The address of the register to
 *        read.
 *
 * Return value:
 *      - The contents of the read register.
 *
 * Globals modified:
 *     - None
 *
 * Resources used:
 *     - None
 */
/****************************************************************/
static unsigned int ReadRegister(const TADS1299 *pADS,
                                 unsigned int uiRegister)
{
    unsigned int uiReadValue;

    /* change the McBSP mode to 24 bits                         */
    (void)SetWordLength(pADS->serial->hMcbsp,
                        ADS1299_WORDLENGTH_24BIT);
    /* send the command word                                    */
    MCBSP_write32(pADS->serial->hMcbsp,
                  (unsigned long)uiRegister << 16 | (unsigned long)ADS1299_CMD_RREG << 8);

    /* and get the data                                         */
    while (MCBSP_FGETH(pADS->serial->hMcbsp, SPCR1, RRDY) != 1);

    uiReadValue = (unsigned int)(MCBSP_read32(pADS->serial->hMcbsp) & 0x00FFu);

    return uiReadValue;
}


/****************************************************************/
/* SendCommand()                                                */
/** Operation:
 *      - Waits until the McBSP is ready to transmit a new word
 *        and send the command to the device.
 *      - Waits until the dummy word is received and discards it.
 *
 * Parameters:
 *      - MCBSP_Handle hMcbsp: The handle to the serial port
 *        in use.
 *        int iCmdValue: The binary opcode for the command to send
 *
 * Return value:
 *     - TIDC_NO_ERR in case everything was OK
 *
 * Globals modified:
 *     - None
 *
 * Resources used:
 *     - None
 */
/****************************************************************/
static inline TTIDCSTATUS SendCommand(MCBSP_Handle hMcbsp,
                                      unsigned long ulCmdValue)
{
    unsigned int uiCmdWidth;

    /* get the width of the command                             */
    uiCmdWidth = MCBSP_FGETH(hMcbsp, XCR1, XWDLEN1);

    /* wait until the McBSP is ready to send data               */
    while (MCBSP_FGETH(hMcbsp,SPCR2, XRDY) != 1);
    /* send the command to the converter                        */
    if (uiCmdWidth == ADS1299_WORDLENGTH_24BIT)
        MCBSP_write32(hMcbsp, ulCmdValue);
    else
        MCBSP_write16(hMcbsp, (unsigned int)ulCmdValue);

    /* wait until the McBSP is ready to receive data            */
    while (MCBSP_FGETH(hMcbsp,SPCR1, RRDY) != 1) ;
    /* discard the received word, as it is meaningless          */
    if (uiCmdWidth == ADS1299_WORDLENGTH_24BIT)
        (void)MCBSP_read32(hMcbsp);
    else
        (void)MCBSP_read16(hMcbsp);

    return TIDC_NO_ERR;
}

/****************************************************************/
/* SendReset()                                                  */
/** Operation:
 *      - Toggles the RESET pin of the device
 *
 * Parameters:
 *      - None
 *
 * Return value:
 *     - None
 *
 * Globals modified:
 *     - None
 *
 * Resources used:
 *     - None
 */
/****************************************************************/
static void SendReset(void)
{
    /* issue a reset command, but wait for 18 t(clk) afterwards */
    pin_set_GPIO2(0);
    (void)Wait(2);
    pin_set_GPIO2(1);
    (void)Wait(18);
}


/****************************************************************/
/* SetSclkCmdMode()                                             */
/** Operation:
 *      - As some of the command on the ADS1299 cannot work at the
 *        full SCLK speed, the SCLK frequency needs to be reduced
 *        for commands. This is the function to perform this
 *        action.
 *
 * Parameters:
 *      - TADS1299 *pADS:Data converter object
 *      - unsigned int mode:
 *           - 0: command mode off, fast SCLK
 *           - 1: command mode on, slow SCLK
 *
 * Return value:
 *     - TIDC_NO_ERR in case everything was OK
 *
 * Globals modified:
 *     - None
 *
 * Resources used:
 *     - None
 */
/****************************************************************/
static TTIDCSTATUS SetSclkCmdMode(const DCP_SERIAL *serial, unsigned int mode)
{
    /* reset the serial port                                    */
    MCBSP_FSETH(serial->hMcbsp, SPCR1, RRST, 0); //reset rcv
    MCBSP_FSETH(serial->hMcbsp, SPCR2, XRST, 0); //reset xmt
    MCBSP_FSETH(serial->hMcbsp, SPCR2, FRST, 0); //reset SRGR
    MCBSP_FSETH(serial->hMcbsp, SPCR2, GRST, 0); //reset FSR

    if (mode==ADS1299_CMDMODE_ON)
        MCBSP_FSETH(serial->hMcbsp, SRGR1, CLKGDV, ADS1299_SCLK_DIV_CMD);
    else if ((mode==ADS1299_CMDMODE_OFF))
        MCBSP_FSETH(serial->hMcbsp, SRGR1, CLKGDV, ADS1299_SCLK_DIV_ACQ);

    MCBSP_FSETH(serial->hMcbsp, SPCR1, RRST, 1); //start rcv
    MCBSP_FSETH(serial->hMcbsp, SPCR2, XRST, 1); //reset xmt
    MCBSP_FSETH(serial->hMcbsp, SPCR2, GRST, 1); //start FSR
    MCBSP_FSETH(serial->hMcbsp, SPCR2, FRST, 1); //start SRGR

    return TIDC_NO_ERR;
}


/****************************************************************/
/* SetWordLength()                                      */
/** Operation:
 *     - Changes the receive and transmit wordlength of the serial
 *       port in use, if the paremeter length does not match the
 *       current setting.
 *
 * Parameters:
 *     - MCBSP_Handle hMcbsp: Handle to the McBSP
 *     - unsigned int uiLength: Desired wordlength
 *
 * Return value:  Status code of type TTIDCSTATUS
 *     - TIDC_NO_ERR:   No problem occurred
 *
 * Globals modified:
 *     - None
 *
 * Resources used:
 *     - None
 */
/****************************************************************/
static TTIDCSTATUS SetWordLength(const MCBSP_Handle hMcbsp,unsigned int uiLength)
{
    /* only change the word-length, if it is not already set    */
    if (MCBSP_FGETH(hMcbsp, RCR1, RWDLEN1) != uiLength)
    {
        /* if no, make sure the port is inactive                */
        while(MCBSP_FGETH(hMcbsp, SPCR2, XEMPTY) == 1);

        /* place receiver and transmitter in reset              */
        MCBSP_FSETH(hMcbsp, SPCR1, RRST,MCBSP_SPCR1_RRST_DISABLE);
        MCBSP_FSETH(hMcbsp, SPCR2, XRST,MCBSP_SPCR2_XRST_DISABLE);

        /* set the new word-length                              */
        MCBSP_FSETH(hMcbsp, RCR1, RWDLEN1, uiLength);
        MCBSP_FSETH(hMcbsp, XCR1, XWDLEN1, uiLength);

        /* re-enable the receiver and transmitter               */
        MCBSP_FSETH(hMcbsp, SPCR1, RRST, MCBSP_SPCR1_RRST_ENABLE);
        MCBSP_FSETH(hMcbsp, SPCR2, XRST, MCBSP_SPCR2_XRST_ENABLE);
    }

    return TIDC_NO_ERR;
}


/****************************************************************/
/* SubmitBlock()                                                */
/**Operation:
 *     - Initializes the DMA channels which have been opened and
 *       pre-configured by the ADS1299_initRcvDma() and
 *       ADS1299_initXmtDma() functions with the correct values
 *       for the DMA channel control registers, the transfer count
 *       registers.
 *     - Initializes the destination address register of the
 *       receive channel to match the buffer passed into the
 *       fucntion.
 *     - If the EN bit is set in the CCR register passed into the
 *       function, the DMA channel will be started.
 *
 * Parameters:
 *     - DMA_Handle hDmaRcv: Handle for the receive DMA channel
 *     - DMA_Handle hDmaXmt: Handle for the transit DMA channel
 *     - unsigned int uiCcrRcv: DMA control register value for the
 *              receive channel.
 *     - unsigned int uiCcrXmt: DMA control register value for the
 *              transmit channel
 *     - unsigned long ulCnt: Number of words to be transferred.
 *     - unsigned long ulDst: Address of the buffer to be filled.
 *     - unsigned int uiDr: Data rate of the converter
 *
 * Return value:
 *     - TIDC_NO_ERR in case everything was OK
 *
 * Globals modified:
 *     - None
 *
 * Resources used:
 *     - DMA channels opened by ADS1299_initRcvDma() and
 *              ADS1299_initXmtDma().
 */
/****************************************************************/
static TTIDCSTATUS SubmitBlock(const DMA_Handle hDmaRcv,
                               const DMA_Handle hDmaXmt,
                               unsigned int uiCcrRcv,
                               unsigned int uiCcrXmt,
                               unsigned long ulCnt,
                               unsigned long ulDst,
                               unsigned int uiDr)
{
    unsigned long cfn = 0;
    unsigned long cen = 0;
    unsigned int uiDstPort;

    DMA_FSETH(hDmaXmt, DMACCR, AUTOINIT, 1);

    /* the DMA controller needs byte addresses                  */
    ulDst = ulDst << 1;

    do {
        cfn++;
        cen = ulCnt / cfn;
        if (cfn > DIVIDER) {
            cfn = 0;
            cen = 0;
            return TIDC_ERR_BADARGS;
        }
    } while ((cen >= DIVIDER) || (ulCnt % cfn));

    /* decide, in which memory area the buffer is located       */
    uiDstPort = ADS1299_DMACSDP_EMIF_DST;
    if (ulDst <= DCP_DARAM_END)
        uiDstPort = ADS1299_DMACSDP_DARAM_DST;
    if (ulDst >= DCP_SARAM_START && ulDst <= DCP_SARAM_END)
        uiDstPort = ADS1299_DMACSDP_SARAM_DST;

    /* wait until the channel is ready to program               */
    while ((DMA_RGETH(hDmaRcv, DMACCR) & ADS1299_DMA_ENDPROG) != 0);

    /* set the DMA Channel Source & Destination Parameter reg.  */
    DMA_RSETH(hDmaRcv,
              DMACSDP,
              ADS1299_DMACSDP_INPUT_VALUE | uiDstPort);

    /* set the DMA Channel Element Number Register (DMACEN)     */
    DMA_RSETH(hDmaRcv, DMACEN, (unsigned int)cen);
    /* set the DMA Channel Frame Number Register (DMACEN)       */
    DMA_RSETH(hDmaRcv, DMACFN, (unsigned int)cfn);



    /* set the DMA Channel Destination Start Address register   */
    DMA_RSETH(hDmaRcv, DMACDSAL,  (unsigned int)ulDst);
    DMA_RSETH(hDmaRcv, DMACDSAU, ((unsigned int)(ulDst >> 16)));

    /* set the Dma Channel Control Register (DMACCR)            */
    DMA_RSETH(hDmaRcv, DMACCR, uiCcrRcv);
    DMA_FSETH(hDmaRcv, DMACCR, EN, 1);

    if (uiDr < 0x0002u)
    {
        if (a > 0)
            DMA_RSETH(hDmaXmt, DMACEN, 8);
        else
            DMA_RSETH(hDmaXmt, DMACEN, 7);
    }
    else
    {
        if (a > 0)
            DMA_RSETH(hDmaXmt, DMACEN, 9);
        else
            DMA_RSETH(hDmaXmt, DMACEN, 8);
    }

    DMA_RSETH(hDmaXmt, DMACCR, uiCcrXmt);

    return TIDC_NO_ERR;
}


/****************************************************************/
/* Wait()                                                       */
/** Operation:
 *      - Opens a DSP timer and configures it for the correct
 *        wait time on the first call. Otherwise, just reprograms
 *        the timer period register.
 *      - Waits the number of tosc periods specified in
 *        uiWaitCount
 *
 * Parameters:
 *      - TADS1299 *pADS:Data converter object
 *      - unsigned int uiWaitCount: Number of tosc, which should
 *        be waited
 *
 * Return value:
 *     - TIDC_NO_ERR in case everything was OK
 *     - TIDC_ERR_TIMER if the timer could not be opened
 *
 * Globals modified:
 *     - None
 *
 * Resources used:
 *     - One timer
 *
 * Note:
 *     - This routine is a blocking one. That means it waits for
 *       the timer to complete before it returns
 */
/****************************************************************/
static TTIDCSTATUS Wait(unsigned int uiWaitCount)
{

    static TIMER_Handle hTimer;
    static int iTimerConfigured = 0;
    static unsigned int uiTimerEventId;
    unsigned long ulTimerPeriod;

    /* setup the timer; is needed as the ADS1299 needs some     */
    /* delay between the commands. Configuration is only done   */
    /* at the very first time                                   */
    if (iTimerConfigured == 0 )
    {
        hTimer = TIMER_open(TIMER_DEV_ANY, TIMER_OPEN_RESET);
        if (hTimer == INV)
            return TIDC_ERR_TIMER;

        uiTimerEventId = TIMER_getEventId(hTimer);

        ulTimerPeriod = (unsigned long)97 //pADS->uiClockDivision
                        * (unsigned long)uiWaitCount + 1;

        TIMER_configArgs(hTimer,
            TIMER_FMK(TCR, IDLEEN, TIMER_TCR_IDLEEN_DISABLE)
          | TIMER_FMK(TCR, FUNC, TIMER_TCR_FUNC_OF(0))
          | TIMER_FMK(TCR, TLB, TIMER_TCR_TLB_RESET)
          | TIMER_FMK(TCR, SOFT, TIMER_TCR_SOFT_DEFAULT)
          | TIMER_FMK(TCR, FREE, TIMER_TCR_FREE_DEFAULT)
          | TIMER_FMK(TCR, PWID, TIMER_TCR_PWID_DEFAULT)
          | TIMER_FMK(TCR, ARB, TIMER_TCR_ARB_RESET)
          | TIMER_FMK(TCR, TSS, TIMER_TCR_TSS_STOP)
          | TIMER_FMK(TCR, CP, TIMER_TCR_CP_CLOCK)
          | TIMER_FMK(TCR, POLAR, TIMER_TCR_POLAR_DEFAULT)
          | TIMER_FMK(TCR, DATOUT, TIMER_TCR_DATOUT_DEFAULT),
          (unsigned int)ulTimerPeriod,
          0);

        iTimerConfigured = 1;
    }
    else
    {
        ulTimerPeriod = (unsigned long)20 //pADS->uiClockDivision
                        * (unsigned long)uiWaitCount + 1;

        TIMER_RSETH(hTimer, PRD, ulTimerPeriod);
        TIMER_RSETH(hTimer, TIM, 0);
    }

    /* start the timer and wait for the interrupt               */
    IRQ_clear(uiTimerEventId);
    TIMER_FSETH(hTimer, TCR, TSS, TIMER_TCR_TSS_START);
    while(!IRQ_test(uiTimerEventId)) ;
    TIMER_FSETH(hTimer, TCR, TSS, TIMER_TCR_TSS_STOP);

    return TIDC_NO_ERR;
}


/****************************************************************/
/* WriteRegister()                                              */
/** Operation:
 *      - Writes a new value to a register.
 *      - It always transfers (only) 24-bits.
 *
 * Parameters:
 *      - TADS1299 *pADS:Data converter object
 *      - unsigned int uiRegister: The address of the register to
 *        write.
 *      - unsigned long *ulValue: The register contents to be
 *        written.
 *
 * Return value:
 *     - TIDC_NO_ERR in case everything was OK
 *
 * Globals modified:
 *     - None
 *
 * Resources used:
 *     - None
 */
/****************************************************************/
static TTIDCSTATUS WriteRegister (const TADS1299 *pADS,
                                  unsigned int uiRegister,
                                  const unsigned int *uiValue)
{
    (void)SetWordLength(pADS->serial->hMcbsp, ADS1299_WORDLENGTH_24BIT);

    /* send the register write command to the converter */
    (void)SendCommand(pADS->serial->hMcbsp,
                      (unsigned long)ADS1299_CMD_WREG << 0x0008u
                      | (unsigned long)uiRegister << 0x0010u
                      | *uiValue);

    return TIDC_NO_ERR;
}


/****************************************************************/
/* END OF t1299_ob.c                                             */
/****************************************************************/

