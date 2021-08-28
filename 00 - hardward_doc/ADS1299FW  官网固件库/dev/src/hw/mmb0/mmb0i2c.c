#include <std.h>
#include <sem.h>
#include "mmb0i2c.h"
#include "mmb0.h"

#define I2C_ACCESS_TIMEOUT_MS 1000L

#ifndef I2C_MODCLOCK
#define I2C_MODCLOCK 10000000UL
#endif

#ifndef I2C_DEFAULT_DATARATE
#define I2C_DEFAULT_DATARATE 400000
#endif

#define POSTDELAY 100000

static u8 current_bus;
static SEM_Handle i2c_sem;

void i2c_lock(void)
{
    SEM_pend(i2c_sem,((long)I2C_ACCESS_TIMEOUT_MS*1000L)/(long)MMB0_US_PER_PRD_TICK);
}

void i2c_unlock(void)
{
    SEM_post(i2c_sem);
}

#if 0
static void sendstop(void)
{
    //if (!(I2CSTR&I2CSTR_BB)) return;
    I2CMDR|=I2CMDR_STP;
    while (I2CMDR&I2CMDR_STP);
}
#endif

static int wr(u8 adr, u8 *b, int len, int stop)
{
    u8 stat, mdr;
    int retVal = 0;

    I2CCNT = len;
    I2CSAR = adr;
    I2CDXR = *b++;

    if (I2CSTR & I2CSTR_NACK)
    {
        // clear the nack & busy bits
        I2CSTR = I2CSTR_NACK | I2CSTR_BB;
    }
    while (I2CSTR & I2CSTR_BB);
    I2CMDR=0x2E20;

    for(;;) 
    {
        stat = I2CSTR;
        mdr = I2CMDR;
        if (stat & I2CSTR_NACK)
        {
            // clear the nack & busy bits
            I2CSTR = I2CSTR_NACK | I2CSTR_BB;
            retVal = -1;
            break;
        }
        //if (stat&I2CSTR_ARDY) break;
        if (!(mdr & I2CMDR_STP)) break;
        if (stat & I2CSTR_XRDY)
            I2CDXR = *b++;
    }
//	for(;;) {
//		I2CMDR=0x0E20;
//		if (I2CMDR&I2CMDR_STP) break;
//	}
//	stat=I2CSTR;
    return retVal;
}

static int rd(u8 adr, u8 *b, int len)
{
    u8 stat, mdr;
    int retVal = 0;

    I2CCNT = len;
    I2CSAR = adr;

    if (I2CSTR & I2CSTR_NACK)
    {
        // clear the nack & busy bits
        I2CSTR = I2CSTR_NACK | I2CSTR_BB;
    }
    while (I2CSTR & I2CSTR_BB);
    I2CMDR = 0x2C20;

    for(;;) 
    {
        stat = I2CSTR;
        mdr = I2CMDR;
        if (stat & I2CSTR_NACK) 
        {
            // clear the nack & busy bits
            I2CSTR = I2CSTR_NACK | I2CSTR_BB;
            retVal = -1;
            break;
        }
        //if (stat&I2CSTR_ARDY) break;
        if (!(mdr & I2CMDR_STP)) break;
        if (stat & I2CSTR_RRDY)
        {
            *b++ = I2CDRR;
        }
    }
//	for(;;) {
//		I2CMDR=0x2C20;
//		if (I2CMDR&I2CMDR_STP) break;
//	}
    if (I2CSTR & I2CSTR_RRDY) //[RO] this fixes the n+1 read bug
        *b++ = I2CDRR;

    return retVal;
}

void i2c_set_bus(u8 bus)
{
    u8 d;

    i2c_lock();
    if (bus==current_bus) {
        i2c_unlock();
        return;
    }
    d=4|(bus&3);
    while (wr(I2C_ADR_MUX>>1,&d,1,1));
    current_bus=bus;
    i2c_unlock();
}

int i2c_get_bus(void)
{
    return current_bus;
}

int i2c_wr(u8 bus, u8 adr, u8 *b, int len)
{
    int e;

    i2c_lock();
    i2c_set_bus(bus);
    e=wr(adr>>1,b,len,1);
    i2c_unlock();
    return e;
}

int i2c_rd(u8 bus, u8 adr, u8 *b, int len)
{
    int e;

    i2c_lock();
    i2c_set_bus(bus);
    e=rd(adr>>1,b,len);
    i2c_unlock();
    return e;
}

int i2c_w8(u8 bus, u8 adr, u8 b)
{
    u8 d=b;
    int e;

    i2c_lock();
    i2c_set_bus(bus);
    e = wr(adr >> 1, &d, 1, 1);
    i2c_unlock();
    return e;
}

short i2c_r8(u8 bus, u8 adr)
{
    u8 d;
    int e;

    i2c_lock();
    i2c_set_bus(bus);
    e = rd(adr >> 1, &d, 1);
    i2c_unlock();
    return (e<0) ? (-1): ((short)d);
}

int i2c_frob(u8 bus, u8 adr, u8 data, u8 mask)
{
    return i2c_w8(bus,adr,(i2c_r8(bus,adr)&~mask)|(data&mask));
}

void i2c_reset(u32 clkrate, u16 selfadr)
{
    u16 psc, d, div;

    I2CMDR  = 0; // resets module by clearing IRS
    I2CMDR2 = 0;
    I2CIER  = 0; // we poll everything at this time
    psc = (mmb0_cpuclk_get() / I2C_MODCLOCK) - 1;
    
    if (psc == 0) 
        d = 7;
    else if (psc == 1) 
        d = 6;
    else 
        d=5;

    div = ((mmb0_cpuclk_get() / (psc + 1)) / clkrate) - (d * 2);
    I2CPSC  = psc;
    // divide divider between clock high and low remainder in low
    I2CCLKH = div / 2;
    I2CCLKL = div - div / 2;
    I2COAR  = selfadr >> 1;
    I2CMDR  = I2CMDR_IRS;
    I2CSTR  = 0x301F;//I2CSTR_BB;
}

void i2c_init(void)
{
    i2c_sem = SEM_create(0, NULL);
    current_bus = 0xff;
    i2c_reset(I2C_DEFAULT_DATARATE, 0);
    i2c_set_bus(I2C_BUS_MB);
}
