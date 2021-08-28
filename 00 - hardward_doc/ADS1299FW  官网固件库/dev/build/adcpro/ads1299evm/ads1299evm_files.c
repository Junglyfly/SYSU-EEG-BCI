#include <std.h>
#include <acquire.h>
#include "ads1299evm.h"
#include "mmb0.h"
#include "utilfiles.h"
#include "t1299_fn.h"
#include "mmb0i2c.h"
#include "mmb0eeprom.h"
#include "mmb0clk.h"
#include "version.h"

#include <csl_gpio.h> /*Add*/

extern void mmb0_clkfreq_set(u32 f);

static unsigned long reset=0;
static int sleep = 0;

/****************************************************************/
/* offset calibration was removed                               */
/* static int offcal = 0;                                       */
/****************************************************************/

#define CONFIG1_MASK                (0x00000008u)
#define CONFIG1_SETMASK				(0x00000090u)
#define CONFIG2_MASK                (0x00000028u)
#define CONFIG2_SETMASK				(0x000000C0u)
#define CONFIG3_SETMASK             (0x00000060u)
#define LOFF_MASK					(0x00000010u)
#define MISC1_MASK                  (0x000000DFu)
#define MISC2_MASK                  (0x000000FFu)
#define CONFIG4_MASK                (0x000000F5u)


/****************************************************************/
/* offset calibration was removed
 * void ads1299evm_offcal_set(int i)
 * {
 *    if (i != offcal) {
 *        offcal = i;
 *        dc_control(&Ads1299_1,ADS1299_CMD_OFFCAL,0);
 *    }
 *    offcal = 0;
 * }
 *
 * int ads1299evm_offcal_get(void)
 * {
 *    return offcal;
 * }
*/
/****************************************************************/

void ads1299evm_sleep_set(int i)
{
    if (i != sleep) {
        sleep = i;
        if (sleep == 0) {
            (void)dc_control(&Ads1299_1,ADS1299_CMD_WAKEUP,0);
        }
        if (sleep == 1) {
            (void)dc_control(&Ads1299_1,ADS1299_CMD_STANDBY,0);
        }
    }
}

int ads1299evm_sleep_get(void)
{
    return sleep;
}

u32 ads1299evm_reset_get(void)
{
    return reset;
}

void ads1299evm_reset_set(u32 i)
{
    if (i == 1) {
        reset = i;
        (void)dc_control(&Ads1299_1,ADS1299_CMD_RESET,0);
    }
    if (i == 2) {
        reset = i;
        (void)dc_control(&Ads1299_1,ADS1299_CMD_INITDEVICE,0);
    }
    
    reset = 0;
}


u32 ads1299evm_devid_get(void)
{
    unsigned int val = ADS1299_REG_DEVID;
    (void)dc_control(&Ads1299_1,ADS1299_CMD_RREG,&val);
    return (unsigned long)val;
}

/****************************************************************/
/* devid is a read only register
 * void ads1299evm_devid_set(u32 i)
 * {
 *     unsigned long val = i;
 *     unsigned int reg = ADS1299_REG_DEVID;
 *     Ads1299_1.regs.devid.value = val;
 *     dc_control(&Ads1299_1,ADS1299_CMD_WREG,&reg);
 * }
*/
/****************************************************************/

u32 ads1299evm_config1_get(void)
{
    unsigned int val = ADS1299_REG_CONFIG1;
    (void)dc_control(&Ads1299_1,ADS1299_CMD_RREG,&val);
    return (unsigned long)val;
}

void ads1299evm_config1_set(u32 i)
{
    unsigned long val = (i | CONFIG1_SETMASK) & ~CONFIG1_MASK;
    unsigned int reg = ADS1299_REG_CONFIG1;
    Ads1299_1.regs.config1.value = val;
    (void)dc_control(&Ads1299_1,ADS1299_CMD_WREG,&reg);
}


u32 ads1299evm_config2_get(void)
{
    unsigned int val = ADS1299_REG_CONFIG2;
    (void)dc_control(&Ads1299_1,ADS1299_CMD_RREG,&val);
    return (unsigned long)val;
}

void ads1299evm_config2_set(u32 i)
{
    unsigned long val = (i | CONFIG2_SETMASK) & ~CONFIG2_MASK;
    unsigned int reg = ADS1299_REG_CONFIG2;
    Ads1299_1.regs.config2.value = val;
    (void)dc_control(&Ads1299_1,ADS1299_CMD_WREG,&reg);
}

