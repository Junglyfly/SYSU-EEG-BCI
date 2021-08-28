#include <std.h>
#include <string.h>
#include <stdlib.h>
#include <swi.h>
#include <tsk.h>
#include <mbx.h>
#include "estyx.h"
#include "mmb0flash.h"
#include "mmb0.h"
#include "usb.h"
#include "utilfiles.h"
#include "mmb0ui.h"
#define USBSTYX_BLKLEN 2048

static estyx_server_t srv;
static SWI_Handle swi_ctl;
static TSK_Handle tsk_rx;
static MBX_Handle mbx_rx;

extern int usb_tx(usb_endpoint_t *ep, usb_data_t *data, u16 len);

static void unpack(u16 *src, u8 *dest);
static void pack(u8 *buf, u16 len, u8 *data, u16 datalen, u16 *pkt);

// instate: 0 = not waiting
// 1 = waiting for first packet
// 2 = got first packet, waiting for more packets

static int instate;
static u16 *inbuf;
static u8 *inpkt;
static u16 inlen;
static usb_endpoint_t *outep; // ie the OUT endpoint

static u16 *outbuf;
static usb_endpoint_t *inep;

void *estyx_malloc(u32 s)
{
    return (void *)malloc(sizeof(u8)*s);
}

void estyx_free(void *p)
{
    free(p);
}

/* Unpacks a Styx packet into 16-bit chars.  We use the packet length
but clamp to BLKLEN

Exception: For Twrite we copy the data portion packed

0	4	5	7	11	19	23
0h	2h	2l	3l	5l	9l	11l
long	byte	word	long	oct	long	data...
len	type
*/

static void unpack(u16 *src, u8 *dest)
{
    u16 i,len,dlen;
    u16 t;
    u16 j;

    len=(*src>>8)|(*src<<8);
    t=src[2]>>8;
    if (len>USBSTYX_BLKLEN) len=USBSTYX_BLKLEN;
    dlen=0;
    if (t==118) {
        if (len>23) {
            dlen=len;
            len=23;
        }
    }

    i=0;
    j=0;
    /* [RO] Changed from a pointer increment to an indexed access*/
    /* to avoid the reset of the pointer at a 64k boundary       */
    for(;;) {
        dest[j++]=*src>>8;
        if (++i>=len||i>=USBSTYX_BLKLEN) break;
        dest[j++]=(*src++)&0xff;
        if (++i>=len||i>=USBSTYX_BLKLEN) break;
    }
    if (i<USBSTYX_BLKLEN&&dlen) {
        for (;;) {
            dest[j]=*src++<<8;
            if (++i>=dlen||i>=USBSTYX_BLKLEN) break;
            dest[j++]|=*src>>8;
            if (++i>=dlen||i>=USBSTYX_BLKLEN) break;
        }
    }
}

/* pack a styx packet for transmission

also concatenate read block if any.

does not work for nonzero datalen and even len, but that should not occur
*/
static void pack(u8 *buf, u16 len, u8 *data, u16 datalen, u16 *pkt)
{
    u16 i;
    u16 j=0; /* [RO] to fix the copy bug at a 64k boundary */
    u16 k=0;

    i=len>=USBSTYX_BLKLEN?USBSTYX_BLKLEN:len;
    usbhw_pack55(buf,pkt,i);
    pkt+=i/2;
    if (i<USBSTYX_BLKLEN&&datalen&&data) {
        datalen+=i;
        for (;;) {
            pkt[k++]|=data[j]>>8;
            if (++i>=datalen||i>=USBSTYX_BLKLEN) break;
            pkt[k]=data[j++]<<8;
            if (++i>=datalen||i>=USBSTYX_BLKLEN) break;
        }
    }
}

int usbstyx_tx(estyx_server_t *srv, u16 len, u8 *buf, u16 datalen, u8 *data)
{
    u16 tlen;

    if (usb_get_epstat(inep)!=USB_EPSTAT_IDLE) {
        return 0;
    }
    pack(buf,len,data,datalen,outbuf);
    //usbhw_dmalog_write(10,17);
    //usb_move_wait(inep,(usb_data_t *)usbstyx_data,len+datalen);
    tlen=(len+datalen)/inep->packetSize;
    if ((len+datalen)%inep->packetSize) tlen+=1;
    usb_tx(inep,(usb_data_t *)outbuf,tlen*inep->packetSize);
    return 0;
}

