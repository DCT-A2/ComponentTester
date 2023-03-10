/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "display.h"
#include "notosans.h"
#include "notosans_small.h"
#include "touch.h"
#include "GFX.h"
#include "measure.h"
#include <string.h>

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
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
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
