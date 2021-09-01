#include "bsp_ads1299.h"

// ADS pin define
#define ADS_DRDY     35
#define ADS_CLKSEL   32
#define ADS_START    33
#define ADS_PWDN     25
#define ADS_RESET    26
#define ADS_CS       27
#define ADS_SCLK     14
#define ADS_MISO     12
#define ADS_MOSI     13


/* ADS1299 Commands */
#define ADS1299_WAKEUP_CMD               (0x02)
#define ADS1299_STANDBY_CMD              (0x04)
#define ADS1299_RESET_CMD                (0x06)
#define ADS1299_START_CMD                (0x08)
#define ADS1299_STOP_CMD                 (0x0A)
#define ADS1299_RDATAC_CMD               (0x10)
#define ADS1299_SDATAC_CMD               (0x11)
#define ADS1299_RDATA_CMD                (0x12)
#define ADS1299_READ_REG_CMD             (0x20)
#define ADS1299_WRITE_REG_CMD            (0x40)


#define ADS_1299DEBUG(...)   Serial.printf(__VA_ARGS__)


SPIClass *pHspi = NULL;
uint8_t ucADS_spi_rxbuf[32] = {0};
uint8_t ucADS_spi_txbuf[32] = {0};  //spi操作寄存器使用,仅用于SPI直接操作的函数使用


uint8_t ucADS_Data_RxBuf[30];

void vAds1299_spi_write_read(uint8_t *write_buf, uint8_t *read_buf, uint8_t len)
{
    for (uint8_t i = 0; i < len; i++)
    {
        read_buf[i] = pHspi->transfer(write_buf[i]);
    }
}



void vADS1299_read_register(uint8_t addr, uint8_t *buffer, uint8_t len, uint8_t cs)
{
    ucADS_spi_txbuf[0] = 0x20 + addr;   //0010 0000
    ucADS_spi_txbuf[1] = 0x00 + len - 1;
    memset(ucADS_spi_txbuf + 2,0xff,len);
    digitalWrite(cs, LOW);
    vAds1299_spi_write_read(ucADS_spi_txbuf, ucADS_spi_rxbuf, 2);
    vAds1299_spi_write_read(ucADS_spi_txbuf+2, buffer, len);
    digitalWrite(cs, HIGH);
}


void vADS1299_write_register(uint8_t addr, uint8_t *buffer, uint8_t len, uint8_t cs)
{
    ucADS_spi_txbuf[0] = 0x40 + addr;  //0100 0000
    ucADS_spi_txbuf[1] = 0x00 + len - 1;
    digitalWrite(cs, LOW);
    vAds1299_spi_write_read(ucADS_spi_txbuf, ucADS_spi_rxbuf, 2);
    vAds1299_spi_write_read(buffer, ucADS_spi_rxbuf, len);
    digitalWrite(cs, HIGH);
}


void ADS129x_Send_CMD(uint8_t *cmd, uint8_t len, uint8_t cs)
{
    digitalWrite(cs, LOW);
    vAds1299_spi_write_read(cmd, ucADS_spi_rxbuf, len);
    digitalWrite(cs, HIGH);
}



