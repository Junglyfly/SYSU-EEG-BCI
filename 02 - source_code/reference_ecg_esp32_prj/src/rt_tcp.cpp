#include "rt_tcp.h"

WiFiClient TcpClient;
WiFiUDP Udp;                                    //创建UDP对象
// const IPAddress tcpServerIP(192, 168, 52, 168); //欲访问的地址

void print_hex(uint8_t *data, uint8_t len)
{
    uint8_t i = 0;
    for (i = 0; i < len; i++)
    {
        Serial.print(data[i], HEX);
        Serial.print(" ");
    }
    Serial.println();
}
uint8_t check_wifi_status()
{
    uint8_t result = 0;
    wifi_mode_t m = WiFi.getMode();
    if (m == WIFI_MODE_AP)
    {
        result = WiFi.softAPgetStationNum();
        return result;
    }
    else if (m == WIFI_MODE_STA)
    {
        if (WiFi.status() == WL_CONNECTED)
        {
            return 1;
        }
    }
    return 0;
}
void udp_task(void *pvParameters)
{
    uint8_t udp_read_buf[128] = {0};
    int udp_read_len = 0;
    (void)pvParameters;
    uint8_t _status = 0;
    while (1)
    {
        switch (_status)
        {
        case 0: //等待wifi连接完成
            while (!check_wifi_status())
            {
                vTaskDelay(100);
            }
            if (check_wifi_status())
            {
                Serial.println("UDP satrt");
                Udp.begin(UDP_LOCAL_PORT); //启用UDP监听以接收数据
                led_set_mode(LED_MODE_udp);
                _status = 1;
            }

            break;
        case 1: //等待数据接收
            while (check_wifi_status())
            {

                udp_read_len = Udp.parsePacket();
                if (udp_read_len > 0)
                {
                    Udp.read(udp_read_buf, udp_read_len);
                    Serial.print("udp Received:");
                    print_hex(udp_read_buf, udp_read_len);
                    unpack_data(udp_read_buf, udp_read_len);
                    udp_read_len = 0;
                }
                if (TcpClient.connected())
                {
                    Udp.stop();
                    _status = 2;
                    break;
                }
                vTaskDelay(50);
            }
            break;
        case 2:  //TCP连接之后
            while (TcpClient.connected())
            {
                vTaskDelay(200);
            }
            _status = 0;
            break;
        
        default:
            _status = 0;
            break;
        }
        vTaskDelay(50);
    }

    // while (1)
    // {
    //     // if (WiFi.status() == WL_CONNECTED)
    //     if (check_wifi_status())
    //     {
    //         Serial.println("UDP satrt");
    //         Udp.begin(UDP_LOCAL_PORT); //启用UDP监听以接收数据
    //         led_set_mode(LED_MODE_udp);
    //         // while (WiFi.status() == WL_CONNECTED)
    //         while (check_wifi_status())
    //         {

    //             udp_read_len = Udp.parsePacket();
    //             if (udp_read_len > 0)
    //             {
    //                 Udp.read(udp_read_buf, udp_read_len);
    //                 Serial.print("udp Received:");
    //                 print_hex(udp_read_buf, udp_read_len);
    //                 unpack_data(udp_read_buf, udp_read_len);
    //                 udp_read_len = 0;
    //             }
    //             vTaskDelay(50);
    //         }
    //         led_set_mode(LED_MODE_wifi);
    //     }
    //     vTaskDelay(1000);
    // }
    vTaskDelete(NULL);
}

void init_tcpip()
{
    // Udp.begin(UDP_LOCAL_PORT); //启用UDP监听以接收数据
    // xTaskCreate(udp_read_task, "udp_read_task", 4096, NULL, 1, NULL);
    xTaskCreate(udp_task, "udp_task", 4096, NULL, 1, NULL); //创建UDP任务
    // tcp_connect(tcpServerIP);
    xTaskCreate(network_task, "network_task", 4096, NULL, 1, NULL); //创建tcp任务
}

// void tcp_connect(IPAddress ip)
// {
//     uint8_t recount = 0;
//     while (recount < 5)
//     {
//         //tcpServerIP
//         if (TcpClient.connect(IPAddress(192, 168, 52, 168), TCP_REMOTTE_PORT))
//         {
//             Serial.println("tcp connected");
//             break;
//         }
//         recount++;
//     }
// }
void network_task(void *pvParameters)
{
    uint8_t _status = STCP_RESET;
    uint8_t tcp_read_buf[128] = {0};
    int tcp_read_len = 0;
    uint8_t tcp_write_buf[1024] = {0};
    int tcp_write_len = 0;
    uint8_t _tcp_resent = 0;

    // int tcp_real_write_len = 0;
    while (1)
    {
        switch (_status)
        {
        case STCP_RESET:
            while (!mSystem_info.start_tcp)
            {
                vTaskDelay(50);
            }
            _status = STCP_CONNET;
            break;
        case STCP_CONNET:
            if (TcpClient.connect(IPAddress(mSystem_info.tcp_ip), mSystem_info.tcp_port), 1)
            {
                if (TcpClient.connected())
                {
                    Serial.println("tcp connected");
                    led_set_mode(LED_MODE_stop);
                    _status = STCP_CONNECTED;
                    lwrb_write(&ring_buf, &Num_Info_frames, 8);
                }
                else
                {
                    Serial.println("tcp connet time out");
                }
                break;
            }
            else
            {
                Serial.println("tcp connet fail");
            }
            vTaskDelay(200);
            break;
        case STCP_CONNECTED:
            while (TcpClient.connected())
            {
                tcp_read_len = TcpClient.available();
                if (tcp_read_len > 0)
                {
                    TcpClient.read(tcp_read_buf, tcp_read_len);
                    Serial.print("tcp Received:");
                    print_hex(tcp_read_buf, tcp_read_len);
                    // Serial.println((char *)tcp_read_buf);
                    // TcpClient.write(tcp_read_buf, tcp_read_len);
                    unpack_data(tcp_read_buf, tcp_read_len);
                    tcp_read_len = 0;
                }
                if (_tcp_resent)
                {
                    if (TcpClient.write(tcp_write_buf, tcp_write_len) == tcp_write_len)
                    {
                        _tcp_resent = 0;
                    }
                    else
                    {
                        _tcp_resent--;
                        vTaskDelay(50);
                        continue;
                    }
                }
                if ((tcp_write_len = lwrb_read(&ring_buf, tcp_write_buf, 1024)) > 0)
                {
                    if (TcpClient.write(tcp_write_buf, tcp_write_len) != tcp_write_len)
                    {
                        _tcp_resent = 200;
                    }
                    // Serial.println(tcp_real_write_len);tcp_real_write_len =
                }
                vTaskDelay(50);
            }
            Serial.println("tcp disconnected");
            sensor_stop();
            led_set_mode(LED_MODE_tcp);
            _status = STCP_CONNET;
            break;
        default:
            _status = STCP_CONNET;
            break;
        }
        vTaskDelay(50);
    }
    vTaskDelete(NULL);
}
