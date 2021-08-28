#include <Arduino.h>
#include "rt_wifi.h"
#include "rt_tcp.h"
#include "rt_ecg_emg.h"
#include "rt_task.h"
#include "rt_spo2.h"
void setup()
{
  
  Serial.begin(115200);
  Serial.printf("\r\nFirmware compile time:%s %s\r\n", __DATE__, __TIME__);

  bsp_init();
  exg_sensor_init();
  spo2_init();

  init_tcpip();
  
}

void loop()
{

  // delay(1000);
}
