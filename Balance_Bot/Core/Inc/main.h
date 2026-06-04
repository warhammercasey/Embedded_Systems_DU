/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define WS2812_0 29
#define WS2812_1 59
#define MT6701_MIN_PWM 10
#define MT6701_MAX_PWM 42155
#define IMU_INT_Pin GPIO_PIN_13
#define IMU_INT_GPIO_Port GPIOC
#define IMU_INT_EXTI_IRQn EXTI15_10_IRQn
#define WS_DIN_Pin GPIO_PIN_0
#define WS_DIN_GPIO_Port GPIOA
#define MD_PWMA_Pin GPIO_PIN_1
#define MD_PWMA_GPIO_Port GPIOA
#define MD_AIN2_Pin GPIO_PIN_2
#define MD_AIN2_GPIO_Port GPIOA
#define MD_AIN1_Pin GPIO_PIN_3
#define MD_AIN1_GPIO_Port GPIOA
#define MD_STBY_Pin GPIO_PIN_4
#define MD_STBY_GPIO_Port GPIOA
#define MD_PWMB_Pin GPIO_PIN_5
#define MD_PWMB_GPIO_Port GPIOA
#define MD_BIN1_Pin GPIO_PIN_6
#define MD_BIN1_GPIO_Port GPIOA
#define MD_BIN2_Pin GPIO_PIN_7
#define MD_BIN2_GPIO_Port GPIOA
#define LED_Pin GPIO_PIN_1
#define LED_GPIO_Port GPIOB
#define MT_PWM_Pin GPIO_PIN_6
#define MT_PWM_GPIO_Port GPIOC
#define MT_SDA_Pin GPIO_PIN_9
#define MT_SDA_GPIO_Port GPIOC
#define MT_SCL_Pin GPIO_PIN_8
#define MT_SCL_GPIO_Port GPIOA
#define ENC_B_Pin GPIO_PIN_6
#define ENC_B_GPIO_Port GPIOB
#define ENC_A_Pin GPIO_PIN_7
#define ENC_A_GPIO_Port GPIOB
#define IMU_SCL_Pin GPIO_PIN_8
#define IMU_SCL_GPIO_Port GPIOB
#define IMU_SDA_Pin GPIO_PIN_9
#define IMU_SDA_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
