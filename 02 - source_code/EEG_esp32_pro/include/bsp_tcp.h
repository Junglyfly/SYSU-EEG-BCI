#ifndef __BSP_TCP_H__
#define __BSP_TCP_H__

#include <WiFi.h>

#define UDP_LOCAL_PORT   8010
#define UDP_REMOTTE_PORT 8010

#define TCP_REMOTTE_PORT 8080

enum _TCP_STATUS
{
    STCP_RESET=0,
    STCP_CONNET,
    STCP_CONNECTED,
};


void bsp_wifi_init();
void vTCP_Connect_task(void * parameter);

#endif

