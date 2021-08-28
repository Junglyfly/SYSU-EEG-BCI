#include "rt_spo2.h"

Ticker Serial2ReadTicker;
Ticker Spo2Ticker;

uint8_t Serial2ReadBuf[32] = {0};
uint8_t Serial2ReadLen = 0;
uint8_t spo2Value = 0;
uint8_t hrValue = 0;
uint8_t spo2_isready = 0;

void Serial2ReadCallback()
{
    uint8_t i = 0;
    static uint8_t _data_len = 0;
    Serial2ReadLen = Serial2.available();
    if (Serial2ReadLen > 0)
    {
        Serial2.read(Serial2ReadBuf + _data_len, Serial2ReadLen);
        _data_len = _data_len + Serial2ReadLen;
        if (_data_len > 6)
        {
            while ((_data_len - i) > 6)
            {
                if (Serial2ReadBuf[i] & 0x80)
                {
                    i += 7;
                    hrValue = Serial2ReadBuf[i+3];
                    spo2Value = Serial2ReadBuf[i+4];
                    spo2_isready = 1;
                }
                else
                {
                    i++;
                }
            }
            _data_len = _data_len - i;
        }
            // if ((Serial2ReadLen == 7) && (Serial2ReadBuf[0] & 0x80))
            // {
            //     // Serial.println(Serial2ReadBuf[4]);
            //     hrValue = Serial2ReadBuf[3];
            //     spo2Value = Serial2ReadBuf[4];
            //     spo2_isready = 1;
            // }
    }
}
void Spo2TickerCallback()
{  
    if(mSystem_info.is_start)
    {
        if(spo2_isready)
        {            
            spo2_isready = 0;
        }
        else
        {
            hrValue = 254;
            spo2Value = 254;
        }
        Spo2_Info_frame.end[0] = hrValue;
        Spo2_Info_frame.end[1] = spo2Value;
        // Serial.printf("hr:%d spo2:%d\r\n",hrValue,spo2Value);
        lwrb_write(&ring_buf, &Spo2_Info_frame, 8);
        Spo2_Info_frame.num++;
    }
} 
void spo2_init()
{
    Serial2.begin(9600);
    // Serial2.setRxBufferSize(7);
    Serial2ReadTicker.attach_ms(10, Serial2ReadCallback);
    Spo2Ticker.attach_ms(1000, Spo2TickerCallback);
}
