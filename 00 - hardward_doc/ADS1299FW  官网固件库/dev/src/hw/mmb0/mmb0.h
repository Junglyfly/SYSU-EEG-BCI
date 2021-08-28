
#ifndef GUARD_mmb0_h
#define GUARD_mmb0_h

#include <tms320vc5507.h>
#include "types.h"

#define MMB0_CPUCLOCK_DEFAULT 144000000L
#define MMB0_CLKIN 12000L

/* this is fed into several files                              */
#define MMB0_US_PER_PRD_TICK 1000

// shadow regs
/* About the shadow registers:

We don't do RMW on the GPIO regs.  Instead we keep "shadow registers", do
RMW on those, then write to the actual registers.  The macro PIN_SETSH()
below does RMW on a shadow register.  Some other macros are provided which
call that and then copy to the real register. */
extern u16 pin_shadow_PCR0;
extern u16 pin_shadow_IODATA;
extern u16 pin_shadow_IODIR;
extern u8 pin_shadow_I2CCTL;
extern u8 pin_shadow_I2CGPIO;

// change shadow register bit
#define PIN_SETSH(REG,MASK,VAL) pin_shadow_ ## REG=(VAL)?((pin_shadow_ ## REG)|(MASK)):((pin_shadow_ ## REG)&(~(MASK)))
#define PIN_SETPCR0(MASK,VAL) PIN_SETSH(PCR0,MASK,VAL); PCR(0)=pin_shadow_PCR0
#define PIN_SETIODATA(MASK,VAL) PIN_SETSH(IODATA,MASK,VAL); IODATA=pin_shadow_IODATA
#define PIN_SETIODIR(MASK,VAL) PIN_SETSH(IODIR,MASK,VAL); IODIR=pin_shadow_IODIR

#define PIN_UPDATE_I2CGPIO() i2c_w8(I2C_BUS_MB,I2C_ADR_GPIO,pin_shadow_I2CGPIO)
#define PIN_UPDATE_I2CCTL() i2c_w8(I2C_BUS_MB,I2C_ADR_CTL,pin_shadow_I2CCTL)

#define PIN_CTL_SETCLR(SET,CLR) PIN_SETSH(I2CCTL,SET,1); PIN_SETSH(I2CCTL,CLR,0); PIN_UPDATE_I2CCTL()

typedef struct pin_state_t {
    unsigned int cntl:1,	// gpio6
        cntldir:1,
        fsxb:1,		// gpio7
        fsxbdir:1,
        romcs:1,	// gpio4
        dx0:1,		// for flash
        gpio0:1,	// fsx0
        gpio0dir:1,
        gpio1:1,	// clkr0
        gpio1dir:1,
        gpio2:1,	// fsr0
        gpio2dir:1,
        gpio3:1,	// clkx0
        gpio3dir:1;
} pin_state_t;

// U1 -- address I2C_ADR_GPIO
#define PIN_SWA	(1<<0)
#define PIN_SWB	(1<<1)
#define PIN_GPIO4	(1<<2)
#define PIN_GPIO5	(1<<3)
    // dir bits: 1 = out
#define PIN_GPIO4DIR	(1<<4)
#define PIN_GPIO5DIR	(1<<5)
#define PIN_DCEN	(1<<7)

// U8 -- address I2C_ADR_CTL
#define PIN_FSXON	(1<<0)
#define PIN_FSXBON	(1<<1)
#define PIN_CLKXON	(1<<2)
#define PIN_CLKCON	(1<<3)
#define PIN_CLKRON	(1<<4)
#define PIN_CLKAON	(1<<5)
#define PIN_TOUT0ON	(1<<6)
#define PIN_CLKBON	(1<<7)

// on the built-in gpio port
#define PIN_ROMCS (1<<4)
#define PIN_CNTL (1<<6)
#define PIN_FSXB (1<<7)

#define CTL_FSX_FSX	PIN_FSXON
#define CTL_FSX_FSXB	PIN_FSXBON
#define CTL_CLKX_CLKX	PIN_CLKXON
#define CTL_CLKX_CLKC	PIN_CLKCON
#define CTL_CLKR_CLKR	PIN_CLKRON
#define CTL_CLKR_CLKA	PIN_CLKAON
#define CTL_TOUT_TOUT0	PIN_TOUT0ON
#define CTL_TOUT_CLKB   PIN_CLKBON

