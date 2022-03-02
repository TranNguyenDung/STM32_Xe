/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
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
#include "stm32f1xx_hal.h"

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
#define LED_Pin GPIO_PIN_13
#define LED_GPIO_Port GPIOC
#define PWM_WATHAN_A_Pin GPIO_PIN_0
#define PWM_WATHAN_A_GPIO_Port GPIOA
#define PWM_WATHAN_B_Pin GPIO_PIN_1
#define PWM_WATHAN_B_GPIO_Port GPIOA
#define PWM_RIGHT_EN_Pin GPIO_PIN_4
#define PWM_RIGHT_EN_GPIO_Port GPIOA
#define PWM_LEFT_EN_Pin GPIO_PIN_5
#define PWM_LEFT_EN_GPIO_Port GPIOA
#define PWM_RIGHT_B_Pin GPIO_PIN_6
#define PWM_RIGHT_B_GPIO_Port GPIOA
#define PWM_RIGHT_A_Pin GPIO_PIN_7
#define PWM_RIGHT_A_GPIO_Port GPIOA
#define PWM_LEFT_A_Pin GPIO_PIN_0
#define PWM_LEFT_A_GPIO_Port GPIOB
#define PWM_LEFT_B_Pin GPIO_PIN_1
#define PWM_LEFT_B_GPIO_Port GPIOB
#define RAS_SET_TH_H_Pin GPIO_PIN_15
#define RAS_SET_TH_H_GPIO_Port GPIOB
#define RAS_SET_TH_L_Pin GPIO_PIN_8
#define RAS_SET_TH_L_GPIO_Port GPIOA
#define RAS_RIGHT_H_Pin GPIO_PIN_11
#define RAS_RIGHT_H_GPIO_Port GPIOA
#define RAS_RIGHT_L_Pin GPIO_PIN_12
#define RAS_RIGHT_L_GPIO_Port GPIOA
#define RAS_CENTER_Pin GPIO_PIN_15
#define RAS_CENTER_GPIO_Port GPIOA
#define RAS_LEFT_L_Pin GPIO_PIN_3
#define RAS_LEFT_L_GPIO_Port GPIOB
#define RAS_LEFT_H_Pin GPIO_PIN_4
#define RAS_LEFT_H_GPIO_Port GPIOB
#define RAS_STOP_Pin GPIO_PIN_5
#define RAS_STOP_GPIO_Port GPIOB
#define RAS_BLINK_Pin GPIO_PIN_6
#define RAS_BLINK_GPIO_Port GPIOB
#define RAS_SET_SHUTDOWN_Pin GPIO_PIN_7
#define RAS_SET_SHUTDOWN_GPIO_Port GPIOB
#define PWM_LED_H_Pin GPIO_PIN_8
#define PWM_LED_H_GPIO_Port GPIOB
#define PWM_LED_L_Pin GPIO_PIN_9
#define PWM_LED_L_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
