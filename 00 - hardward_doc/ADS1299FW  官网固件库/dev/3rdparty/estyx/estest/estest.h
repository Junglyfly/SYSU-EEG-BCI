
#ifndef GUARD_estest_h
#define GUARD_estest_h

#include "../estyx.h"

estyx_server_t* estest_init(estyx_tx_cb txcb);
int estest_rx(u16 len, char* buf);

#endif