u32 ads1299evm_config3_get(void)
{
    unsigned int val = ADS1299_REG_CONFIG3;
    (void)dc_control(&Ads1299_1,ADS1299_CMD_RREG,&val);
    return (unsigned long)val;
}

void ads1299evm_config3_set(u32 i)
{
    unsigned long val = i | CONFIG3_SETMASK;
    unsigned int reg = ADS1299_REG_CONFIG3;
    Ads1299_1.regs.config3.value = val;
    (void)dc_control(&Ads1299_1,ADS1299_CMD_WREG,&reg);
}

u32 ads1299evm_loff_get(void)
{
    unsigned int val = ADS1299_REG_LOFF;
    (void)dc_control(&Ads1299_1,ADS1299_CMD_RREG,&val);
    return (unsigned long)val;
}

void ads1299evm_loff_set(u32 i)
{
    unsigned long val = i  & ~LOFF_MASK;
    unsigned int reg = ADS1299_REG_LOFF;
    Ads1299_1.regs.loff.value = val;
    (void)dc_control(&Ads1299_1,ADS1299_CMD_WREG,&reg);
}

u32 ads1299evm_ch1set_get(void)
{
    unsigned int val = ADS1299_REG_CH1SET;
    (void)dc_control(&Ads1299_1,ADS1299_CMD_RREG,&val);
    return (unsigned long) val;
}

void ads1299evm_ch1set_set(u32 i)
{
    unsigned long val = i;
    unsigned int reg = ADS1299_REG_CH1SET;
    Ads1299_1.regs.ch1set.value = val;
    (void)dc_control(&Ads1299_1,ADS1299_CMD_WREG,&reg);
}

u32 ads1299evm_ch2set_get(void)
{
    unsigned int val = ADS1299_REG_CH2SET;
    (void)dc_control(&Ads1299_1,ADS1299_CMD_RREG,&val);
    return (unsigned long) val;
}

void ads1299evm_ch2set_set(u32 i)
{
    unsigned long val = i;
    unsigned int reg = ADS1299_REG_CH2SET;
    Ads1299_1.regs.ch2set.value = val;
    (void)dc_control(&Ads1299_1,ADS1299_CMD_WREG,&reg);
}

u32 ads1299evm_ch3set_get(void)
{
    unsigned int val = ADS1299_REG_CH3SET;
    (void)dc_control(&Ads1299_1,ADS1299_CMD_RREG,&val);
    return (unsigned long) val;
}

void ads1299evm_ch3set_set(u32 i)
{
    unsigned long val = i;
    unsigned int reg = ADS1299_REG_CH3SET;
    Ads1299_1.regs.ch3set.value = val;
    (void)dc_control(&Ads1299_1,ADS1299_CMD_WREG,&reg);
}

u32 ads1299evm_ch4set_get(void)
{
    unsigned int val = ADS1299_REG_CH4SET;
    (void)dc_control(&Ads1299_1,ADS1299_CMD_RREG,&val);
    return (unsigned long) val;
}

void ads1299evm_ch4set_set(u32 i)
{
    unsigned long val = i;
    unsigned int reg = ADS1299_REG_CH4SET;
    Ads1299_1.regs.ch4set.value = val;
    (void)dc_control(&Ads1299_1,ADS1299_CMD_WREG,&reg);
}

u32 ads1299evm_ch5set_get(void)
{
    unsigned int val = ADS1299_REG_CH5SET;
    (void)dc_control(&Ads1299_1,ADS1299_CMD_RREG,&val);
    return (unsigned long) val;
}

void ads1299evm_ch5set_set(u32 i)
{
    unsigned long val = i;
    unsigned int reg = ADS1299_REG_CH5SET;
    Ads1299_1.regs.ch5set.value = val;
    (void)dc_control(&Ads1299_1,ADS1299_CMD_WREG,&reg);
}

u32 ads1299evm_ch6set_get(void)
{
    unsigned int val = ADS1299_REG_CH6SET;
    (void)dc_control(&Ads1299_1,ADS1299_CMD_RREG,&val);
    return (unsigned long) val;
}

