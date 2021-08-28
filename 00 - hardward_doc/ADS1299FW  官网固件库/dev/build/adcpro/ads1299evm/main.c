#include "ads1299evm.h"
#include "mmb0.h"
#include "usb.h"
#include "mmb0ui.h"

extern int mmb0_mkdir(estyx_server_t *srv, estyx_file_t *parent);
extern int ads1299evm_mkdir(estyx_server_t *srv, estyx_file_t *parent);
extern void usbstyx_main(void);

int user_setup(estyx_server_t *srv)
{
    int status;
    mmb0_mkdir(srv,0L);
    status = ads1299evm_mkdir(srv,0);
    if (status != 0)
        led_showchar('e');
    return 0;
}

void main(void)
{
    (void)mmb0_cpuclk_set(192000000L);
    (void)usbstyx_main();
}
