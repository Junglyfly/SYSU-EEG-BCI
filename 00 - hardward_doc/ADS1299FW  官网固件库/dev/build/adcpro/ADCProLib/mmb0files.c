
/* implements the basic mmb0 files:

mmb0/
    clkfreq - the frequency on the PLL's CLKB pin
    clkout - frequency on the DSP's CLKOUT pin / the PLL XTALIN pin
    pll - file to set the PLL to new values; returns the last written value
    test/
        ram - sdram tester                 ==> not yet implemented
        eeprom - EEPROM test               ==> not yet implemented
    hw/
        led/
            ch - LED display, character mode - see text
            seg - LED display, set segments directly
            dp - LED display decimal point
        eeprom - EEPROM access             ==> not yet implemented
        sw - read switches                 ==> not yet implemented
        i2c/                               ==> not yet implemented
            bus - which bus is on (0-3)    ==> not yet implemented
            adr - current address we are talking to ==> not yet implemented
            data - data i/o                ==> not yet implemented
        gpio                               ==> not yet implemented
        evmsw                              ==> not yet implemented
*/

#include "estyx.h"
#include "mmb0.h"
#include "usb.h"
#include "utilfiles.h"
#include "mmb0ui.h"
#include "mmb0clk.h"

static float clkfreq_val;
static u32 pll_settings=0;

u32 mmb0_pll_get(void)
{
    return pll_settings;
}

void mmb0_pll_set(u32 settings)
{
    int pll, output;
    u16 p;
    u8 q;
    u8 post;

    pll_settings=settings;

    pll = (int)(settings & 0x00000003L);
    output = (int)((settings >> 2) & 0x00000007L);
    p = (u16)((settings >> 5) & 0x000007FFL);
    q = (u8)((settings >> 16) & 0x000000FFL);
    post = (u8)((settings >> 24) & 0x0000007Fl);
    if (p==0 && q==0 && post !=0) {
       pll= CLK_SRC_REF;
    }
    clk_set_pll(pll,p,q);
    if (p!=0 && q!=0) { // enable only if the PLL is really used
        clk_enable_pll(pll);
    }
    clk_set_out(output, pll, post);

    return;
}

void mmb0_clockout_set(u32 f)
{
    return;
}

u32 mmb0_clockout_get(void)
{
    return mmb0_clkout_get();
}

u32 mmb0_clkfreq_get(void)
{
    return (u32)clkfreq_val;
}

void mmb0_clkfreq_set(u32 f)
{
    clkfreq_val=clk_set_outfreq(CLK_PIN_B,2,f);
}

static int led_dp_get(void)
{
    return led()&0x80;
}

static char lastledchar[2];

static void led_ch_get(u16 *len, char **d)
{
    if (*lastledchar) {
        *len=1;
        *d=lastledchar;
    } else
        *len=0;
}

static void led_ch_set(u16 *len, char *d)
{
    lastledchar[0]=*d;
    *len=1;
    led_showchar(*d);
}

static void led_seg_set(u32 v)
{
    led_set(v);
}

static u32 led_seg_get(void)
{
    return led();
}

int mmb0_mkdir(estyx_server_t *srv, estyx_file_t *parent)
{
    estyx_file_t *mmb0, *led;

    lastledchar[0]=0;
    lastledchar[1]=1;

    if (!(mmb0=estyx_mkdir(srv,parent,"mmb0"))) return -1;
    if (!estyx_file_u32_make(srv,mmb0,"clkfreq",mmb0_clkfreq_get,mmb0_clkfreq_set,0)) return -1;
    if (!estyx_file_u32_make(srv,mmb0,"clkout",mmb0_clockout_get,mmb0_clockout_set,0)) return -1;
    if (!estyx_file_u32_make(srv,mmb0,"pll",mmb0_pll_get,mmb0_pll_set,0)) return -1;
    //if (!(test=estyx_mkdir(srv,mmb0,"test"))) return -1;
    //if (!estyx_file_msg_make(srv,test,"msg","Do not multiply objects without necessity")) return -1;
    //if (!(hw=estyx_mkdir(srv,mmb0,"hw"))) return -1;
    if (!(led=estyx_mkdir(srv,mmb0,"led"))) return -1;
    if (!estyx_file_data_make(srv,led,"ch",led_ch_get,led_ch_set)) return -1;
    if (!estyx_file_u32_make(srv,led,"seg",led_seg_get,led_seg_set,1)) return -1;
    if (!estyx_file_bool_make(srv,led,"dp",led_dp_get,led_dp)) return -1;

    return 0;
}
