#include "measure.h"

HAL_StatusTypeDef Setup_ADC(ADC_HandleTypeDef *ADCx, uint32_t ADC_channel, ADC_TypeDef *ADC_Instance)
{
	Pin_Mode_HighZ(ADC_Input_Port, (ADC1_Input_Pin | ADC2_Input_Pin));
	ADC_ChannelConfTypeDef sConfig = {0};
	ADCx->Instance = ADC_Instance;
	ADCx->Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
	ADCx->Init.Resolution = ADC_RESOLUTION_12B;
	ADCx->Init.ScanConvMode = DISABLE;
	ADCx->Init.ContinuousConvMode = DISABLE;
	ADCx->Init.DiscontinuousConvMode = DISABLE;
	ADCx->Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
	ADCx->Init.ExternalTrigConv = ADC_SOFTWARE_START;
	ADCx->Init.DataAlign = ADC_DATAALIGN_RIGHT;
	ADCx->Init.NbrOfConversion = 1;
	ADCx->Init.DMAContinuousRequests = DISABLE;
	ADCx->Init.EOCSelection = ADC_EOC_SINGLE_CONV;
	HAL_StatusTypeDef result = HAL_ADC_Init(ADCx);
	if (result != HAL_OK)
	{
		return result;
	}
	sConfig.Channel = ADC_channel;
	sConfig.Rank = 1;
	sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
	result = HAL_ADC_ConfigChannel(ADCx, &sConfig);
	return result;
}

uint16_t Measure_Resistor(ADC_HandleTypeDef *ADC_R, ADC_HandleTypeDef *ADC_L)
{
	Pin_Mode_HighZ(ADC_Input_Port, ADC1_Input_Pin);
	HAL_StatusTypeDef result = HAL_ADC_DeInit(ADC_L);
	if (result != HAL_OK)
	{
		return 0;
	}
	result = Setup_ADC(ADC_R, ADC_CHANNEL_0, ADC1);
	if (result != HAL_OK)
	{
		return 0;
	}
	result = HAL_ADC_Start(ADC_R);
	if (result != HAL_OK)
	{
		return 0;
	}
	Pin_Mode_HighZ(GPIOD, (R1_680R_Pin | R3_680R_Pin | R4_470k_Pin));
	Pin_Mode_Output(GPIOD, R2_470k_Pin);
	Pin_Mode_Output(ADC_Input_Port, ADC2_Input_Pin);
	HAL_GPIO_WritePin(ADC_Input_Port, ADC2_Input_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(R2_470k_GPIO_Port, R2_470k_Pin, GPIO_PIN_SET);
	HAL_ADC_PollForConversion(ADC_R, HAL_MAX_DELAY);
	uint16_t res = HAL_ADC_GetValue(ADC_R);
	HAL_ADC_Stop(ADC_R);
	return res;
}

void Pin_Mode_HighZ(GPIO_TypeDef *GPIOx, uint32_t pin)
{
	// Sets pin to analog mode to make sure its floating and doesn't have a meaningful effect on the measurement.
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = pin;
	GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
}

void Pin_Mode_Output(GPIO_TypeDef *GPIOx, uint32_t pin)
{
	// Set pin to output.
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
}