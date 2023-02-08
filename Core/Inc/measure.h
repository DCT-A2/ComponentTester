#include "stm32f4xx_hal.h"

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
#define R1_680R_Pin GPIO_PIN_3
#define R1_680R_GPIO_Port GPIOD
#define R2_470k_Pin GPIO_PIN_4
#define R2_470k_GPIO_Port GPIOD
#define R3_680R_Pin GPIO_PIN_5
#define R3_680R_GPIO_Port GPIOD
#define R4_470k_Pin GPIO_PIN_6
#define R4_470k_GPIO_Port GPIOD
#define ADC1_Input_Pin GPIO_PIN_0
#define ADC2_Input_Pin GPIO_PIN_1
#define ADC_Input_Port GPIOA

uint16_t Measure_Resistor(ADC_HandleTypeDef*, ADC_HandleTypeDef*);