void ads1299evm_ch6set_set(u32 i)
{
    unsigned long val = i;
    unsigned int reg = ADS1299_REG_CH6SET;
    Ads1299_1.regs.ch6set.value = val;
    (void)dc_control(&Ads1299_1,ADS1299_CMD_WREG,&reg);
}

u32 ads1299evm_ch7set_get(void)
{
    unsigned int val = ADS1299_REG_CH7SET;
    (void)dc_control(&Ads1299_1,ADS1299_CMD_RREG,&val);
    return (unsigned long) val;
}

void ads1299evm_ch7set_set(u32 i)
{
    unsigned long val = i;
    unsigned int reg = ADS1299_REG_CH7SET;
    Ads1299_1.regs.ch7set.value = val;
    (void)dc_control(&Ads1299_1,ADS1299_CMD_WREG,&reg);
}

u32 ads1299evm_ch8set_get(void)
{
    unsigned int val = ADS1299_REG_CH8SET;
    (void)dc_control(&Ads1299_1,ADS1299_CMD_RREG,&val);
    return (unsigned long) val;
}

void ads1299evm_ch8set_set(u32 i)
{
    unsigned long val = i;
    unsigned int reg = ADS1299_REG_CH8SET;
    Ads1299_1.regs.ch8set.value = val;
    (void)dc_control(&Ads1299_1,ADS1299_CMD_WREG,&reg);
}

u32 ads1299evm_biassensp_get(void)
{
    unsigned int val = ADS1299_REG_BIASSENSP;
    (void)dc_control(&Ads1299_1,ADS1299_CMD_RREG,&val);
    return (unsigned long) val;
}

void ads1299evm_biassensp_set(u32 i)
{
    unsigned long val = i;
    unsigned int reg = ADS1299_REG_BIASSENSP;
    Ads1299_1.regs.biassensp.value = val;
    (void)dc_control(&Ads1299_1,ADS1299_CMD_WREG,&reg);
}

u32 ads1299evm_biassensn_get(void)
{
    unsigned int val = ADS1299_REG_BIASSENSN;
    (void)dc_control(&Ads1299_1,ADS1299_CMD_RREG,&val);
    return (unsigned long) val;
}

void ads1299evm_biassensn_set(u32 i)
{
    unsigned long val = i;
    unsigned int reg = ADS1299_REG_BIASSENSN;
    Ads1299_1.regs.biassensn.value = val;
    (void)dc_control(&Ads1299_1,ADS1299_CMD_WREG,&reg);
}

u32 ads1299evm_loffsensp_get(void)
{
    unsigned int val = ADS1299_REG_LOFFSENSP;
    (void)dc_control(&Ads1299_1,ADS1299_CMD_RREG,&val);
    return (unsigned long) val;
}

void ads1299evm_loffsensp_set(u32 i)
{
    unsigned long val = i;
    unsigned int reg = ADS1299_REG_LOFFSENSP;
    Ads1299_1.regs.loffsensp.value = val;
    (void)dc_control(&Ads1299_1,ADS1299_CMD_WREG,&reg);
}

u32 ads1299evm_loffsensn_get(void)
{
    unsigned int val = ADS1299_REG_LOFFSENSN;
    (void)dc_control(&Ads1299_1,ADS1299_CMD_RREG,&val);
    return (unsigned long) val;
}

void ads1299evm_loffsensn_set(u32 i)
{
    unsigned long val = i;
    unsigned int reg = ADS1299_REG_LOFFSENSN;
    Ads1299_1.regs.loffsensn.value = val;
    (void)dc_control(&Ads1299_1,ADS1299_CMD_WREG,&reg);
}

u32 ads1299evm_loffflip_get(void)
{
    unsigned int val = ADS1299_REG_LOFFFLIP;
    (void)dc_control(&Ads1299_1,ADS1299_CMD_RREG,&val);
    return (unsigned long) val;
}

void ads1299evm_loffflip_set(u32 i)
{
    unsigned long val = i;
    unsigned int reg = ADS1299_REG_LOFFFLIP;
    Ads1299_1.regs.loffflip.value = val;
    (void)dc_control(&Ads1299_1,ADS1299_CMD_WREG,&reg);
}

u32 ads1299evm_loffstatp_get(void)
{
    unsigned int val = ADS1299_REG_LOFFSTATP;
    (void)dc_control(&Ads1299_1,ADS1299_CMD_RREG,&val);
    return (unsigned long) val;
}

