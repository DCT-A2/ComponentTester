#ifndef _DELAY_H
#define _DELAY_H

#include "stm32f4xx_hal.h"

void Setup_Delay(TIM_HandleTypeDef *htimIN);
void Delay_us(uint32_t delay);

#endif