#ifndef __RT_TASK_H__
#define __RT_TASK_H__

#include <Arduino.h>
#include <Preferences.h>
#include "lwrb.h"
#include "rt_tcp.h"
#include "Button2.h"
#include "rt_wifi.h"
#include "rt_ecg_emg.h"

#define SAMPLE_DATA_NUM 10
#define PIN_NUM_LEDR 18
#define PIN_NUM_LEDB 12
#define PIN_NUM_ADC  36//15
#define PIN_NUM_Btn 13
#define PIN_NUM_POWE 2

#define BAT_INTERVAL_100MS 600 //电池采样间隔 n*100ms
#define PREFS_NUM_KEY "module_num" //电池采样间隔 n*100ms

// typedef enum _System_status_value
// {
//     SYS_RESET = 0,
//     SYS_WIFI_CONNECTED,
//     SYS_UDP_START,
//     SYS_TCP_CONNECTED,
// } System_status_value;

typedef enum _LED_MODE_value
{
    LED_MODE_wifi = 0,  //wifi连接
    LED_MODE_udp,  //等待udp广播
    LED_MODE_tcp,  //等待tcp连接
    LED_MODE_stop, //连接tcp成功
    LED_MODE_start, //开始采集
    LED_MODE_update, //配置和OTA模式
    LED_MODE_MAX,
} LED_MODE_value;

//*用于发送EMG和ECG数据
typedef struct _Data_frame
{
    uint8_t start_byte{0xAF};
    uint8_t len_byte;
    uint8_t type_byte;
    uint16_t frame_num;
    uint8_t data[255];
    uint8_t end_byte{0xAE};
    uint8_t count_byte;
}
__attribute__((packed, aligned(1))) Data_frame;

//用于电池电量和spo2的数据发送
typedef struct _Info_frame
{
    uint8_t start[3]{0xAF, 0x02,0x00};
    uint16_t num{0};
    uint8_t end[3]{0x00,0x00,0xAE};
} __attribute__((packed, aligned(1))) Info_frame;

typedef struct _System_info
{
    uint8_t status;
    uint8_t wifi_mode;
    uint8_t event;
    uint8_t start_tcp;
    uint8_t tcp_ip[4];
    uint16_t tcp_port;
    uint8_t emg_rate;
    uint8_t ecg_rate;
    uint8_t ecg_count;
    uint8_t is_start;
    uint8_t num;
}System_info;

extern System_info mSystem_info;
extern Data_frame emg_Data_frame;
extern Data_frame ecg_Data_frame;
extern lwrb_t ring_buf;
extern Info_frame Spo2_Info_frame;
extern Info_frame Num_Info_frames;

void bsp_init();

void task_init();
void pack_sensor_data(Data_frame *frame, uint8_t *data, uint8_t len);
void unpack_data(uint8_t *data, uint8_t len);
void led_set_mode(LED_MODE_value mode);

#endif
