
#ifndef _ADS1x9x_H_
#define _ADS1x9x_H_


#include "spi.h"

#include "delay.h"


#define ADS_DRDY		PAin(8)
#define ADS_CS			PBout(12)		
#define ADS_RESET 	PAout(0)
#define ADS_START		PAout(2)	
#define ADS_CLKSEL	PAout(3)	

/////////////////////////////////////////////////////////////////////////////////////////////
//声明变量
//extern	volatile u8 ads1298_Cache[27];		//ads1298数据缓存
//extern  volatile u8 ads1298_recive_flag;	//数据读取完成标志


/*ADS1298命令定义*/
	/*系统命令*/
	#define WAKEUP		0X02	//从待机模式唤醒
	#define STANDBY		0X04	//进入待机模式
	#define ADS_RESET1	0X06	//复位ADS1298
	#define START			0X08	//启动或转换
	#define STOP			0X0A	//停止转换
	//#define OFFSETCAL	0X1A	//通道偏移校准
	/*数据读取命令*/
	#define RDATAC		0X10	//启用连续的数据读取模式,默认使用此模式
	#define SDATAC		0X11	//停止连续的数据读取模式
	#define RDATA			0X12	//通过命令读取数据;支持多种读回。
	/*寄存器读取命令*/
	#define	RREG			0X20	//读取001r rrrr 000n nnnn  这里定义的只有高八位，低8位在发送命令时设置
	#define WREG			0X40	//写入010r rrrr 000n nnnn
	/*	r rrrr=要读、写的寄存器地址
			n nnnn=要读、写的数据*/
/*ADS1292内部寄存器地址定义*/
	#define ID				0X00	//ID控制寄存器
	#define CONFIG1		0X01	//配置寄存器1
	#define CONFIG2		0X02	//配置寄存器2
	#define CONFIG3 	0X03	//配置寄存器3
	#define LOFF			0X04	//导联脱落控制寄存器
	#define CH1SET		0X05	//通道1设置寄存器
	#define CH2SET		0X06	//通道2设置寄存器
	#define CH3SET		0X07	//通道3设置寄存器
	#define CH4SET		0X08	//通道4设置寄存器
	#define CH5SET		0X09	//通道5设置寄存器
	#define CH6SET		0X0A	//通道6设置寄存器
	#define CH7SET		0X0B	//通道7设置寄存器
	#define CH8SET		0X0C	//通道8设置寄存器
	#define	BIAS_SENSP	0X0D//
	#define	BIAS_SENSN	0X0E
	#define	LOFF_SENSP	0X0F
	#define	LOFF_SENSN	0X10
	#define	LOFF_FLIP		0X11
	
	#define	LOFF_STATP	0X12
	#define	LOFF_STATN	0X13//掉电状态寄存器地址（只读）
	
	#define	GPIO				0X14	//GPIO控制寄存器
	#define	PACE				0X15
	#define	RESP				0X16
	#define	CONFIG4			0X17
	#define	WCT   			0X18
	
	

unsigned char ADS_SPI(unsigned char com);//通过SPI总线与ADS1298通信
unsigned char ADS_REG(unsigned char com,unsigned data);//对ADS1298内部寄存器进行操作
void ADS_PowerOnInit(void);//ADS1298上电复位
void ADS_READ_DATA(u8 *data);//读取数据
//unsigned char ADS_SPI_Read(void);
void ADS_Init(void); //初始化ADS1298
void EXTI9_5_IRQHandler(void);

#endif
