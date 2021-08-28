#include "spi.h"
#include "delay.h" 


extern SPI_HandleTypeDef hspi2;
//SPIx 读写一个字节
//TxData:要写入的字节
//返回值:读取到的字节
u8 ADS_SPI(u8 TxData)
{		
	u8 rx=0;				 	
	HAL_SPI_TransmitReceive(&hspi2,&TxData,&rx,1,0xff);

//	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET) //检查指定的SPI标志位设置与否:发送缓存空标志位
//		{
//		retry++;
//		if(retry>200)return 0;
//		}			  
//	SPI_I2S_SendData(SPI1, TxData); //通过外设SPIx发送一个数据
//	retry=0;

//	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET) //检查指定的SPI标志位设置与否:接受缓存非空标志位
//		{
//		retry++;
//		if(retry>200)return 0;
//		}	  						    
//	return SPI_I2S_ReceiveData(SPI1); //返回通过SPIx最近接收的数据				

		return rx;
}

