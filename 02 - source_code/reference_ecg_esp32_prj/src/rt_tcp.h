#ifndef __RT_TCP_H__
#define __RT_TCP_H__

#include <WiFi.h>
#include <WiFiUdp.h> //引用以使用UDP
#include "Ticker.h"
#include "rt_task.h"

#define UDP_LOCAL_PORT   8010
#define UDP_REMOTTE_PORT 8010

#define TCP_REMOTTE_PORT 8080

enum _TCP_STATUS
{
    STCP_RESET=0,
    STCP_CONNET,
    STCP_CONNECTED,
};

extern WiFiClient TcpClient;
extern WiFiUDP Udp;

void init_tcpip();
// void tcp_connect(IPAddress ip);
void network_task(void *pvParameters);

#endif
