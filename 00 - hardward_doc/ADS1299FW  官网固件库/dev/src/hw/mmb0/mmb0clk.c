#define _TI_ENHANCED_MATH_H 1
#include <math.h>

#include "mmb0.h"
#include "mmb0clk.h"
#include "mmb0i2c.h"

#define FREQ_THRESVCO (333000000.0)
#define FREQ_MAXVCO   (400000000.0)
#define MAX_PPM       (100)
#define FREQ_PHASEDET (250000.0)

typedef struct clk_pll_config_t {
    u8 q;
    u16 p;
    unsigned int lf:3, po:1, en:1;
} clk_pll_config_t;

typedef struct clk_config_t {
    u8 clkdiv[6]; // a0 a1 b0 b1 c d
    u8 fs[4]; // a b c d
    unsigned int acadj_cx:2,
        acadj_abde:2,
        pdnen:1,
        xbufoe:1,
        div_clke:2,
        dcadj_x:2,
        dcadj_de:2,
        dcadj_c:2,
        dcadj_ab:2;
    unsigned int osc_cap:6,
        osc_drv:2;
    /* [RO]: removed array for the frequency table of PLL1, as  */
    /* the index is hardwired on the MMB0 (it is 111b)          */
    clk_pll_config_t pll1;
    clk_pll_config_t pll2, pll3;
} clk_config_t;

clk_config_t conf;

#define bits(src,mask,lsb,elsb) (((src&mask)>>lsb)<<elsb)

#define mkpll0(pll) (pll->q&0xff)
#define mkpll1(pll) (pll->p&0xff)
#define mkpll2(pll) ((pll->en<<6)|(pll->lf<<3)|(pll->po<<2)|((pll->p>>8)&3))

#define mk08(c) (((c->fs[0]&1)<<7)|(c->clkdiv[0]&0x7f))
#define mk09(c) (((c->fs[0]&1)<<7)|(c->clkdiv[1]&0x7f))
#define mk0A(c) (((c->fs[1]&1)<<7)|(c->clkdiv[2]&0x7f))
#define mk0B(c) (((c->fs[1]&1)<<7)|(c->clkdiv[3]&0x7f))
#define mk0C(c) (((c->fs[2]&1)<<7)|(c->clkdiv[4]&0x7f))
#define mk0D(c) (((c->fs[3]&1)<<7)|(c->clkdiv[5]&0x7f))
#define mk0E(c) (bits(c->fs[3],6,1,6)|bits(c->fs[2],6,1,4)|bits(c->fs[1],6,1,2)|bits(c->fs[0],6,1,0))
#define mk0F(c) ((c->acadj_cx<<6)|(c->acadj_abde<<4)|(c->pdnen<<3)|(c->xbufoe<<2)|(c->div_clke&3))
#define mk10(c) ((c->dcadj_x<<6)|(c->dcadj_de<<4)|(c->dcadj_c<<2)|(c->dcadj_ab))
#define mk17(c) ((c->osc_cap<<2)|(c->osc_drv))

static u8 mkPLLByte(clk_pll_config_t *pll, u8 ofs)
{
    switch (ofs) {
    case 0:
        return mkpll0(pll);
    case 1:
        return mkpll1(pll);
    case 2:
        return mkpll2(pll);
    default:
        break;
    }
    return 0;
}

static u8 mkRegByte(clk_config_t *conf, u8 adr)
{
    if ((adr>=0x11)&&(adr<=0x13))
        return mkPLLByte(&(conf->pll2),adr-0x11);
    if ((adr>=0x14)&&(adr<=0x16))
        return mkPLLByte(&(conf->pll3),adr-0x14);
/* [RO]: Changed the addresses to match the H/W on MMB0         */
    if ((adr>=0x55)&&(adr<=0x57))
        return mkPLLByte(&(conf->pll1),adr-0x55);
    switch (adr) {
    case 0x08:
        return mk08(conf);
    case 0x09:
        return mk09(conf);
    case 0x0A:
        return mk0A(conf);
    case 0x0B:
        return mk0B(conf);
    case 0x0C:
        return mk0C(conf);
    case 0x0D:
        return mk0D(conf);
    case 0x0E:
        return mk0E(conf);
    case 0x0F:
        return mk0F(conf);
    case 0x10:
        return mk10(conf);
    case 0x17:
        return mk17(conf);
    default:
        break;
    }
    return 0;
}

