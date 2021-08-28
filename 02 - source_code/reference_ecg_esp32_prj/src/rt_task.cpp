#include "rt_task.h"

lwrb_t ring_buf;         //环形缓冲区
uint8_t ring_data[60*1024]; //环形缓冲区
// Ticker watchTicker;      //监视系统状态
Ticker ledTicker;        //led和按键
// uint8_t Spo2_data[8] = {0xAF, 0x02, 0x04, 0x00, 0x00, 0x00, 0x00, 0xAE};
Info_frame Spo2_Info_frame;
Info_frame Bat_Info_frame;
Info_frame Con_Info_frame;
Info_frame Tcp_Info_frame;
Info_frame Num_Info_frames;
Data_frame emg_Data_frame;
Data_frame ecg_Data_frame;
System_info mSystem_info;
// uint8_t config_frame[8] = {0xAF, 0x02, 0x05, 0x00, 0x00, 0x00, 0x00, 0xAE};

Button2 button = Button2(PIN_NUM_Btn, INPUT_PULLDOWN, false, false);
Preferences prefs; // 声明Preferences对象

// uint8_t led_status_num = 2;
// uint8_t led_time_ary[LED_MODE_MAX][16] = {2, 2, 0};

uint8_t led_num = 2;
uint8_t led_tim_ary[16] = {2,2,0};

void led_set_mode(LED_MODE_value mode)
{
    switch (mode)
    {
    case LED_MODE_wifi:
        led_tim_ary[0] = 2;
        led_tim_ary[1] = 2;
        led_num = 2;
        break;
    case LED_MODE_udp:
        led_tim_ary[0] = 10;
        led_tim_ary[1] = 10;
        led_num = 2;
        break;
    case LED_MODE_tcp:
        led_tim_ary[0] = 1;
        led_tim_ary[1] = 20;
        led_num = 2;
        break;
    case LED_MODE_stop:
        led_tim_ary[0] = 1;
        led_tim_ary[1] = 1;
        led_tim_ary[2] = 1;
        led_tim_ary[3] = 20;
        led_num = 4;
        break;
    case LED_MODE_start: //
        led_tim_ary[0] = 1;
        led_tim_ary[1] = 1;
        led_tim_ary[2] = 1;
        led_tim_ary[3] = 1;
        led_tim_ary[4] = 1;
        led_tim_ary[5] = 20;
        led_num = 6;
        break;
    case LED_MODE_update: //
        led_tim_ary[0] = 1;
        led_tim_ary[1] = 1;
        led_tim_ary[2] = 1;
        led_tim_ary[3] = 1;
        led_tim_ary[4] = 1;
        led_tim_ary[5] = 1;
        led_tim_ary[6] = 1;
        led_tim_ary[7] = 20;
        led_num = 8;
        break;
    default:

        break;
    }
}

void led_loop()
{
    static uint8_t _led_count = 0;
    static uint8_t _led_index = 0;

    _led_count++;
    if ((_led_count > led_tim_ary[_led_index]))
    {
        _led_count = 1;
        _led_index++;
        digitalWrite(PIN_NUM_LEDB, _led_index % 2);
        
        if(_led_index>=led_num)
        {
            _led_index = 0;
        }
    }
}
void led_callback() //100ms
{
    
    static uint16_t bat_sample_count = BAT_INTERVAL_100MS-1;
    button.loop();
    if(TcpClient.connected())
    {
        bat_sample_count++;
        if(bat_sample_count>BAT_INTERVAL_100MS)
        {
            bat_sample_count = 0;
            uint16_t vtmp = analogRead(PIN_NUM_ADC); //IO15 ADC获取电压
            Serial.printf("bat:%.2fv\r\n", vtmp*3.3/4096*1.33);
            memcpy(Bat_Info_frame.end, &vtmp, 2);
            Bat_Info_frame.num++;
            lwrb_write(&ring_buf, &Bat_Info_frame, 8);
        }
    }
    led_loop();
}
void longClick(Button2 &btn)
{
    uint8_t led_level = 0;
    Serial.println("long click time out\r\n");
    digitalWrite(PIN_NUM_POWE, LOW);
    // digitalWrite(PIN_NUM_LEDR, LOW);
    // digitalWrite(PIN_NUM_LEDB, HIGH);
    prefs.end(); // 关闭当前命名空间
    while(digitalRead(PIN_NUM_Btn))
    {
        digitalWrite(PIN_NUM_LEDB, led_level);
        digitalWrite(PIN_NUM_LEDR, !led_level);
        led_level = !led_level;
        delay(100);
    }
}