void vADS_ControlPin_Init()
{
    uint8_t tmpcmd[2] = {ADS1299_SDATAC_CMD, 0x00};
    pinMode(ADS_DRDY,INPUT_PULLUP);
    pinMode(ADS_CLKSEL,OUTPUT);
    pinMode(ADS_START,OUTPUT);
    pinMode(ADS_PWDN,OUTPUT);
    pinMode(ADS_RESET,OUTPUT);

    pHspi = new SPIClass(HSPI);
    pHspi->begin(ADS_SCLK,ADS_MISO,ADS_MOSI,-1);  //CS 引脚不使用SPI控制使用软件控制
    pHspi->setBitOrder(MSBFIRST);
    pHspi->setFrequency(200000); //10Mhz
    pHspi->setDataMode(SPI_MODE1);
    pHspi->setHwCs(false);
    pinMode(ADS_CS,OUTPUT);
    digitalWrite(ADS_CS,HIGH);

    vTaskDelay(100);
    // configure  the ADS1299 chip


    digitalWrite(ADS_PWDN,LOW);
    digitalWrite(ADS_RESET,LOW);
    digitalWrite(ADS_CLKSEL,LOW);  //ADS CLKSEL
    vTaskDelay(1000);

    digitalWrite(ADS_PWDN,HIGH);
    digitalWrite(ADS_RESET,HIGH);
    vTaskDelay(200);

    tmpcmd[0] = ADS1299_RESET_CMD;
    ADS129x_Send_CMD(tmpcmd, 2, ADS_CS);
    vTaskDelay(10);

    tmpcmd[0] = ADS1299_SDATAC_CMD;
    ADS129x_Send_CMD(tmpcmd, 2, ADS_CS);
    vTaskDelay(10);

    //configuration res
    // tmpcmd[0] = 0x03<<5 | 0x01<<3 | 1<<0;
    // vADS1299_write_register(0x03,tmpcmd,2,ADS_CS);  //1 : BIASREF signal (AVDD + AVSS) / 2 generated internally
    // vTaskDelay(10);



    // // tmpcmd[0] = 0xD0;
    // // vADS1299_write_register(0x02,tmpcmd,2,ADS_CS);  //1 : test signal generate in internal
    // // vTaskDelay(10);



    const uint8_t ucCHnSET_Adress[8] = {0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C};
    //configuration CHnSET: Individual Channel Settings Register
    for(uint8_t i = 0 ; i < 8 ; i ++)
    {
        tmpcmd[0] = 0x00 | 0x04<<4 ;// | 1<<3; // PGA set to 8  //将INP连接至SRB2
        vADS1299_write_register(ucCHnSET_Adress[i],tmpcmd,2,ADS_CS); 
        vTaskDelay(10);
    }



    uint8_t ucRx_ID[1];
    vADS1299_read_register(0x00,ucRx_ID,1,ADS_CS);
    ADS_1299DEBUG("ID %x \n",ucRx_ID[0]&0x03);



    tmpcmd[0] = ADS1299_SDATAC_CMD;
    ADS129x_Send_CMD(tmpcmd, 2, ADS_CS);
    vTaskDelay(10);
    tmpcmd[0] = ADS1299_START_CMD;
    ADS129x_Send_CMD(tmpcmd, 2, ADS_CS);
    vTaskDelay(10);
    tmpcmd[0] = ADS1299_RDATAC_CMD;
    ADS129x_Send_CMD(tmpcmd, 2, ADS_CS);  //turn on continual convention

    digitalWrite(ADS_START,HIGH);


    // tmpcmd[0] = 0xD0;
    // vADS1299_write_register(0x02,tmpcmd,2,ADS_CS);  //1 : test signal generate in internal
    // vTaskDelay(10);






}





//读取ADS1299数据
void vADS1299_Receive_Data(uint8_t *read_buf, uint8_t len)
{
    digitalWrite(ADS_CS, LOW);
    pHspi->transfer(ADS1299_RDATA_CMD);
    for (uint8_t i = 0; i < len; i++)
    {
        read_buf[i] = pHspi->transfer(0x00);
    }
    digitalWrite(ADS_CS, HIGH);

}




int32_t iADS_EEG_Data[8] = {0};



//　负数的补码转换为源码：符号位不变，数值位按位取反，末位加一。
void vDecodingDat_Upload(uint8_t OriginalData[])
{
    int32_t iDecodingData[8];
    int32_t iDateTemp = 0x00;
    uint8_t j = 0;
    for(uint8_t i = 3 ; i < 27 ; i += 3)
    {

        iDateTemp = OriginalData[i]<<16 | OriginalData[i+1]<<8 | OriginalData[i+2];


        iDateTemp ^= 0x00800000;

        // if(iDateTemp & 1 <<23)
        //  {
        //     iDateTemp = ~iDateTemp;  // 符号位不变其余为按位取反
        //     iDateTemp++;
        //     iDateTemp &= 0x007FFFFF;  //
        //     iDateTemp *= -1;
        // }

        iDecodingData[j++] = iDateTemp;

        // ADS_1299DEBUG(" %x ",iDateTemp);
    }


        // ADS_1299DEBUG("CH0,%d;CH1,%d;Ch2,%d;CH3,%d;CH4,%d;CH5,%d;CH6,%d;CH7,%d;",iDecodingData[0],iDecodingData[1],iDecodingData[2],iDecodingData[3],\
        //                                                                         iDecodingData[4],iDecodingData[5],iDecodingData[6],iDecodingData[7]);

        ADS_1299DEBUG("CH3,%d",iDecodingData[2]);
        ADS_1299DEBUG("\n");
}



void vOriginalData_Upload(uint8_t OriginalData[])
{

    uint8_t i;
    if(OriginalData[0] == 0xc0)
    {
        for( i = 0 ; i <27; i++)   // i < 27
        {
            ADS_1299DEBUG("%x,",OriginalData[i]);
        }
        ADS_1299DEBUG("\n");
    }
    else
    {
        ADS_1299DEBUG("None\n");
    }


}

void vADS1299_CaptureData_task(void * parameter)
{
    vADS_ControlPin_Init();  //ADS1299 Init
    ADS_1299DEBUG("ADS init ok");
    uint8_t ucReadFlg = 0x00;
    uint32_t cent = 0;
    while (1)
    {
        if((digitalRead(ADS_DRDY)==0) && (ucReadFlg==0x01))
        {
            vADS1299_Receive_Data(ucADS_Data_RxBuf,28);
            vOriginalData_Upload(ucADS_Data_RxBuf);
            ucReadFlg = 0x00;
        }
        else
        {
            ucReadFlg = 0x01;
        }

        if(cent++%40 == 0)
        {
          //  ADS_1299DEBUG("None %d \n",cent);
        }

        vTaskDelay(10);
    }

}
