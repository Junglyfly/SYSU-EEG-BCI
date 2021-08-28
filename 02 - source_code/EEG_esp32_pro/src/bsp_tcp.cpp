#include <Arduino.h>
#include <WiFi.h>
#include "bsp_tcp.h"


const char *ssid = "junglyfly";
const char *password = "abcd1234";

const IPAddress serverIP(192,168,50,14); //欲访问的地址

uint16_t serverPort = 8000;         //服务器端口号

WiFiClient client; //声明一个客户端对象，用于与服务器进行连接

void bsp_wifi_init()
{
    WiFi.mode(WIFI_STA);
    WiFi.setSleep(false); //关闭STA模式下wifi休眠，提高响应速度
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        vTaskDelay(500);
        Serial.print(".");
    }
    Serial.println("Connected");
    Serial.print("IP Address:");
    Serial.println(WiFi.localIP());
}



void vTCP_Connect_task(void * parameter)
{
    
    bsp_wifi_init();
    while (1)
    {
        Serial.println("try visit address of destination");
        if (client.connect(serverIP, serverPort)) //尝试访问目标地址
        {
            Serial.println("visit success");
            client.print("Hello world!");                    //向服务器发送数据
            while (client.connected() || client.available()) //如果已连接或有收到的未读取的数据
            {
                if (client.available()) //如果有数据可读取
                {
                    String line = client.readStringUntil('\n'); //读取数据到换行符
                    Serial.print("get data");
                    Serial.println(line);
                    client.write(line.c_str()); //将收到的数据回发
                }
            }
            Serial.println("close connection");
            client.stop(); //关闭客户端
        }
        else
        {
            Serial.println("visit failure");
            client.stop(); //关闭客户端
        }

        vTaskDelay(600);
    }

}


