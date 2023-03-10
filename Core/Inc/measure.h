#ifndef _MEASURE_H
#define _MEASURE_H

#include "stm32f4xx_hal.h"
#include "delay.h"

#define SPI2_TS_IRQ_Pin GPIO_PIN_0
#define SPI2_TS_IRQ_GPIO_Port GPIOC
#define SPI2_TS_CS_Pin GPIO_PIN_1
#define SPI2_TS_CS_GPIO_Port GPIOC
#define SPI2_TS_MISO_Pin GPIO_PIN_2
#define SPI2_TS_MISO_GPIO_Port GPIOC
#define SPI2_TS_MOSI_Pin GPIO_PIN_3
#define SPI2_TS_MOSI_GPIO_Port GPIOC
#define SPI_TFT_CS_Pin GPIO_PIN_2
#define SPI_TFT_CS_GPIO_Port GPIOA
#define SPI_TFT_DC_Pin GPIO_PIN_3
#define SPI_TFT_DC_GPIO_Port GPIOA
#define SPI_TFT_RESET_Pin GPIO_PIN_4
#define SPI_TFT_RESET_GPIO_Port GPIOA
#define SPI1_TFT_SCK_Pin GPIO_PIN_5
#define SPI1_TFT_SCK_GPIO_Port GPIOA
#define SPI1_TFT_MISO_Pin GPIO_PIN_6
#define SPI1_TFT_MISO_GPIO_Port GPIOA
#define SPI1_TFT_MOSI_Pin GPIO_PIN_7
#define SPI1_TFT_MOSI_GPIO_Port GPIOA
#define SPI2_TS_SCK_Pin GPIO_PIN_10
#define SPI2_TS_SCK_GPIO_Port GPIOB
#define R5_Pin GPIO_PIN_0
#define R5_GPIO_Port GPIOD
#define R4_Pin GPIO_PIN_1
#define R4_GPIO_Port GPIOD
#define R3_Pin GPIO_PIN_2
#define R3_GPIO_Port GPIOD
#define R2_Pin GPIO_PIN_3
#define R2_GPIO_Port GPIOD
#define R1_Pin GPIO_PIN_4
#define R1_GPIO_Port GPIOD
#define TP1_Pin GPIO_PIN_5
#define TP1_GPIO_Port GPIOD
#define R6_Pin GPIO_PIN_6
#define R6_GPIO_Port GPIOD
#define ADC1_Input_Pin GPIO_PIN_0
#define ADC2_Input_Pin GPIO_PIN_1
#define ADC_Input_Port GPIOA

#define REF_NUM 6 //Define the number of reference resistors
#define R1 676
#define R2 472200
#define R3 75170
#define R4 100000
#define R5 460
#define R6 1002000

#define R_PARA_GPIO 23
#define RESISTOR_PORT GPIOD
#define OVERSAMPLE_AMOUNT 256
//675R, 471.2k, 200.8k, 100.0k, 9.7M, 495.5R

float Calc_Resistance(uint16_t ref_res, uint16_t direction, uint16_t voltage);
uint16_t Measure_Voltage(ADC_HandleTypeDef *ADC_L);
void Setup_Test(uint16_t ref_res, uint16_t direction);
void Resistance_String(char *buffer, float resistance);
void Pin_Mode_Output(GPIO_TypeDef *GPIOx, uint32_t pin);
void Pin_Mode_HighZ(GPIO_TypeDef *GPIOx, uint32_t pin);
float Measure_Resistor(ADC_HandleTypeDef *ADCx);
float Measure_Capacitor(ADC_HandleTypeDef *ADCx);
#endif