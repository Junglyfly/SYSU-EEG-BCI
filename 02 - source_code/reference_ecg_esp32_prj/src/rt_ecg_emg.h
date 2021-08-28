#ifndef __RT_ECG_EMG_H__
#define __RT_ECG_EMG_H__

#include "Arduino.h"
#include <SPI.h>
#include "rt_task.h"

#define PIN_NUM_MISO 32
#define PIN_NUM_MOSI 14
#define PIN_NUM_CLK 25
#define PIN_NUM_CS1 21
#define PIN_NUM_CS2 33

#define PIN_NUM_ECG_DRDY 22
#define PIN_NUM_EXG_DRDY 34
#define PIN_NUM_EXG_START 26
#define PIN_NUM_EXG_PWDN 19  //1298
#define PIN_NUM_EMG_PWDN 15  //1292
#define PIN_NUM_EXG_RESET 27
#define PIN_NUM_EXG_CRYS 4

#define CMD_STANDBY 0x04
#define CMD_SDATAC 0x11
#define CMD_START 0x08
#define CMD_RDATA 0x12
#define CMD_RDATAC 0x10
#define CMD_STOP 0x0A
#define CMD_RESET 0x06

#define EMG_SPS500_REGVALUE 0x02
#define EMG_SPS1K_REGVALUE  0x03

#define ECG_SPS500_REGVALUE 0x05
#define ECG_SPS250_REGVALUE 0x06

#define CH_COUNT  2//通道数量
struct Frame
{
    float fdata[CH_COUNT];
    unsigned char tail[4]{0x00, 0x00, 0x80, 0x7f};
};

void sensor_spi_init();
void exg_sensor_init();

// void sensor_spi_write_read(uint8_t *write_buf, uint8_t *read_buf, uint8_t len);
// void ads129x_read_register(uint8_t addr, uint8_t *buffer, uint8_t len, uint8_t cs);
// void ADS129x_Send_CMD(uint8_t *cmd, uint8_t len, uint8_t cs);
void ADS129x_Read_Data(uint8_t *buffer, uint8_t len, uint8_t cs);
void sensor_start();
void sensor_stop();
void emg_set_rate(uint8_t value);
void exg_set_rate(uint8_t emg_rate, uint8_t ecg_rate);

#endif

