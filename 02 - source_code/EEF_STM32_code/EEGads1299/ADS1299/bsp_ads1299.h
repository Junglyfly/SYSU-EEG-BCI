#ifndef _BSP_ADS1299_H
#define _BSP_ADS1299_H

#include "stm32h7xx.h"
#include "core_cm7.h"
#include "stm32h7xx_hal.h"
#include "sys.h"
#include "ads1299.h"


void vADS1299_Init(ads1299_t *pADS1299_1_t);


extern SPI_HandleTypeDef hspi2;
extern ads1299_t xADS1299_1_t ;
#endif


