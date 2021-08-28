//#include "stdef.h"
#include "delay.h"
#include "ADS1x9x.h"
#include "spi.h"
#include "usart.h"

#include "ads1299.h"


typedef struct 
{
    uint8_t id ;            
    uint8_t cfg1 ;          /* ���üĴ��� */
    uint8_t cfg2 ;          /* ���üĴ��� */
	  uint8_t cfg3 ;          /* ���üĴ��� */
    uint8_t loff ;          /* ���� */
    uint8_t ch1set ;        /* ͨ��һ���� */
    uint8_t ch2set ;        /* ͨ�������� */
	  uint8_t ch3set ;        /* ͨ��һ���� */
    uint8_t ch4set ;        /* ͨ�������� */
	  uint8_t ch5set ;        /* ͨ��һ���� */
    uint8_t ch6set ;        /* ͨ�������� */
	  uint8_t ch7set ;        /* ͨ��һ���� */
    uint8_t ch8set ;        /* ͨ�������� */
		uint8_t rld_sensp ;
		uint8_t rld_sensn ;
		uint8_t loff_sensp ;
    uint8_t loff_sensn ;
    uint8_t loff_fifp ;
    uint8_t loff_statp ;
    uint8_t loff_statn ;
    uint8_t gpio ;	
    uint8_t pace ;
    uint8_t resp ;
    uint8_t cfg4 ;
    uint8_t wct ;
		uint8_t wct2 ;
}ads1298_regs_s ;


static ads1298_regs_s ads1298_reg = {        /* Ĭ�ϲ��� */
                                0x92,   /* Ĭ�ϵ�ַ0x73 */
                                0x86,   /* ����ת����500sps������ */
                                0x00,   
                                0xCC,   
                                0x02,   
                                0x00,  
                                0x00,   
                                0x00,  
                                0x10,   
                                0x00,   
                                0x10,  
                                0x00,
	                              0x00,  
                                0xFF,   
                                0xFF,   
                                0x00,  
                                0x00,
	                              0x00,   
                                0x00,  
                                0x00,
	                              0x00,  
                                0x00,   
                                0x00,   
                                0x00,  
                                0x00,
																0X00
                                } ;

ads1298_regs_s ads1298_reg1;







//void ADS_READ_DATA(u8 *add)	
//{			  
//	
//	  u8 res;
//		while( ADS1298_DRDY_GET());//���ݽ����ж�				
//			
//        ADS1298_CS_RESET() ;
//				for (res=0;res<27;res++)
//					{
//						*add=ADS_SPI(0XAA);
//						add++;
//					}//���ݴ浽27�ֽڻ�����

//        ADS1298_CS_SET() ;
//}



void ADS_PowerOnInit(ads1299_t *pads1299)//ADS1298�ϵ縴λ
	{	
		//ADS1298_LED_SET();
		//ADS1298_RESET1_RESET();
		pads1299->SetReset(0);
		
		//ADS1298_PWDN_RESET();
		//ADS1298_CLKSEL_SET();//�����ڲ�ʱ��
		//ADS1298_PWDN_SET(); 
	//	ADS1298_RESET1_SET(); 

		ads1299->SetReset(1);
    ADS1298_CS_SET() ;
    delay_ms(100) ;    /* Ӳ����λ */
    
	  ads1298_send_cmd(SDATAC) ;    /* �����λ����ֹͣ������״̬ */
    delay_ms(100) ;
    ads1298_send_cmd(ADS_RESET1) ;
    delay_ms(1000) ;
    ads1298_send_cmd(SDATAC) ;
		delay_ms(100);
		ADS1298_START_RESET();
    delay_ms(200) ;
		ads1298_write_multiregs( ADS_REG_ID, (uint8_t *)&ads1298_reg, 26) ;
    /* д������ */
    delay_ms(100) ;	
	  ads1298_read_multiregs( ADS_REG_ID, (uint8_t *)&ads1298_reg, 26);	
		ads1298_send_cmd(START);
		delay_ms(10);
		ADS1298_START_SET();
    delay_ms(10) ;

    ads1298_send_cmd(RDATAC) ; /* ��������ģʽ */

	}


	





/* ��ָ���Ĵ�����ʼ��дһ�������ļĴ��� */
void ads1298_write_multiregs( uint8_t reg, uint8_t *ch, uint8_t size)
{
    uint8_t i ;

    ADS1298_CS_RESET() ;    /* ѡ���豸 */	
    delay_us(100) ;

    ADS_SPI( WREG | reg) ;
    delay_us(100);
    ADS_SPI(size - 1) ;
    
    for( i = 0; i < size; i++)
    {
        delay_us(100) ;		
        ADS_SPI(*ch) ;
        ch++ ;				
    }			
    
    delay_us(100) ;	
   	ADS1298_CS_SET() ;   
}

/* ��ָ���Ĵ�����ʼ��дһ�������ļĴ��� */
void ads1298_read_multiregs( uint8_t reg, uint8_t *ch, uint8_t size)
{
    uint8_t i ;

   ADS1298_CS_RESET() ;    /* ѡ���豸 */	
    delay_us(100) ;

    ADS_SPI( RREG | reg) ;
    delay_us(100);
    ADS_SPI(size - 1) ;
    
    for( i = 0; i < size; i++)
    {
			delay_us(100) ;		
			*ch = ADS_SPI(0) ;  
			//USART_SendData(USART1, *ch);
			//ch++;
			//while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);
    }
    
    delay_us(100) ;	
	ADS1298_CS_SET() ;  
}


uint8_t ads1298_send_cmd( uint8_t cmd)
{
    uint8_t rx = 0 ;
    
	ADS1298_CS_RESET() ;    /* ѡ���豸 */   

	delay_us(100) ;
    
	rx = ADS_SPI(cmd) ;
	
    
	delay_us(100) ;
	ADS1298_CS_SET() ;      /* �ͷ��豸 */
    
    return rx ;
}
