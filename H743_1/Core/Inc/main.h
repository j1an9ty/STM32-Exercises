/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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
#include "stm32h7xx_hal.h"
#include "usart.h"
#include "gpio.h"


/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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
#define COM_LED_Pin GPIO_PIN_2
#define COM_LED_GPIO_Port GPIOE
#define RUN_LED_Pin GPIO_PIN_3
#define RUN_LED_GPIO_Port GPIOE
#define CON_RELAY1_Pin GPIO_PIN_8
#define CON_RELAY1_GPIO_Port GPIOD
#define CON_RELAY2_Pin GPIO_PIN_9
#define CON_RELAY2_GPIO_Port GPIOD
#define CON_RELAY3_Pin GPIO_PIN_10
#define CON_RELAY3_GPIO_Port GPIOD
#define CON_RELAY4_Pin GPIO_PIN_11
#define CON_RELAY4_GPIO_Port GPIOD
#define CON_RELAY5_Pin GPIO_PIN_12
#define CON_RELAY5_GPIO_Port GPIOD
#define CON_RELAY6_Pin GPIO_PIN_13
#define CON_RELAY6_GPIO_Port GPIOD
#define CON3_485_Pin GPIO_PIN_9
#define CON3_485_GPIO_Port GPIOC
#define IO_IN1_Pin GPIO_PIN_12
#define IO_IN1_GPIO_Port GPIOC
#define IO_IN2_Pin GPIO_PIN_2
#define IO_IN2_GPIO_Port GPIOD
#define IO_IN3_Pin GPIO_PIN_3
#define IO_IN3_GPIO_Port GPIOD
#define IO_IN4_Pin GPIO_PIN_4
#define IO_IN4_GPIO_Port GPIOD
#define IO_IN5_Pin GPIO_PIN_7
#define IO_IN5_GPIO_Port GPIOD
#define IO_IN6_Pin GPIO_PIN_7
#define IO_IN6_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