void wifi_event(WiFiEvent_t event) //事件回调函数
{
    switch (event)
    {
    case SYSTEM_EVENT_STA_CONNECTED:
        Serial.println("wifi event connected");
        
        break;
    case SYSTEM_EVENT_STA_DISCONNECTED:
        Serial.println("wifi event disconnected");

        break;
     default:

        break;
    }
}

//初始化和结构体IO
void bsp_init()
{
    boolean result;
    uint8_t btn_count = 0;
    delay(1000);
    pinMode(PIN_NUM_POWE, OUTPUT);
    digitalWrite(PIN_NUM_POWE, HIGH);
    pinMode(PIN_NUM_LEDR, OUTPUT);
    pinMode(PIN_NUM_LEDB, OUTPUT);
    digitalWrite(PIN_NUM_LEDR, LOW);
    digitalWrite(PIN_NUM_LEDB, HIGH);
    pinMode(PIN_NUM_Btn, INPUT_PULLDOWN);
    prefs.begin("rtNamespace"); // 打开命名空间rtNamespace
    mSystem_info.num = prefs.getUChar(PREFS_NUM_KEY, 255); // 获取当前命名空间中的键名为PREFS_NUM_KEY的值// 如果没有该元素则返回默认值255
    Serial.printf("module num:%d\r\n", mSystem_info.num);
    mSystem_info.wifi_mode = 0; //sta模式
    while ((digitalRead(PIN_NUM_Btn)) && (btn_count < 105))
    {
        btn_count++;
        if (btn_count == 60)
        {
            digitalWrite(PIN_NUM_LEDB, LOW);
            digitalWrite(PIN_NUM_LEDR, HIGH);
            mSystem_info.wifi_mode = 1; //ap模式
        }
        if (btn_count == 100)
        {
            digitalWrite(PIN_NUM_LEDB, LOW);
            digitalWrite(PIN_NUM_LEDR, LOW);
            mSystem_info.wifi_mode = 2; //更新模式
            break;
        }
        delay(100);
    }
    digitalWrite(PIN_NUM_LEDR, LOW);
    
    button.setLongClickTime(2000);
    // button.setLongClickHandler(longClick);
    button.setLongClickDetectedHandler(longClick);
    ledTicker.attach_ms(100, led_callback);
    led_set_mode(LED_MODE_wifi);
    WiFi.begin();
    switch (mSystem_info.wifi_mode)
    {
    case 0: //sta模式
        digitalWrite(PIN_NUM_LEDR, HIGH);
        if (!AutoConfig())
        {
            digitalWrite(PIN_NUM_LEDR, LOW);
            led_set_mode(LED_MODE_update);
            wifi_Config();
        }
        while (WiFi.status() != WL_CONNECTED)
        {
            server.handleClient();
        }
        digitalWrite(PIN_NUM_LEDR, HIGH);
        break;
    case 1: //ap模式
        
        WiFi.mode(WIFI_AP);
        result = WiFi.softAP("RT_SENSOR", ""); //开启WIFI热点
        if (result)
        {
            IPAddress myIP = WiFi.softAPIP();
            //打印相关信息
            Serial.println("");
            Serial.print("Soft-AP IP address = ");
            Serial.println(myIP);
            Serial.println(String("MAC address = ") + WiFi.softAPmacAddress().c_str());
            Serial.println("waiting ...");
            while (WiFi.softAPgetStationNum() == 0)
            {
                delay(100);
            }
        }
        else
        { //开启热点失败
            Serial.println("WiFiAP Failed");
            delay(3000);
            ESP.restart(); //复位esp32
        }
        break;
    case 2:
        led_set_mode(LED_MODE_update);
        wifi_Config();
        while (WiFi.status() != WL_CONNECTED)
        {
            server.handleClient();
            if (shouldreboot)
            {
                delay(5000);
                ESP.restart(); // 重启设备
            }
        }
        break;
    default:

        break;
    }
    WiFi.onEvent(wifi_event);
    led_set_mode(LED_MODE_udp);
    // if(mSystem_info.num<8)
    // {
        emg_Data_frame.type_byte = (mSystem_info.num << 4) | 0x01;
        ecg_Data_frame.type_byte = (mSystem_info.num << 4) | 0x02;
        Spo2_Info_frame.start[2] = (mSystem_info.num << 4)| 0x03;
        Bat_Info_frame.start[2] = (mSystem_info.num << 4) | 0x04;
        Con_Info_frame.start[2] = (mSystem_info.num << 4) | 0x05;
        Tcp_Info_frame.start[2] = (mSystem_info.num << 4) | 0x06;
        Num_Info_frames.start[2] = (mSystem_info.num << 4) | 0x07;
    // }
    emg_Data_frame.len_byte = 60;
    ecg_Data_frame.len_byte = 240;
    // emg_Data_frame.type_byte = 0x01;
    // ecg_Data_frame.type_byte = 0x02;
    // Spo2_Info_frame.start[2] = 0x03;
    // Bat_Info_frame.start[2] = 0x04;
    // Con_Info_frame.start[2] = 0x05;
    // Tcp_Info_frame.start[2] = 0x06;
    // Num_Info_frames.start[2] =0x07;
    Num_Info_frames.end[0] = mSystem_info.num;
    lwrb_init(&ring_buf, ring_data, sizeof(ring_data));
}
// void task_init()
// {
    // watchTicker.attach_ms(100, watch_callback);
    // xTaskCreate(read_task,"read_task",1024,NULL,1,NULL)