u32 ads1299evm_loffstatn_get(void)
{
    unsigned int val = ADS1299_REG_LOFFSTATN;
    (void)dc_control(&Ads1299_1,ADS1299_CMD_RREG,&val);
    return (unsigned long) val;
}

u32 ads1299evm_gpio_get(void)
{
    unsigned int val = ADS1299_REG_GPIO;
    (void)dc_control(&Ads1299_1,ADS1299_CMD_RREG,&val);
    return (unsigned long) val;
}

void ads1299evm_gpio_set(u32 i)
{
    unsigned long val = i;
    unsigned int reg = ADS1299_REG_GPIO;
    Ads1299_1.regs.gpio.value = val;
    (void)dc_control(&Ads1299_1,ADS1299_CMD_WREG,&reg);
}

u32 ads1299evm_misc1_get(void)
{
    unsigned int val = ADS1299_REG_MISC1;
    (void)dc_control(&Ads1299_1,ADS1299_CMD_RREG,&val);
    return (unsigned long) val;
}

void ads1299evm_misc1_set(u32 i)
{
    unsigned long val = i & ~MISC1_MASK;
    unsigned int reg = ADS1299_REG_MISC1;
    Ads1299_1.regs.misc1.value = val;
    (void)dc_control(&Ads1299_1,ADS1299_CMD_WREG,&reg);
}

u32 ads1299evm_misc2_get(void)
{
   unsigned int val = ADS1299_REG_MISC2;
   (void)dc_control(&Ads1299_1,ADS1299_CMD_RREG,&val);
   return (unsigned long) val;
}

void ads1299evm_misc2_set(u32 i)
{
   unsigned long val = i  & ~MISC2_MASK;
   unsigned int reg = ADS1299_REG_MISC2;
   Ads1299_1.regs.misc2.value = val;
   dc_control(&Ads1299_1,ADS1299_CMD_WREG,&reg);
}

u32 ads1299evm_config4_get(void)
{
    unsigned int val = ADS1299_REG_CONFIG4;
    (void)dc_control(&Ads1299_1,ADS1299_CMD_RREG,&val);
    return (unsigned long) val;
}

void ads1299evm_config4_set(u32 i)
{
    unsigned long val = i & ~CONFIG4_MASK;
    unsigned int reg = ADS1299_REG_CONFIG4;
    Ads1299_1.regs.config4.value = val;
    (void)dc_control(&Ads1299_1,ADS1299_CMD_WREG,&reg);
}