void listen(usb_endpoint_t *ep, usb_data_t *buf, u16 len)
{
    while (usb_rx(ep,buf,len)) {
        if (usb_get_epstat(ep)!=USB_EPSTAT_IDLE)
            return;
    }
}

void usbstyx_rx(void)
{
    u16 rlen;

    if (instate==0) { // ask for 1 packet
        instate=1;
        listen(outep,inbuf,outep->packetSize);
        return;
    } else if (instate==1) { // got 1 packet, ask for more if nec.
        memmove(inbuf,inbuf+1,outep->packetSize/2);
        inlen=(*inbuf>>8)|(*inbuf<<8);
        if (!inlen) { // dummy packet
            instate=0;
            usbstyx_rx();
            return;
        }
        if (inlen>outep->packetSize) { // need more packets
            instate=2;
            rlen=inlen/outep->packetSize-1;
            if (inlen%outep->packetSize) rlen+=1;
            rlen*=outep->packetSize;
            if (rlen>USBSTYX_BLKLEN) rlen=USBSTYX_BLKLEN;
            listen(outep,inbuf+outep->packetSize,rlen);
            return;
        }
    } else { // (should be 2) got all our packets, unpack and send.
        memmove(inbuf+outep->packetSize/2,inbuf+outep->packetSize/2+1,(inlen-outep->packetSize)/2+1);
    }
    // same as state 0 here
    instate=1;
    unpack(inbuf,inpkt);
    estyx_rx(&srv,inlen,inpkt);
    listen(outep,inbuf,outep->packetSize);
}

static void sys_setup(void);

void usbstyx_task(void)
{
    short evt;

    sys_setup();

    for(;;) {
        MBX_pend(mbx_rx,(short *)&evt,SYS_FOREVER);
        if (evt==USB_EVT_CONFIGURED) {
            /* we do detach here because:
            1. if host s/w disconnects we may not get deconfigured
            2. calling detach with no client is harmless
            */
            estyx_detach(&srv);
            led_dp(1);
        }
        if (evt==USB_EVT_DECONFIGURED||evt==USB_EVT_SUSPENDED) {
            led_dp(0);
        }
        if (evt!=USB_EVT_DECONFIGURED) {
            if (evt!=USB_EVT_READY)
                instate=0;
            usbstyx_rx();
        }
    }
}

void usbstyx_rx_evt_cb(usb_endpoint_t *ep, usb_data_t *data, u16 len, u8 evt)
{
    short mevt;

    mevt=evt;
    MBX_post(mbx_rx,(short *)&mevt,0);
}

void usbstyx_ctl(void)
{
    int err;

    err=usb_ctl_std();
    if (!err) {
        usb_ctl_stall();
    }
}

void usb_ctl(void)
{
    SWI_post(swi_ctl);
}

int user_setup(estyx_server_t *);

static void sys_setup(void)
{
    struct c55x_params p;

    // set up board
    p.clkin_khz=MMB0_CLKIN;
    p.us_per_prd_tick=MMB0_US_PER_PRD_TICK;
    mmb0_init();
    flash_init();
    usb_init(&p);
    led_showchar('s');

    // set up usbstyx
    outep=usb_get_ep(usb_get_config(),1);
    inbuf=estyx_malloc(USBSTYX_BLKLEN/2+16); // make room for length bytes.
    inpkt=estyx_malloc(USBSTYX_BLKLEN);
    inep=usb_get_ep(usb_get_config(),17);
    outbuf=estyx_malloc(USBSTYX_BLKLEN/2);
    instate=0;
    usb_set_evt_cb(outep,usbstyx_rx_evt_cb);
    estyx_init(&srv,0,usbstyx_tx);
    led_dp(0);

    user_setup(&srv);

    usb_attach();
}

void usbstyx_main(void)
{
    SWI_Attrs swi={(SWI_Fxn)usbstyx_ctl,0,0,2,0};
    TSK_Attrs tsk={
        2, // priority
        0, // stack ptr
        2048, // stack size, words
        512, // system stack size, words
        0, // stack segment
        0, // environs
        "", // task name
        FALSE, // must terminate
    };

    mmb0_preinit();
    swi_ctl=SWI_create(&swi);
    tsk_rx=TSK_create((Fxn)usbstyx_task,&tsk);
    mbx_rx=MBX_create(8,1,NULL);
}
