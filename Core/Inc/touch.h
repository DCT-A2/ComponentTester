#ifndef _TOUCH_H
#define _TOUCH_H

#include "stm32f4xx_hal.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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


struct XPT2046{
	uint16_t last_x;
	uint16_t last_y;
	uint16_t width;
	uint16_t height;
	UART_HandleTypeDef *UARTHandle;
	SPI_HandleTypeDef *SPIHandle;
};

struct XPT2046* XPT2046_Create(uint16_t, uint16_t, SPI_HandleTypeDef*, UART_HandleTypeDef*);
void XPT2046_Destroy(struct XPT2046*);
int TS_is_Pressed();
int TS_Get_Touch(struct XPT2046*);
void TS_Chip_Enable();
void TS_Chip_Disable();


#endif
