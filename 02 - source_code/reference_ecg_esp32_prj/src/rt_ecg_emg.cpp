#include "rt_ecg_emg.h"

SPIClass *hspi = NULL;
uint8_t spi_rxbuf[32] = {0};
uint8_t spi_txbuf[32] = {0};  //spi操作寄存器使用,仅用于SPI直接操作的函数使用
uint8_t EMG_data[9] = {0};
uint8_t ECG_data[27] = {0};
struct Frame mFrame;
// const uint8_t EMG_REG_VALUE[10] = {0x03, 0xa3, 0x10, 0x65, 0x65, 0x60, 0x00, 0x40, 0x02, 0x03}; //TEST signal
const uint8_t EMG_REG_VALUE[10] = {0x03, 0xa0, 0x10, 0x60, 0x60, 0x60, 0x00, 0x40, 0x02, 0x03};
const uint8_t ECG_REG_VALUE[25] = {0x05, 0x10, 0xce, 0x03, 0x40, 0x40, 0x40, 0x40, 0x40, 0x80, 0x80, 0x40, 0xfe, 0xfe, 0xfe, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf6, 0x22, 0x0A, 0xDC}; //0x05 500sps 0x06 250sps
// const uint8_t ECG_REG_VALUE[25] = {0x05, 0x10, 0xce, 0x03, 0x45, 0x45, 0x45, 0x45, 0x45, 0x45, 0x45, 0x45, 0xfe, 0xfe, 0xfe, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe2, 0x02, 0x0A, 0xDC}; //TEST signal

void sensor_spi_init()
{
    hspi = new SPIClass(HSPI);
    hspi->begin(PIN_NUM_CLK, PIN_NUM_MISO, PIN_NUM_MOSI, -1); //SCLK, MISO, MOSI, SS
    hspi->setBitOrder(MSBFIRST);
    hspi->setFrequency(100000);
    hspi->setDataMode(SPI_MODE1);
    hspi->setHwCs(false);
    pinMode(PIN_NUM_CS1, OUTPUT);
    digitalWrite(PIN_NUM_CS1, HIGH);
    pinMode(PIN_NUM_CS2, OUTPUT);
    digitalWrite(PIN_NUM_CS2, HIGH);
}

void sensor_spi_write_read(uint8_t *write_buf, uint8_t *read_buf, uint8_t len)
{
    for (uint8_t i = 0; i < len; i++)
    {
        read_buf[i] = hspi->transfer(write_buf[i]);
    }  
}
void ads129x_read_register(uint8_t addr, uint8_t *buffer, uint8_t len, uint8_t cs)
{
    spi_txbuf[0] = 0x20 + addr;
    spi_txbuf[1] = 0x00 + len - 1;
    memset(spi_txbuf + 2,0xff,len);
    digitalWrite(cs, LOW);
    sensor_spi_write_read(spi_txbuf, spi_rxbuf, 2);
    sensor_spi_write_read(spi_txbuf+2, buffer, len);
    digitalWrite(cs, HIGH);

}
void ads129x_write_register(uint8_t addr, uint8_t *buffer, uint8_t len, uint8_t cs)
{
    spi_txbuf[0] = 0x40 + addr;
    spi_txbuf[1] = 0x00 + len - 1;
    digitalWrite(cs, LOW);
    sensor_spi_write_read(spi_txbuf, spi_rxbuf, 2);
    sensor_spi_write_read(buffer, spi_rxbuf, len);
    digitalWrite(cs, HIGH);
}

void ADS129x_Send_CMD(uint8_t *cmd, uint8_t len, uint8_t cs)
{
    digitalWrite(cs, LOW);
    sensor_spi_write_read(cmd, spi_rxbuf, len);
    digitalWrite(cs, HIGH);
}



