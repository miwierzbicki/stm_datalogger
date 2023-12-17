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
#include <stdbool.h>
#include "ds18b20.h"
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
bool isClicked(void);
//bool debug;
void clearEncButton(void);
void send_uart_uint32(uint32_t value);
void send_uart_float(float value);
void send_uart(char *string);
char* getRtcString(void);
extern RTC_TimeTypeDef time;
extern RTC_DateTypeDef date;
char* getRtcString(void);
extern bool debug;
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define ADC1_IN0_INT_Pin GPIO_PIN_0
#define ADC1_IN0_INT_GPIO_Port GPIOA
#define ADC1_IN1_INT_Pin GPIO_PIN_1
#define ADC1_IN1_INT_GPIO_Port GPIOA
#define ADC1_IN2_INT_Pin GPIO_PIN_2
#define ADC1_IN2_INT_GPIO_Port GPIOA
#define ADC1_IN3_INT_Pin GPIO_PIN_3
#define ADC1_IN3_INT_GPIO_Port GPIOA
#define CS_SD_Pin GPIO_PIN_4
#define CS_SD_GPIO_Port GPIOA
#define SCK_SD_Pin GPIO_PIN_5
#define SCK_SD_GPIO_Port GPIOA
#define MISO_SD_Pin GPIO_PIN_6
#define MISO_SD_GPIO_Port GPIOA
#define MOSI_SD_Pin GPIO_PIN_7
#define MOSI_SD_GPIO_Port GPIOA
#define DSO_DATA_Pin GPIO_PIN_1
#define DSO_DATA_GPIO_Port GPIOB
#define DET_SD_Pin GPIO_PIN_2
#define DET_SD_GPIO_Port GPIOB
#define SCL_ADS_Pin GPIO_PIN_10
#define SCL_ADS_GPIO_Port GPIOB
#define LED2_Pin GPIO_PIN_12
#define LED2_GPIO_Port GPIOB
#define LED1_Pin GPIO_PIN_13
#define LED1_GPIO_Port GPIOB
#define ENC_A_Pin GPIO_PIN_8
#define ENC_A_GPIO_Port GPIOA
#define ENC_B_Pin GPIO_PIN_9
#define ENC_B_GPIO_Port GPIOA
#define LED5_Pin GPIO_PIN_15
#define LED5_GPIO_Port GPIOA
#define LED4_Pin GPIO_PIN_3
#define LED4_GPIO_Port GPIOB
#define LED3_Pin GPIO_PIN_4
#define LED3_GPIO_Port GPIOB
#define SCL_OLED_Pin GPIO_PIN_6
#define SCL_OLED_GPIO_Port GPIOB
#define SDA_OLED_Pin GPIO_PIN_7
#define SDA_OLED_GPIO_Port GPIOB
#define ENC_BTN_Pin GPIO_PIN_8
#define ENC_BTN_GPIO_Port GPIOB
#define ENC_BTN_EXTI_IRQn EXTI9_5_IRQn
#define SDA_ADS_Pin GPIO_PIN_9
#define SDA_ADS_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */
#define SD_SPI_HANDLE hspi1
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
