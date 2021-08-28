#include "bsp_ads1299.h"
#include "stdio.h"
#include "main.h"
#include "delay.h"

/*******************  ADS1299--1   ******************************/  

void vSet_ADS1299_1_CS(uint8_t state)
{
	HAL_GPIO_WritePin(Ads_1_SPI_CS_GPIO_Port,Ads_1_SPI_CS_Pin,(GPIO_PinState)state);
}

//hardware has connected DVDD
void vSet_ADS1299_1_PWDN(uint8_t state)
{
	//state ? HAL_GPIO_WritePin(GPIO_P) : HAL_GPIO_WritePin() ;
}

void vSet_ADS1299_1_Start(uint8_t state)
{
	HAL_GPIO_WritePin(Ads_1_START_GPIO_Port,Ads_1_START_Pin,(GPIO_PinState)state);
}

void vSet_ADS1299_1_Reset(uint8_t state)
{
	HAL_GPIO_WritePin(Ads_1_RESET_GPIO_Port,Ads_1_RESET_Pin,(GPIO_PinState)state);
}

void vADS1299_1_Transfer(uint8_t tx[], uint8_t rx[], uint16_t len)
{
	//HAL_GPIO_WritePin();
	HAL_SPI_TransmitReceive(&hspi2,tx,rx,len,0xff);
}

uint8_t ucReceiveBuf[30];

//ADS1299--1 controling variable 
ads1299_t xADS1299_1_t = {
					.DelayMs = delay_ms,
					.DelayUs = delay_us,
					.Transfer = vADS1299_1_Transfer,
					.SetCS = vSet_ADS1299_1_CS,
					.SetReset = vSet_ADS1299_1_Reset,
					.SetStart = vSet_ADS1299_1_Start,
					.SetPWDN = vSet_ADS1299_1_PWDN
};



//ADS1299 initial function
void vADS1299_Init(ads1299_t *pADS1299)
{	
//		delay_ms(300);
//	pADS1299->SetReset(0);
//	pADS1299->SetPWDN(0);
//	delay_ms(1000);
//	pADS1299->SetReset(1);
//	pADS1299->SetPWDN(1);

//	delay_ms(100);
//	ADS1299_DisableContRead(pADS1299); 
//	delay_ms(100);
//	ADS1299_Reset(pADS1299);
//	delay_ms(1000);
//	ADS1299_DisableContRead(pADS1299); 
//	pADS1299->SetStart(0);
//	delay_ms(200);
//	//ADS1299_WriteReg();
//	ADS1299_StartAdc(pADS1299);
//	delay_ms(10);
//	pADS1299->SetStart(1);
//	ADS1299_StartAdc(pADS1299);
//	ADS1299_EnableContRead(pADS1299);  //trun on continual adc conversation

 	pADS1299->DelayMs(300);
 	pADS1299->SetReset(0);
 	pADS1299->SetPWDN(0);
 	pADS1299->DelayMs(300);   //Set CLKSEL Pin = 1 and Wait for Oscillator to Wake Up
 	pADS1299->SetReset(1);
 	pADS1299->SetPWDN(1);
 	pADS1299->DelayMs(100);

 	ADS1299_DisableContRead(pADS1299);  //Send SDATAC 
 	delay_ms(100);
 	ADS1299_Reset(pADS1299);
 	pADS1299->SetStart(0);
	ADS1299_StartAdc(pADS1299);
 	delay_ms(100);
	ADS1299_EnableContRead(pADS1299); //RDATAC
 	pADS1299->SetStart(1);

 // 	//Write Certain Registers, Including Input Short
 
}



// ads1299_WREG(0x01,0x01,Data);
// Data[0]=0x0100;Data[1]=0x0100;Data[2]=0x0100;Data[3]=0x0100;Data[4]=0x0100;Data[5]=0x0100;Data[6]=0x0100;Data[7]=0x0100;
// ads1299_WREG(0x05,0x07,Data);
// Data[0]=0x2000;Data[1]=0x0000;
// ads1299_WREG(0x15,0x01,Data);


// ADS1299 DRAY Processing function
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	static uint32_t data = 0 ;
	if(GPIO_Pin == GPIO_PIN_0) //PA0
	{
		xADS1299_1_t.ucDataDRDY = 0x01;  //标记接收到数据
		//data = ADS1299_ReadAdc(&xADS1299_1_t);
		printf(" PA0 %d \n",data++);
	}
}