void data_ready_callBack ()
{
    // static uint16_t i = 0;
    // i++;
    ADS129x_Read_Data(EMG_data,9, PIN_NUM_CS1);
    // EMG_data[6] = i;
    // EMG_data[5] = i>>8;
    pack_sensor_data(&emg_Data_frame, EMG_data + 3, 6);
    mSystem_info.ecg_count++;
    if (mSystem_info.ecg_count>mSystem_info.ecg_rate )
    {
        ADS129x_Read_Data(ECG_data, 27, PIN_NUM_CS2);
        pack_sensor_data(&ecg_Data_frame, ECG_data + 3, 24);

        mSystem_info.ecg_count = 0;
    }
    // ADS129x_Read_Data(ECG_data, 27, PIN_NUM_CS2);
    // pack_sensor_data(&ecg_Data_frame, ECG_data + 3, 24);
    // Serial.write(EMG_data,9);
}
void sensor_reset()
{
    uint8_t _tmpcmd[2] = {CMD_SDATAC, CMD_STOP};
    digitalWrite(PIN_NUM_CS1, LOW);
    digitalWrite(PIN_NUM_CS2, LOW);
    digitalWrite(PIN_NUM_EXG_PWDN, HIGH);  //退出掉电模式
    digitalWrite(PIN_NUM_EMG_PWDN, HIGH);
    digitalWrite(PIN_NUM_EXG_RESET, HIGH); //停止复位
    delay(50);
    digitalWrite(PIN_NUM_EXG_PWDN, LOW);
    digitalWrite(PIN_NUM_EMG_PWDN, LOW);
    delay(100);
    digitalWrite(PIN_NUM_EXG_PWDN, HIGH);
    digitalWrite(PIN_NUM_EMG_PWDN, HIGH);
    delay(200);

    // digitalWrite(PIN_NUM_EXG_RESET, LOW);
    // delay(100);
    // digitalWrite(PIN_NUM_EXG_RESET, HIGH);
    // delay(200);

    digitalWrite(PIN_NUM_CS1, HIGH);
    digitalWrite(PIN_NUM_CS2, HIGH);

    _tmpcmd[0] = CMD_RESET;
    ADS129x_Send_CMD(_tmpcmd, 1, PIN_NUM_CS1);
    ADS129x_Send_CMD(_tmpcmd, 1, PIN_NUM_CS2);
    delay(200);
    _tmpcmd[0] = CMD_SDATAC;
    ADS129x_Send_CMD(_tmpcmd, 2, PIN_NUM_CS1);
    ADS129x_Send_CMD(_tmpcmd, 2, PIN_NUM_CS2);
}

void exg_sensor_init()
{
    uint8_t _tmpcmd[3] = {0};
    uint8_t _tmpreg[32] = {0};
    sensor_spi_init();
    pinMode(PIN_NUM_EXG_START, OUTPUT);
    digitalWrite(PIN_NUM_EXG_START, LOW);
    pinMode(PIN_NUM_EXG_PWDN, OUTPUT);
    digitalWrite(PIN_NUM_EXG_PWDN, HIGH);
    pinMode(PIN_NUM_EMG_PWDN, OUTPUT);
    digitalWrite(PIN_NUM_EMG_PWDN, HIGH);
    pinMode(PIN_NUM_EXG_RESET, OUTPUT);
    digitalWrite(PIN_NUM_EXG_RESET, LOW);
    pinMode(PIN_NUM_EXG_CRYS, OUTPUT);
    digitalWrite(PIN_NUM_EXG_CRYS, HIGH);
    delay(10);
    pinMode(PIN_NUM_EXG_DRDY, INPUT_PULLUP);
    attachInterrupt(PIN_NUM_EXG_DRDY, data_ready_callBack, FALLING); //使能中断
    // pinMode(PIN_NUM_ECG_DRDY, INPUT_PULLUP);
    // attachInterrupt(PIN_NUM_ECG_DRDY, data_ready_callBack, FALLING); //使能中断

    sensor_reset();
    ads129x_write_register(0x01, (uint8_t *)EMG_REG_VALUE, 10, PIN_NUM_CS1);
    ads129x_read_register(0x00, _tmpreg, 12, PIN_NUM_CS1);
    for (uint8_t i = 0; i < 12; i++)
    {
        Serial.printf("\r\nREG_DATA[%d] = %x", i, _tmpreg[i]); //打印数据到串口
        // Serial.println(_tmpreg[i],HEX);
    }
    delay(100);
    memset(_tmpreg, 0, sizeof(_tmpreg));
    ads129x_write_register(0x01, (uint8_t *)ECG_REG_VALUE, 25, PIN_NUM_CS2);
    ads129x_read_register(0x00, _tmpreg, 26, PIN_NUM_CS2);
    for (uint8_t i = 0; i < 26; i++)
    {
        Serial.printf("\r\nADS1298 REG_DATA[%d] = %x", i, _tmpreg[i]); //打印数据到串口
        // Serial.println(_tmpreg[i],HEX);
    }
    Serial.println();
    _tmpcmd[0] = CMD_RDATAC;
    _tmpcmd[1] = CMD_STOP;
    ADS129x_Send_CMD(_tmpcmd, 2, PIN_NUM_CS1);
    ADS129x_Send_CMD(_tmpcmd, 2, PIN_NUM_CS2);
    _tmpcmd[0] = CMD_STANDBY;
    // ADS129x_Send_CMD(_tmpcmd, 1, PIN_NUM_CS1);
    memset(spi_txbuf, 0xff, 32);
    hspi->setFrequency(10000000);
    // delay(1000);
    // sensor_start();
}
void ADS129x_Read_Data(uint8_t *buffer, uint8_t len, uint8_t cs)
{
    digitalWrite(cs, LOW);
    sensor_spi_write_read(spi_txbuf, buffer, len);
    digitalWrite(cs, HIGH);
}

