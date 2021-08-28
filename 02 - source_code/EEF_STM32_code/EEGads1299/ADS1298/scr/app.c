#include "app.h"
#include "ADS1x9x.h"


u8 i ;
uint8_t ad_b[27] ;   /* 存储原始数据 */

	




void my_app_init()
{      
    ads1298_init();
	  ADS_PowerOnInit();	

}


//ADS_READ_DATA(*&ad_b); /* 获取原始数据 */ 






























