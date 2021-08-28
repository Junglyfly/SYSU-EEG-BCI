
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
//��������
//extern	volatile u8 ads1298_Cache[27];		//ads1298���ݻ���
//extern  volatile u8 ads1298_recive_flag;	//���ݶ�ȡ��ɱ�־


/*ADS1298�����*/
	/*ϵͳ����*/
	#define WAKEUP		0X02	//�Ӵ���ģʽ����
	#define STANDBY		0X04	//�������ģʽ
	#define ADS_RESET1	0X06	//��λADS1298
	#define START			0X08	//������ת��
	#define STOP			0X0A	//ֹͣת��
	//#define OFFSETCAL	0X1A	//ͨ��ƫ��У׼
	/*���ݶ�ȡ����*/
	#define RDATAC		0X10	//�������������ݶ�ȡģʽ,Ĭ��ʹ�ô�ģʽ
	#define SDATAC		0X11	//ֹͣ���������ݶ�ȡģʽ
	#define RDATA			0X12	//ͨ�������ȡ����;֧�ֶ��ֶ��ء�
	/*�Ĵ�����ȡ����*/
	#define	RREG			0X20	//��ȡ001r rrrr 000n nnnn  ���ﶨ���ֻ�и߰�λ����8λ�ڷ�������ʱ����
	#define WREG			0X40	//д��010r rrrr 000n nnnn
	/*	r rrrr=Ҫ����д�ļĴ�����ַ
			n nnnn=Ҫ����д������*/
/*ADS1292�ڲ��Ĵ�����ַ����*/
	#define ID				0X00	//ID���ƼĴ���
	#define CONFIG1		0X01	//���üĴ���1
	#define CONFIG2		0X02	//���üĴ���2
	#define CONFIG3 	0X03	//���üĴ���3
	#define LOFF			0X04	//����������ƼĴ���
	#define CH1SET		0X05	//ͨ��1���üĴ���
	#define CH2SET		0X06	//ͨ��2���üĴ���
	#define CH3SET		0X07	//ͨ��3���üĴ���
	#define CH4SET		0X08	//ͨ��4���üĴ���
	#define CH5SET		0X09	//ͨ��5���üĴ���
	#define CH6SET		0X0A	//ͨ��6���üĴ���
	#define CH7SET		0X0B	//ͨ��7���üĴ���
	#define CH8SET		0X0C	//ͨ��8���üĴ���
	#define	BIAS_SENSP	0X0D//
	#define	BIAS_SENSN	0X0E
	#define	LOFF_SENSP	0X0F
	#define	LOFF_SENSN	0X10
	#define	LOFF_FLIP		0X11
	
	#define	LOFF_STATP	0X12
	#define	LOFF_STATN	0X13//����״̬�Ĵ�����ַ��ֻ����
	
	#define	GPIO				0X14	//GPIO���ƼĴ���
	#define	PACE				0X15
	#define	RESP				0X16
	#define	CONFIG4			0X17
	#define	WCT   			0X18
	
	

unsigned char ADS_SPI(unsigned char com);//ͨ��SPI������ADS1298ͨ��
unsigned char ADS_REG(unsigned char com,unsigned data);//��ADS1298�ڲ��Ĵ������в���
void ADS_PowerOnInit(void);//ADS1298�ϵ縴λ
void ADS_READ_DATA(u8 *data);//��ȡ����
//unsigned char ADS_SPI_Read(void);
void ADS_Init(void); //��ʼ��ADS1298
void EXTI9_5_IRQHandler(void);

#endif
