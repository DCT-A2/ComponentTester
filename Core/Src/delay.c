#include "delay.h"

TIM_HandleTypeDef *htim = NULL;

void Setup_Delay(TIM_HandleTypeDef *htimIN){
	htim = htimIN;
}

void Delay_us(uint32_t delay){
	__HAL_TIM_SET_COUNTER(htim, 0);
	while(__HAL_TIM_GET_COUNTER(htim) < (delay));
	
}