// }

void pack_sensor_data(Data_frame *frame, uint8_t *data, uint8_t len)
{
    
    memcpy(frame->data + len * frame->count_byte, data, len);
    frame->count_byte++;
    if (frame->count_byte >= SAMPLE_DATA_NUM)
    {
        frame->data[frame->len_byte] = frame->end_byte;
        lwrb_write(&ring_buf, frame, frame->len_byte + 6);
        frame->count_byte = 0;
        frame->frame_num++;
    }
}
void unpack_data(uint8_t *data, uint8_t len)
{
    uint8_t i = 0;
    uint8_t _tmp_len = 0;
    uint8_t _tmp_byte = 0;
    static uint8_t _unpack_buf[128] = {0};
    static uint8_t _unpack_len = 0;
    memcpy(_unpack_buf + _unpack_len, data, len);
    _unpack_len = _unpack_len + len;
    for (i = 0; i < _unpack_len; i++)
    {
        if (_unpack_buf[i] == 0xA7)
        {
            _tmp_len = i + _unpack_buf[i + 1] + 3; //加少一个,_tmp_len指向最后一个字节
            if (_unpack_len > _tmp_len)
            {
                // Serial.printf("frame num:%X\r\n", _unpack_buf[i + 2]);
                if (_unpack_buf[_tmp_len] == 0xAE)
                {
                    // Serial.printf("frame num:%X\r\n", _unpack_buf[i + 2]);
                    switch (_unpack_buf[i + 2])
                    {
                    case 0x81: //开始停止
                        // Serial.println("sensor_start/sensor_stop");
                        if (_unpack_buf[i + 3] == 0x00)
                        {                            
                            Serial.println("sensor_stop");                            
                            led_set_mode(LED_MODE_stop);
                            sensor_stop();
                        }
                        else
                        {                
                            Serial.println("sensor_start");
                            led_set_mode(LED_MODE_start);
                            sensor_start();  
                        }                        
                        break;
                    case 0x82: //配置采样率
                        sensor_stop();
                        lwrb_reset(&ring_buf);
                        mSystem_info.emg_rate = _unpack_buf[i + 5];
                        _tmp_byte = ((_unpack_buf[i + 3] << 1) | _unpack_buf[i + 5]) & 0x03;
                        switch (_tmp_byte)
                        {
                        case 0x00: //ecg 200  emg 500
                            mSystem_info.ecg_rate = 1;
                            // emg_set_rate(EMG_SPS500_REGVALUE);
                            exg_set_rate(EMG_SPS500_REGVALUE, ECG_SPS250_REGVALUE);
                            break;
                        case 0x01: //ecg 200 emg 1000
                            mSystem_info.ecg_rate = 4;
                            // emg_set_rate(EMG_SPS1K_REGVALUE);
                            exg_set_rate(EMG_SPS1K_REGVALUE, ECG_SPS250_REGVALUE);
                            break;
                        case 0x02: //ecg 500  emg 500
                            mSystem_info.ecg_rate = 0;
                            // emg_set_rate(EMG_SPS500_REGVALUE);
                            exg_set_rate(EMG_SPS500_REGVALUE, ECG_SPS500_REGVALUE);
                            break;
                        case 0x03: //ecg 500  emg 1000
                            mSystem_info.ecg_rate = 1;
                            // emg_set_rate(EMG_SPS1K_REGVALUE);
                            exg_set_rate(EMG_SPS1K_REGVALUE, ECG_SPS500_REGVALUE);
                            break;
                        }
                        ecg_Data_frame.count_byte = 0;
                        emg_Data_frame.count_byte = 0;
                        ecg_Data_frame.frame_num = 0;
                        emg_Data_frame.frame_num = 0;
                        Serial.printf("ecg_rate:%d,emg_rate:%d\r\n", mSystem_info.ecg_rate,mSystem_info.emg_rate);
                        Con_Info_frame.end[0] = _unpack_buf[i + 3];
                        Con_Info_frame.end[1] = _unpack_buf[i + 5];
                        TcpClient.write((uint8_t *)&Con_Info_frame, 8);
                        Con_Info_frame.num++;
                        break;
                    case 0x83: //tcp连接数据

                        memcpy(mSystem_info.tcp_ip, &_unpack_buf[i + 3], 4);
                        memcpy(&mSystem_info.tcp_port, &_unpack_buf[i + 7], 2);
                        memcpy(Tcp_Info_frame.end, &_unpack_buf[i + 7], 2);
                        Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
                        Udp.write((uint8_t *)&Tcp_Info_frame, 8);
                        Udp.endPacket();
                        Tcp_Info_frame.num++;
                        mSystem_info.start_tcp = 1;
                        Serial.printf("udp tcp ip:%d.%d.%d.%d:%d\r\n", mSystem_info.tcp_ip[0], mSystem_info.tcp_ip[1], mSystem_info.tcp_ip[2], mSystem_info.tcp_ip[3], mSystem_info.tcp_port);
                        led_set_mode(LED_MODE_tcp);
                        break;
                    case 0x84: //设置编号
                        if (_unpack_buf[i + 3]<8)
                        {
                            mSystem_info.num = _unpack_buf[i + 3];
                            prefs.putUChar(PREFS_NUM_KEY, mSystem_info.num);
                            emg_Data_frame.type_byte = (mSystem_info.num << 4) | 0x01;
                            ecg_Data_frame.type_byte = (mSystem_info.num << 4) | 0x02;
                            Spo2_Info_frame.start[2] = (mSystem_info.num << 4) | 0x03;
                            Bat_Info_frame.start[2] = (mSystem_info.num << 4) | 0x04;
                            Con_Info_frame.start[2] = (mSystem_info.num << 4) | 0x05;
                            Tcp_Info_frame.start[2] = (mSystem_info.num << 4) | 0x06;
                            Num_Info_frames.start[2] = (mSystem_info.num << 4) | 0x07;
                            Num_Info_frames.end[0] = mSystem_info.num;
                            Serial.printf("module num set %d\r\n", mSystem_info.num);
                        }
                        break;
                    default:

                        break;
                    }
                    i += _tmp_len;
                }
            }
            else
            {
                Serial.println("frame Incomplete ");
                break;
            }
        }
    }
    _unpack_len = _unpack_len - i;
    if(_unpack_len>100)
    {
        _unpack_len = 0;
    }
    memcpy(_unpack_buf, _unpack_buf + i, _unpack_len);
    Serial.printf("end_unpack_len:%d\r\n", _unpack_len);
}

