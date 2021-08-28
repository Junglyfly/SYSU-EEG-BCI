#include "acquire.h"
#include "BlockQueue.h"
extern "C" {
#include <std.h>
#include <swi.h>
#include "mmb0.h"
#include "utilfiles.h"
#include "mmb0ui.h"
#include "tidc_api.h"

extern void* hAdsDeviceHandle;
}

/**
 * Defines the start address of the block queue.
 * It is defined by the end of the SDRAM section as defined
 * in the *.cdb file
*/
#define QUEUE_START 0x038000

/**
 * Defines the end address of the block queue. It
 * is defined by the memory map of the DSP
*/
#define QUEUE_END 0x7F8000


/* this is adjusted for a suitable update rate. */
static long acquire_blocksize;

static int acquiring;
/**
 * The block queue access function.
 *
 * \see BlockQueue
*/
static BlockQueue adcq((long*)QUEUE_START,(long*)QUEUE_END);


/***************************************************************/
/* acquire_adc_done()                                          */
/***************************************************************/
extern "C" void acquire_adc_done(void* p)
{
    adcq.finishWrite();
    acquire_set(0);
}


/***************************************************************/
/* acquire_next_block()                                        */
/***************************************************************/
extern "C" void acquire_next_block(void)
{
    if (acquiring) {
        long* buf;
        long count=acquire_blocksize;
        buf=adcq.startWrite(count);
        if (!buf) {
            acquiring=0;
            return;
        }
        (void)dc_readblock(hAdsDeviceHandle, (void*)(buf), acquire_blocksize, acquire_adc_done);
    }
}

/***************************************************************/
/* acquire_get()                                               */
/***************************************************************/
extern "C" int acquire_get(void)
{
    return acquiring;
}

/***************************************************************/
/* acquire_set()                                               */
/***************************************************************/
extern "C" void acquire_set(int i)
{
    if (i==acquiring) return;
    if (i) {
        /* clear the memory queue before starting a new block */
        adcq.clear();
        SWI_disable();
        acquiring=1;
        acquire_next_block();
        SWI_enable();
    } else {
        acquiring=0;
    }
}


/***************************************************************/
/* acquire_data_read()                                         */
/***************************************************************/
extern "C" int acquire_data_read(estyx_fid_t* f, u16 len, u32 pos)
{
    long count=len>>2;
    SWI_disable();
    long* buf=adcq.startRead(count);
    SWI_enable();
    int res=estyx_rread(f,count<<2,(u8*)(buf));
    SWI_disable();
    adcq.finishRead();
    SWI_enable();
    return 0;
}

/***************************************************************/
/* acquire_data_write()                                        */
/***************************************************************/
extern "C" int acquire_data_write(estyx_fid_t* f, u16 *len, u8 *data, u32 pos)
{
    if (!acquiring) {
        *len=0;
        adcq.clear();
    }
    return 0;
}

/***************************************************************/
/* acquire_data_close()                                        */
/***************************************************************/
extern "C" void acquire_data_close(estyx_fid_t* f)
{
    adcq.finishRead();
}


/***************************************************************/
/* acquire_data_len()                                          */
/***************************************************************/
extern "C" u32 acquire_data_len(estyx_file_t* f)
{
    return adcq.len()<<2; // convert to bytes
}


/***************************************************************/
/** Declare the structure for the file access function for the
 * \p data file. The members of this structure are explained in
 * \p estyx.h
 *
 * \see estyx_fops_t
*/
static const estyx_fops_t data_ops={
    acquire_data_read,
    acquire_data_write,
    0,
    acquire_data_close,
    acquire_data_len
};


/***************************************************************/
/* acquire_blocksize_set()                                     */
/***************************************************************/
extern "C" void acquire_blocksize_set(u32 n)
{
    acquire_blocksize=n;
}


/***************************************************************/
/* acquire_blocksize_get()                                     */
/***************************************************************/
extern "C" u32 acquire_blocksize_get(void)
{
    return acquire_blocksize;
}


/***************************************************************/
/* acquire_init()                                              */
/***************************************************************/
int acquire_init(estyx_server_t* srv, estyx_file_t* parent)
{
    acquiring=0;
    acquire_blocksize=576;

    if (!estyx_mkfile(srv,parent,"data",(estyx_fops_t*)(&data_ops),0)) return -1;
    if (!estyx_file_bool_make(srv,parent,"acquire",acquire_get,acquire_set)) return -1;
    if (!estyx_file_u32_make(srv,parent,"blocksize_samples",acquire_blocksize_get,acquire_blocksize_set,0)) return -1;

    return 0;
}

