#ifndef _APP_H
#define _APP_H	  

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/** @defgroup My_App My App
  * @{
  */
/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/
/** @addtogroup My_App_Exported_Functions My App Exported Functions 
  * @{
  */

/** @brief  应用程序初始化函数
  * @details 初始化顶层应用程序
  * @retval None
  */
void my_app_init(void) ;

/** @brief  应用程序
  * @details 顶层应用程序
  * @retval None
  */
void my_app(void) ; 

/**
  * @}
  */
  
/**
  * @}
  */

#ifdef __cplusplus
}
#endif
  
#endif /* __MY_APP_H */


