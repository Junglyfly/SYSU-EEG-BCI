#include "mainTask.h"
#include "delay.h"
#include "main.h"
#include "stdio.h"
#include "bsp_ads1299.h"


uint32_t u32_adc;

void vMainTak(void)
{
	delay_init(240);
	//uint8_t SPI_TestBuf[3] = {0xA7,0x01,0x05};
	uint8_t data;
	printf("system ok");
	
	vADS1299_Init(&xADS1299_1_t);
	data = ADS1299_ReadReg(&xADS1299_1_t,0x00);
	printf("data = %x",data);
	while(1)
	{
		
		if(xADS1299_1_t.ucDataDRDY==1)
		{
			u32_adc = ADS1299_ReadAdc(&xADS1299_1_t);
			delay_ms(10);
			xADS1299_1_t.ucDataDRDY = 0x00;
		}
		
//		//HAL_SPI_Transmit(&hspi2,SPI_TestBuf,3,0xf);
//			delay_ms(500);
//  		HAL_GPIO_TogglePin(Board_Led_GPIO_Port,Board_Led_Pin);
	}
	
}