static void wrChip(u8 firstAdr, u8 endAdr)
{
    u8 buf[17];
    int i;

    if (endAdr==firstAdr) return;
    if ((endAdr-firstAdr)>16)
        endAdr=firstAdr+16;
    for (i=firstAdr;i<endAdr;++i)
        buf[i-firstAdr+1]=mkRegByte(&conf,i);
    buf[0]=firstAdr;
    i2c_wr(I2C_BUS_MB,I2C_ADR_CLK,buf,endAdr-firstAdr+1);
}

void clk_set_enable_pll(int pll, int on)
{
    if (pll==1) {
        conf.pll1.en=on;
        wrChip(0x57,0x58); //[RO]: changed to match the HW
    } else if (pll==2) {
        conf.pll2.en=on;
        wrChip(0x13,0x14);
    } else {
        conf.pll3.en=on;
        wrChip(0x16,0x17);
    }
}

void clk_enable_pll(int pll)
{
    clk_set_enable_pll(pll,1);
}

void clk_disable_pll(int pll)
{
    clk_set_enable_pll(pll,0);
}

void clk_init(void)
{
    int i;

    for (i=0;i<6;++i)
        conf.clkdiv[i]=0;
    for (i=0;i<4;++i)
        conf.fs[i]=0;
    conf.acadj_cx=1;
    conf.acadj_abde=1;
    conf.xbufoe=0;
    conf.div_clke=0;
    conf.dcadj_x=1;
    conf.dcadj_de=1;
    conf.dcadj_c=1;
    conf.dcadj_ab=1;
    conf.osc_cap=0;
    conf.osc_drv=0;
    conf.pll1.en=0;
    conf.pll2.en=0;
    conf.pll3.en=0;
    wrChip(0x8,0x18);
    wrChip(0x40,0x50);
    wrChip(0x50,0x58);
}

static void setPLL(clk_pll_config_t *pll, u16 p, u8 q)
{
    if (q<2) {
        pll->en=0;
        return;
    }
    pll->po=(p-6)&1;
    pll->p=((u16)(p-6)>>1);
    pll->q=(u8)(q-2);
    if (p<231) pll->lf=0;
    else if (p<626) pll->lf=1;
    else if (p<834) pll->lf=2;
    else if (p<1043) pll->lf=3;
    else pll->lf=4;
}

void clk_set_pll(int pll, u16 p, u8 q)
{
    switch(pll) {
    case 1:
        setPLL(&(conf.pll1),p,q);
        wrChip(0x55,0x58); //[RO]: Changed to match the HW

        break;
    case 2:
        setPLL(&(conf.pll2),p,q);
        wrChip(0x11,0x14);
        break;
    case 3:
        setPLL(&(conf.pll3),p,q);
        wrChip(0x14,0x17);
        break;
    default:
        break;
    }
}

void clk_set_out(int output, int pllsrc, u8 div)
{
    if (output>3) return;
    if (pllsrc>7) return;
    conf.fs[output]=pllsrc<<1;
    switch(output) {
    case CLK_PIN_A:
        conf.clkdiv[0]=div;
        wrChip(8,9);
        break;
    case CLK_PIN_B:
        conf.clkdiv[2]=div;
        wrChip(10,11);
        break;
    case CLK_PIN_C:
        conf.clkdiv[4]=div;
        wrChip(12,13);
        break;
    case CLK_PIN_D:
        conf.clkdiv[5]=div;
        wrChip(13,14);
        break;
    }
    wrChip(14,15);
}


