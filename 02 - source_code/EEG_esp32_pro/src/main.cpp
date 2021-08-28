#include <Arduino.h>
#include "bsp_tcp.h"
#include "bsp_ads1299.h"
#include "data_upload.h"

#define LED_R_Pin   5 
#define LED_G_Pin   18
#define LED_B_Pin   19  

#define BATT_ADC    34
#define ESP_TOUCH2  2
#define ESP_TOUCH3  15


void vSystem_basic_task(void * parameter)
{
  pinMode(LED_R_Pin, OUTPUT);
  pinMode(LED_G_Pin, OUTPUT);
  pinMode(LED_B_Pin, OUTPUT);

  const unsigned char ucLed_Pin[3] = {LED_R_Pin,LED_G_Pin,LED_B_Pin};
  unsigned char i = 0;
  while(1){
    i = i > 2 ? 0 : i;
    digitalWrite(ucLed_Pin[i],HIGH);
    vTaskDelay(100);
    digitalWrite(ucLed_Pin[i],LOW);
    i++;
    vTaskDelay(100);
  }
}




void setup() 
{


  Serial.begin(115200);
  Serial.println();

  xTaskCreate(
              vSystem_basic_task,          /*任务函数*/
              "vSystem_basic_task",        /*带任务名称的字符串*/
              1000,                        /*堆栈大小，单位为字节*/
              NULL,                        /*作为任务输入传递的参数*/
              1,                           /*任务的优先级*/
              NULL);                       /*任务句柄*/


  // xTaskCreate(
  //             vTCP_Connect_task,          /*任务函数*/
  //             "vTCP_Connect_task",        /*带任务名称的字符串*/
  //             4000,                       /*堆栈大小，单位为字节*/
  //             NULL,                       /*作为任务输入传递的参数*/
  //             2,                          /*任务的优先级*/
  //             NULL);                      /*任务句柄*/
 

  xTaskCreate(
              vADS1299_CaptureData_task,           /*任务函数*/
              "vADS1299_CaptureData_task",         /*带任务名称的字符串*/
              12000,                               /*堆栈大小，单位为字节*/
              NULL,                                /*作为任务输入传递的参数*/
              3,                                   /*任务的优先级*/
              NULL);                               /*任务句柄*/


  xTaskCreate(
              vData_Upload_task,           /*任务函数*/
              "vData_Upload_task",         /*带任务名称的字符串*/
              15000,                               /*堆栈大小，单位为字节*/
              NULL,                                /*作为任务输入传递的参数*/
              3,                                   /*任务的优先级*/
              NULL);                               /*任务句柄*/


}



void loop() {
  delay(100);
}