void sensor_start()
{
    // uint8_t _tmpcmd = CMD_START;
    // ADS129x_Send_CMD(&_tmpcmd,1, PIN_NUM_CS2);
   
    digitalWrite(PIN_NUM_CS1, LOW);
    digitalWrite(PIN_NUM_CS2, LOW);
    digitalWrite(PIN_NUM_EXG_START, HIGH);
    digitalWrite(PIN_NUM_CS1, HIGH);
    digitalWrite(PIN_NUM_CS2, HIGH);
    mSystem_info.is_start = 1;
}
void sensor_stop()
{
    // uint8_t _tmpcmd = CMD_STOP;
    // ADS129x_Send_CMD(&_tmpcmd, 1, PIN_NUM_CS2);
    
    digitalWrite(PIN_NUM_CS1, LOW);
    digitalWrite(PIN_NUM_CS2, LOW);
    digitalWrite(PIN_NUM_EXG_START, LOW);
    digitalWrite(PIN_NUM_CS1, HIGH);
    digitalWrite(PIN_NUM_CS2, HIGH);
    mSystem_info.is_start = 0;
}

//设置采样率
void emg_set_rate(uint8_t value)
{
    uint8_t _tmpcmd[2] = {CMD_SDATAC, CMD_STOP};
    uint8_t _tmpreg[16] = {0};
    hspi->setFrequency(100000);
    ADS129x_Send_CMD(_tmpcmd, 2, PIN_NUM_CS1);
    _tmpcmd[0] = value;
    ads129x_write_register(0x01, _tmpcmd, 1, PIN_NUM_CS1);
    ads129x_read_register(0x00, _tmpreg, 12, PIN_NUM_CS1);
    for (uint8_t i = 0; i < 12; i++)
    {
        Serial.printf("\r\nREG_DATA[%d] = %x", i, _tmpreg[i]); //打印数据到串口
        // Serial.println(_tmpreg[i],HEX);
    }
    _tmpcmd[0] = CMD_RDATAC;
    _tmpcmd[1] = CMD_STOP;
    ADS129x_Send_CMD(_tmpcmd, 2, PIN_NUM_CS1);
    hspi->setFrequency(10000000);
}

//设置采样率
void exg_set_rate(uint8_t emg_rate,uint8_t ecg_rate)
{
    uint8_t _tmpcmd[4] = {CMD_SDATAC, CMD_STOP, CMD_RDATAC, CMD_STOP};
    uint8_t _tmpreg[28] = {0};
    hspi->setFrequency(100000);

    ADS129x_Send_CMD(_tmpcmd, 2, PIN_NUM_CS2);
    _tmpreg[0] = ecg_rate;
    ads129x_write_register(0x01, _tmpreg, 1, PIN_NUM_CS2);
    ads129x_read_register(0x00, _tmpreg, 27, PIN_NUM_CS2);
    for (uint8_t i = 0; i < 27; i++)
    {
        Serial.printf("\r\nECG REG_DATA[%d] = %x", i, _tmpreg[i]); //打印数据到串口
        // Serial.println(_tmpreg[i],HEX);
    }
    ADS129x_Send_CMD(_tmpcmd + 2, 2, PIN_NUM_CS2);

    ADS129x_Send_CMD(_tmpcmd, 2, PIN_NUM_CS1);
    _tmpreg[0] = emg_rate;
    ads129x_write_register(0x01, _tmpreg, 1, PIN_NUM_CS1);
    ads129x_read_register(0x00, _tmpreg, 12, PIN_NUM_CS1);
    for (uint8_t i = 0; i < 12; i++)
    {
        Serial.printf("\r\nEMG REG_DATA[%d] = %x", i, _tmpreg[i]); //打印数据到串口
        // Serial.println(_tmpreg[i],HEX);
    }
    ADS129x_Send_CMD(_tmpcmd + 2, 2, PIN_NUM_CS1);
    

    hspi->setFrequency(10000000);
}