// [LL]: new algorithm for calculating q, p, post
float clk_calc_div(float in, float out, u16 *p, u16 *q, u8 *post)
{
    u16 q_min;
    u16 q_max;
    u16 q_cur;

    u16 post_min;
    u16 post_max;
    u16 post_cur;

    u16 p_min;
    u16 p_max;
    u16 p_cur;

    u16 isDone = 0;
    u16 hasResult = 0;

    double error;
    double error_save = 3.4028235e38;   // highest possible double
    double fout_real;
    double fout_save;

    double fout_fref;
    double fvco_fref;
    double fmax_fref;
    double ppm;
    double ddiv;

    *p = 0;
    *q = 0;

    // check first if when can bypass PLL and use post divider only
    ddiv = in/out;
    if (ddiv == round(ddiv)) {
        *post = (unsigned int)ddiv;
        if (*post < 128) {
            return out;
        }
    }

    q_min = 2;
    // phase detector frequency cannot be higher than 250kHz, is dependent on Q
    q_max = (unsigned int)(in/FREQ_PHASEDET);
    if (q_max > 257) {
        q_max = 257;
    }

    post_min = 1;
    post_max = 127;

    p_min = 16;

    fvco_fref = FREQ_THRESVCO / in;
    fmax_fref = FREQ_MAXVCO / in;
    ppm = out / 1000000.0 * MAX_PPM;

    for(post_cur=post_max; post_cur>post_min; post_cur--) {

        fout_fref = out/in * (double)post_cur;

        for(q_cur=q_min; q_cur<q_max; q_cur++) {
            if (post_cur > 31) {
                p_max = (unsigned int)(fvco_fref*(double)q_cur);
            } else {
                p_max = (unsigned int)(fmax_fref*(double)q_cur);
            }
            p_cur = (unsigned int)(fout_fref * (double)q_cur + 0.5);
            if (p_cur < p_min) {
                continue;
            }
            if (p_cur > p_max) {
                break;
            }
            // calculate real output frequency with given p, q, post
            fout_real = in * (double)p_cur / (double)q_cur / (double)post_cur;
            error = fabs(fout_real-out);
            // update return values if result is in request ppm and better than the saved
            if (error < ppm && error < error_save) {
                error_save = error;
                fout_save = fout_real;
                *p = p_cur;
                *q = q_cur;
                *post = post_cur;
                hasResult = 1;
            }
            // hard break if we hit the exact frequency
            if (fout_real == out) {
                isDone = 1;
                break;
            }
        }
        if (isDone) {
            break;
        }
    }
    if (!hasResult) {
        *post = 1;
        return 0;
    }
    return fout_save;
}


float clk_set_freq(int *pll, float freq, u8 *post)
{
    u16 p, q;
    float out;

    //out=clk_calc_div(REFCLK,freq,&p,&q,post); // [RO]: changed to get the actual frequency
    out=clk_calc_div((float)mmb0_clkout_get(),freq,&p,&q,post);
    if (p==0 && q==0 && *post !=0 && out!=0) { //[RO]: added the possibility to use the post divider only
       *pll= CLK_SRC_REF;
    }
    //printf("out=%f p=%d q=%d post=%d\n",out,(int)p,(int)q,(int)post);
    clk_set_pll(*pll,p,q);
    return out;
}

float clk_set_outfreq(int output, int pll, float freq)
{
    u8 post;
    float act;

    clk_set_out(output,pll,0);
    clk_disable_pll(pll);
    act=clk_set_freq(&pll,freq,&post);
    if (act!=0) {         // [RO]: Only if the PLL can reprodice the desired frequency
        clk_enable_pll(pll); // [RO]: this will actually set the pll enable bit even if only the post
                             // divider is used but as q and p are zero, the PLL remains disabled
        clk_set_out(output,pll,post);
    }
    return act;
}
