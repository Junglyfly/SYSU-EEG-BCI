#include "app.h"
#include "ADS1x9x.h"


u8 i ;
uint8_t ad_b[27] ;   /* �洢ԭʼ���� */

	




void my_app_init()
{      
    ads1298_init();
	  ADS_PowerOnInit();	

}


//ADS_READ_DATA(*&ad_b); /* ��ȡԭʼ���� */ 






























