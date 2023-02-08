#ifndef _DISPLAY_H
#define _DISPLAY_H

#include "stm32f4xx_hal.h"
#include <stdlib.h>
#include <string.h>

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

struct ILI9341{
	SPI_HandleTypeDef* SPIHandle;
	UART_HandleTypeDef* UARTHandle;
	uint16_t x_size;
	uint16_t y_size;
	uint16_t* buffer;
};


struct ILI9341* ILI9341_Create(SPI_HandleTypeDef*, UART_HandleTypeDef*, uint16_t, uint16_t);
void ILI9341_Destroy(struct ILI9341*);
void SPI_Write_8(struct ILI9341*, uint8_t);
void SPI_Write_16(struct ILI9341*, uint16_t);
void ILI9341_Write_Command(struct ILI9341*, uint8_t);
void ILI9341_Init(struct ILI9341*);
void Set_Display_Rotation(struct ILI9341*, uint8_t);
void Set_Address(struct ILI9341*, uint16_t, uint16_t, uint16_t, uint16_t);
void Set_Address_Read(struct ILI9341*, uint16_t, uint16_t, uint16_t, uint16_t);
void Set_Address_Write(struct ILI9341*, uint16_t, uint16_t, uint16_t, uint16_t);
void Fill_Display(struct ILI9341*, uint16_t);
uint16_t RGB_to_HEX(int, int, int); 
void Fill_Area(struct ILI9341*, uint16_t, uint16_t, uint16_t, uint16_t, uint16_t);
void Print_Debug_Message(struct ILI9341*, char*);
uint16_t* Buffer_Area(struct ILI9341 *self, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);

#endif