// gpio control etc.
//void pin_set_gpio(u8 d);
//void pin_frob_gpio(u8 mask, u8 data);
//#define pin_set_gpio_bit(mask,on) pin_frob_gpio(mask,on?0xff:0)
#define pin_gpio() i2c_r8(I2C_BUS_MB,I2C_ADR_GPIO)

#define mmb0_activate_card() pin_shadow_I2CGPIO&=~PIN_DCEN; PIN_UPDATE_I2CGPIO()
#define mmb0_deactivate_card() pin_shadow_I2CGPIO|=PIN_DCEN; PIN_UPDATE_I2CGPIO()

void pin_set_ctl(u8 d);
void pin_frob_ctl(u8 mask, u8 data);
#define pin_set_ctl_bit(mask,on) pin_frob_ctl(mask,on?0xff:0)

// ---- pin funcs

#define pin_rd_GPIO0() ((PCR(0)&PCR_FSXP)!=0)
#define pin_get_GPIO0() pin_rd_GPIO0()
#define pin_set_GPIO0(VAL) PIN_SETPCR0(PCR_FSXP,VAL)
#define pin_dir_GPIO0(VAL) PIN_SETPCR0(PCR_FSXM,VAL)
#define pin_set_GPIO0_dir(VAL) pin_dir_GPIO0(VAL)
#define pin_get_GPIO0_dir() ((pin_shadow_PCR0 & PCR_FSXM)!=0)

#define pin_rd_GPIO1() ((PCR(0)&PCR_CLKRP)!=0)
#define pin_get_GPIO1() pin_rd_GPIO1()
#define pin_set_GPIO1(VAL) PIN_SETPCR0(PCR_CLKRP,VAL)
#define pin_dir_GPIO1(VAL) PIN_SETPCR0(PCR_CLKRM,VAL)
#define pin_set_GPIO1_dir(VAL) pin_dir_GPIO1(VAL)
#define pin_get_GPIO1_dir() ((pin_shadow_PCR0 & PCR_CLKRM)!=0)

#define pin_rd_GPIO2() ((PCR(0)&PCR_FSRP)!=0)
#define pin_get_GPIO2() pin_rd_GPIO2()
#define pin_set_GPIO2(VAL) PIN_SETPCR0(PCR_FSRP,VAL)
#define pin_dir_GPIO2(VAL) PIN_SETPCR0(PCR_FSRM,VAL)
#define pin_set_GPIO2_dir(VAL) pin_dir_GPIO2(VAL)
#define pin_get_GPIO2_dir() ((pin_shadow_PCR0 & PCR_FSRM)!=0)

#define pin_rd_GPIO3() ((PCR(0)&PCR_CLKXP)!=0)
#define pin_get_GPIO3() pin_rd_GPIO3()
#define pin_set_GPIO3(VAL) PIN_SETPCR0(PCR_CLKXP,VAL)
#define pin_dir_GPIO3(VAL) PIN_SETPCR0(PCR_CLKXM,VAL)
#define pin_set_GPIO3_dir(VAL) pin_dir_GPIO3(VAL)
#define pin_get_GPIO3_dir() ((pin_shadow_PCR0 & PCR_CLKXM)!=0)

#define pin_rd_GPIO4() ((pin_gpio()&PIN_GPIO4)!=0)
#define pin_get_GPIO4() pin_rd_GPIO4()
#define pin_set_GPIO4(d) PIN_SETSH(I2CGPIO,PIN_GPIO4,d); PIN_UPDATE_I2CGPIO()
#define pin_dir_GPIO4(VAL) pin_shadow_I2CGPIO=\
    (VAL)?((pin_shadow_I2CGPIO&~PIN_GPIO4)|PIN_GPIO4DIR):\
    (pin_shadow_I2CGPIO&~PIN_GPIO4DIR);\
    PIN_UPDATE_I2CGPIO()
#define pin_set_GPIO4_dir(VAL) pin_dir_GPIO4(VAL)
#define pin_get_GPIO4_dir() ((pin_shadow_I2CGPIO & PIN_GPIO4DIR) != 0)

