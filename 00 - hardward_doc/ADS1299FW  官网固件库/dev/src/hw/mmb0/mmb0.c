
#include "mmb0.h"
#include "mmb0i2c.h"
#include <std.h>
#include <mmb0ui.h>
#include <mmb0clk.h>

u16 pin_shadow_PCR0;
u16 pin_shadow_IODATA;
u16 pin_shadow_IODIR;
u8 pin_shadow_I2CCTL;
u8 pin_shadow_I2CGPIO;

//static volatile pin_state_t iostate2;
static u32 mmb0_cpu_freq = 0;

extern void clk_init(void);

static void init_gpio(void)
{
    SPCR1(0)=0;
    SPCR2(0)=0;
    pin_shadow_IODATA=PIN_ROMCS;
    IODATA=pin_shadow_IODATA;
    pin_shadow_IODIR=PIN_ROMCS;
    IODIR=pin_shadow_IODIR;
    pin_shadow_PCR0=PCR_XIOEN|PCR_RIOEN|PCR_FSXP|PCR_FSXM|PCR_CLKRP;
    PCR(0)=pin_shadow_PCR0;
    pin_shadow_I2CGPIO=PIN_SWA|PIN_SWB|PIN_GPIO4|PIN_GPIO4DIR|PIN_GPIO5|PIN_GPIO5DIR|PIN_DCEN;
    PIN_UPDATE_I2CGPIO();
    pin_shadow_I2CCTL=0xFF;
    PIN_UPDATE_I2CCTL();
}

#if 0
static void pin_update_mcbsp2_io(void)
{
    u8 pcr;

    pcr=PCR_XIOEN|PCR_RIOEN;
    pcr|=iostate2.gpio3?PCR_CLKXP:0;
    pcr|=iostate2.gpio3dir?PCR_CLKXM:0;
    pcr|=iostate2.gpio0?PCR_FSXP:0;
    pcr|=iostate2.gpio0dir?PCR_FSXM:0;
    pcr|=iostate2.dx0?PCR_DXSTAT:0;
    pcr|=iostate2.gpio1?PCR_CLKRP:0;
    pcr|=iostate2.gpio1dir?PCR_CLKRM:0;
    pcr|=iostate2.gpio2?PCR_FSRP:0;
    pcr|=iostate2.gpio2dir?PCR_FSRM:0;
    PCR(2)=pcr;
}

static void init_mcbsp2_io(void)
{
    SPCR1(2)=0;
    SPCR2(2)=0;
    iostate2.cntldir=1;
    iostate2.cntl=1;
    iostate2.fsxbdir=1;
    iostate2.fsxb=1;
    iostate2.romcs=1;
    iostate2.gpio0dir=1;
    iostate2.gpio0=1;
    iostate2.gpio1dir=1;
    iostate2.gpio1=1;
    iostate2.gpio2dir=1;
    iostate2.gpio3dir=1;
    pin_update_mcbsp2_io();
}

void pin_set_ctl(u8 d)
{
    i2c_lock();
    current_ctl=d;
    i2c_w8(I2C_BUS_MB,I2C_ADR_CTL,~d);
    i2c_unlock();
}

void pin_frob_ctl(u8 mask, u8 data)
{
    pin_set_ctl((mask&data)|((~mask)&current_ctl));
}
#endif

// freq in Hz
u32 mmb0_cpuclk_set(u32 freq)
{
    int d, prev_d;
    u32 m, prev_m;
    u32 near_freq = 0;
    u32 prev_near_freq = 0;
    volatile u16 clkmd;


    for (d = 4; d > 0; d--) {
        for (m = 31; m > 0; m--) {
            if(12000000L * m / d <= freq) {
                near_freq = m * MMB0_CLKIN * 1000L / d;
                if((freq-near_freq) < (freq-prev_near_freq)) {
                    prev_m = m;
                    prev_d = d;
                    prev_near_freq = near_freq;
                    break;
                }
            }
        }
    }

    clkmd=0x2010|(prev_m<<7)|((prev_d-1)<<5);
    CLKMD=clkmd;
    mmb0_cpu_freq = prev_near_freq;
    //GBL_setFrequency(12000*(m/i));
    //CLK_reconfig();

   return(mmb0_cpu_freq);
}


u32 mmb0_cpuclk_get(void)
{
    return mmb0_cpu_freq;
}

/* return the frequency of the clockout - pin                   */
u32 mmb0_clkout_get(void)
{
    return mmb0_cpuclk_get() / (long)(IOREG(0x7FD) << 1);;
}

static void emif_init(void)
{
    IOREG(0x6c00)=1;
    EMIF_EGCR=0x220;
    EMIF_CE01=0x3FFF;
    EMIF_CE02=0x5FFF;
    EMIF_CE03=0;
    EMIF_CE11=0x3FFF;
    EMIF_CE12=0x5FFF;
    EMIF_CE13=0;
    EMIF_CE21=0x3FFF;
    EMIF_CE22=0x5FFF;
    EMIF_CE23=0;
    EMIF_CE31=0x3FFF;
    EMIF_CE32=0x5FFF;
    EMIF_CE33=0;
    EMIF_SDC1=0x3522;
    EMIF_SDC2=0x0242;
    EMIF_SDC3=0x0003;
    EMIF_SDPER=0x0100;
    EMIF_EMIRST=0;
    EMIF_INIT=0;
}

void mmb0_preinit(void)
{
    // system register; see '5507 PDS
    IOREG(0x7FD)=6; // divide clock by 12
    /* if there was an user initialization, skip this step */
    if (mmb0_cpuclk_get() == 0)
        mmb0_cpuclk_set(MMB0_CPUCLOCK_DEFAULT);
    //emif_init();
}

void mmb0_init(void)
{
    i2c_init();
    init_gpio();
    clk_init();
    ui_init();
    mmb0_deactivate_card();
}

#ifdef DEFAULT_INT
// daughtercard INT pin
void isr_INT(Arg a)
{
}
#endif

#ifdef DEFAULT_EINT
// external interrupt (pin on ext i2c connector)
void isr_EINT(Arg a)
{
}
#endif

// power interrupt
void isr_PWRINT(Arg a)
{
}