int ads1299evm_mkdir(estyx_server_t *srv, estyx_file_t *parent)
{
    estyx_file_t *evm, *conf;
    int iStatus;

    /* make all GPIO's input to avoid conflict with the I2C     */
    /* all features of the device are controlled by the I2C and */
    /* not by the GPIO pins                                     */
    /* GPIO2 is connected to the converters RESET pin           */
    /* Make it an output and set it to high and toggle it later */
    pin_dir_GPIO0(1);
    pin_set_GPIO0(0);
    pin_dir_GPIO2(1);
    pin_set_GPIO2(1);
    pin_dir_GPIO3(0);
    pin_dir_GPIO4(0);




    /* set the reset pin low. It will be high again after all the
       configuration is done. This make sure that there is no
       glitch on the SPI                                        */
    pin_set_GPIO2(0);
    connect_DSPCLKX_DCCLKX();
    connect_DSPFSX_DCFSX();

	/* added for fix of EVM  GH 08JUL10 */
	GPIO_pinDirection(GPIO_PIN6, 1);

    mmb0_clkfreq_set((unsigned long)12e6);
    mmb0_activate_card();


    /* create file-system for the EVM files                     */
    /* create the root files and directories                    */
    if (!(evm=estyx_mkdir(srv,parent,"ads1299evm"))) return -1;
    if (!(conf=estyx_mkdir(srv,evm,"conf"))) return -1;
    if (!estyx_file_msg_make(srv,0,"id",FW_ID)) return -1;
    if (!estyx_file_msg_make(srv,0,"version",FW_VERSION)) return -1;
    // if (!(eprom=estyx_mkdir(srv,evm,"eprom"))) return -1;

    /* create the support files for the converter configuration */
    if (!estyx_file_u32_make(srv,conf,"config4",ads1299evm_config4_get,ads1299evm_config4_set,2)) return -1;
    if (!estyx_file_bool_make(srv,conf,"sleep",ads1299evm_sleep_get,ads1299evm_sleep_set)) return -1;
    if (!estyx_file_u32_make(srv,conf,"reset",ads1299evm_reset_get,ads1299evm_reset_set,0)) return -1;
    if (!estyx_file_u32_make(srv,conf,"biassensp",ads1299evm_biassensp_get,ads1299evm_biassensp_set,2)) return -1;
    if (!estyx_file_u32_make(srv,conf,"biassensn",ads1299evm_biassensn_get,ads1299evm_biassensn_set,2)) return -1;
    if (!estyx_file_u32_make(srv,conf,"misc1",ads1299evm_misc1_get,ads1299evm_misc1_set,2)) return -1;
    if (!estyx_file_u32_make(srv,conf,"misc2",ads1299evm_misc2_get,ads1299evm_misc2_set,2)) return -1;
    if (!estyx_file_u32_make(srv,conf,"loffstatp",ads1299evm_loffstatp_get,0,2)) return -1;
    if (!estyx_file_u32_make(srv,conf,"loffstatn",ads1299evm_loffstatn_get,0,2)) return -1;
    if (!estyx_file_u32_make(srv,conf,"loffsensp",ads1299evm_loffsensp_get,ads1299evm_loffsensp_set,2)) return -1;
    if (!estyx_file_u32_make(srv,conf,"loffsensn",ads1299evm_loffsensn_get,ads1299evm_loffsensn_set,2)) return -1;
    if (!estyx_file_u32_make(srv,conf,"loffflip",ads1299evm_loffflip_get,ads1299evm_loffflip_set,2)) return -1;
    if (!estyx_file_u32_make(srv,conf,"loff",ads1299evm_loff_get,ads1299evm_loff_set,2)) return -1;
    if (!estyx_file_u32_make(srv,conf,"gpio",ads1299evm_gpio_get,ads1299evm_gpio_set,2)) return -1;
    if (!estyx_file_u32_make(srv,conf,"devid",ads1299evm_devid_get,0,2)) return -1;
    if (!estyx_file_u32_make(srv,conf,"config3",ads1299evm_config3_get,ads1299evm_config3_set,2)) return -1;
    if (!estyx_file_u32_make(srv,conf,"config2",ads1299evm_config2_get,ads1299evm_config2_set,2)) return -1;
    if (!estyx_file_u32_make(srv,conf,"config1",ads1299evm_config1_get,ads1299evm_config1_set,2)) return -1;
    if (!estyx_file_u32_make(srv,conf,"ch8set",ads1299evm_ch8set_get,ads1299evm_ch8set_set,2)) return -1;
    if (!estyx_file_u32_make(srv,conf,"ch7set",ads1299evm_ch7set_get,ads1299evm_ch7set_set,2)) return -1;
    if (!estyx_file_u32_make(srv,conf,"ch6set",ads1299evm_ch6set_get,ads1299evm_ch6set_set,2)) return -1;
    if (!estyx_file_u32_make(srv,conf,"ch5set",ads1299evm_ch5set_get,ads1299evm_ch5set_set,2)) return -1;
    if (!estyx_file_u32_make(srv,conf,"ch4set",ads1299evm_ch4set_get,ads1299evm_ch4set_set,2)) return -1;
    if (!estyx_file_u32_make(srv,conf,"ch3set",ads1299evm_ch3set_get,ads1299evm_ch3set_set,2)) return -1;
    if (!estyx_file_u32_make(srv,conf,"ch2set",ads1299evm_ch2set_get,ads1299evm_ch2set_set,2)) return -1;
    if (!estyx_file_u32_make(srv,conf,"ch1set",ads1299evm_ch1set_get,ads1299evm_ch1set_set,2)) return -1;

/* added for fix of EVM  GH 08JUL10 */
	AdsCSPin(1);
	AdsCSPin(0);

    /* create the files for the EEPROM                          */
    //  (void)eeprom_mkdir(srv, eprom);

    /* create the files for the data acquisition                */
    if (acquire_init(srv,evm)<0) return -1;

    iStatus = dc_configure(&Ads1299_1);

    return iStatus;
}