#define pin_rd_GPIO5() ((pin_gpio()&PIN_GPIO5)!=0)
#define pin_get_GPIO5() pin_rd_GPIO5()
#define pin_set_GPIO5(d) PIN_SETSH(I2CGPIO,PIN_GPIO5,d); PIN_UPDATE_I2CGPIO()
#define pin_dir_GPIO5(VAL) pin_shadow_I2CGPIO=\
    (VAL)?((pin_shadow_I2CGPIO&~PIN_GPIO5)|PIN_GPIO5DIR):\
    (pin_shadow_I2CGPIO&~PIN_GPIO5DIR);\
    PIN_UPDATE_I2CGPIO()
#define pin_set_GPIO5_dir(VAL) pin_dir_GPIO5(VAL)
#define pin_get_GPIO5_dir() ((pin_shadow_I2CGPIO & PIN_GPIO5DIR) != 0)

#define pin_rd_CNTL() ((IODATA&PIN_CNTL)!=0)
#define pin_CNTL() pin_rd_CNTL()
#define pin_set_CNTL(VAL) PIN_SETIODATA(PIN_CNTL,VAL)
#define pin_dir_CNTL(VAL) PIN_SETIODIR(PIN_CNTL,VAL)
#define pin_set_CNTL_dir(VAL) pin_dir_CNTL(VAL)

#define pin_rd_FSX() ((IODATA&PIN_FSXB)!=0)
#define pin_FSX() pin_rd_FSX()
#define pin_FSXB() pin_rd_FSX()
#define pin_set_FSX(VAL) PIN_SETIODATA(PIN_FSXB,VAL)
#define pin_set_FSXB(VAL) pin_set_FSX(VAL)
#define pin_dir_FSX(VAL) PIN_SETIODIR(PIN_FSXB,VAL)
#define pin_set_FSX_dir(VAL) pin_dir_FSX(VAL)
#define pin_set_FSXB_dir(VAL) pin_dir_FSX(VAL)

#define pin_set_ROMCS(VAL) PIN_SETIODATA(PIN_ROMCS,VAL)

// --- signal routing

//! Connect MCBSP1 CLKX to daughtercard CLKX
#define connect_DSPCLKX_DCCLKX() PIN_CTL_SETCLR(PIN_CLKCON,PIN_CLKXON)
//! Connect PLL CLKC to MCBSP1 CLKX
#define connect_DSPCLKX_PLLCLKC() PIN_CTL_SETCLR(PIN_CLKXON,PIN_CLKCON)
//! Connect MCBSP1 CLKR to daughtercard CLKR
#define connect_DSPCLKR_DCCLKR() PIN_CTL_SETCLR(PIN_CLKAON,PIN_CLKRON)
//! Connect PLL CLKA to MCBSP1 CLKR
#define connect_DSPCLKR_PLLCLKA() PIN_CTL_SETCLR(PIN_CLKRON,PIN_CLKAON)
//! Connect MCBSP1 FSX to daughtercard FSX
#define connect_DSPFSX_DCFSX() PIN_CTL_SETCLR(PIN_FSXBON,PIN_FSXON)
//! Connect GPIO FSX to daughtercard FSX
#define connect_GPIOFSX_DCFSX() PIN_CTL_SETCLR(PIN_FSXON,PIN_FSXBON)
//! Connect DSP TOUT to daughtercard TOUT
#define connect_DSPTOUT_DCTOUT() PIN_CTL_SETCLR(PIN_CLKBON,PIN_TOUT0ON)
//! Connect PLL CLKB to daughtercard TOUT
#define connect_PLLCLKB_DCTOUT() PIN_CTL_SETCLR(PIN_TOUT0ON,PIN_CLKBON)
//! Disconnect PLL CLKB and DSP TOUT from daughtercard TOUT
#define disconnect_DCTOUT() PIN_SETSH(I2CCTL,PIN_CLKBON,1); PIN_SETSH(I2CCTL,PIN_TOUT0ON,1); PIN_UPDATE_I2CCTL()

//! Initialise the MMB0
/*! Initialises the MMB0. */
void mmb0_init(void);

/* prototypes for the exposed functions                         */
void mmb0_preinit(void);
void usbstyx_main(void);
u32 mmb0_cpuclk_set(u32 freq);
u32 mmb0_cpuclk_get(void);
u32 mmb0_clkout_get(void);
void mmb0_pll_set(u32 settings);
u32 mmb0_pll_get(void);

#endif
