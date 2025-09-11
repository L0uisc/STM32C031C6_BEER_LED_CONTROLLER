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
#include "stm32c0xx_hal.h"

#include "stm32c0xx_nucleo.h"
#include <stdio.h>

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
#define RCC_OSCX_IN_Pin GPIO_PIN_14
#define RCC_OSCX_IN_GPIO_Port GPIOC
#define RCC_OSCX_OUT_Pin GPIO_PIN_15
#define RCC_OSCX_OUT_GPIO_Port GPIOC
#define GROUP7_G_Pin GPIO_PIN_3
#define GROUP7_G_GPIO_Port GPIOF
#define RCC_OSC_IN_Pin GPIO_PIN_0
#define RCC_OSC_IN_GPIO_Port GPIOF
#define RCC_OSC_OUT_Pin GPIO_PIN_1
#define RCC_OSC_OUT_GPIO_Port GPIOF
#define GROUP2_G_Pin GPIO_PIN_0
#define GROUP2_G_GPIO_Port GPIOA
#define GROUP2_B_Pin GPIO_PIN_1
#define GROUP2_B_GPIO_Port GPIOA
#define GROUP3_R_Pin GPIO_PIN_4
#define GROUP3_R_GPIO_Port GPIOA
#define GROUP5_R_Pin GPIO_PIN_6
#define GROUP5_R_GPIO_Port GPIOA
#define GROUP5_G_Pin GPIO_PIN_7
#define GROUP5_G_GPIO_Port GPIOA
#define GROUP6_R_Pin GPIO_PIN_0
#define GROUP6_R_GPIO_Port GPIOB
#define GROUP3_G_Pin GPIO_PIN_1
#define GROUP3_G_GPIO_Port GPIOB
#define GROUP6_G_Pin GPIO_PIN_2
#define GROUP6_G_GPIO_Port GPIOB
#define GROUP10_R_Pin GPIO_PIN_10
#define GROUP10_R_GPIO_Port GPIOB
#define GROUP3_B_Pin GPIO_PIN_11
#define GROUP3_B_GPIO_Port GPIOB
#define GROUP5_B_Pin GPIO_PIN_12
#define GROUP5_B_GPIO_Port GPIOB
#define GROUP9_B_Pin GPIO_PIN_13
#define GROUP9_B_GPIO_Port GPIOB
#define GROUP9_G_Pin GPIO_PIN_14
#define GROUP9_G_GPIO_Port GPIOB
#define GROUP8_G_Pin GPIO_PIN_15
#define GROUP8_G_GPIO_Port GPIOB
#define GROUP8_R_Pin GPIO_PIN_8
#define GROUP8_R_GPIO_Port GPIOA
#define GROUP7_R_Pin GPIO_PIN_9
#define GROUP7_R_GPIO_Port GPIOA
#define GROUP2_R_Pin GPIO_PIN_6
#define GROUP2_R_GPIO_Port GPIOC
#define GROUP6_B_Pin GPIO_PIN_7
#define GROUP6_B_GPIO_Port GPIOC
#define GROUP10_B_Pin GPIO_PIN_10
#define GROUP10_B_GPIO_Port GPIOA
#define DEBUG_SWDIO_Pin GPIO_PIN_13
#define DEBUG_SWDIO_GPIO_Port GPIOA
#define DEBUG_SWCLK_Pin GPIO_PIN_14
#define DEBUG_SWCLK_GPIO_Port GPIOA
#define GROUP7_B_Pin GPIO_PIN_15
#define GROUP7_B_GPIO_Port GPIOA
#define GROUP1_R_Pin GPIO_PIN_0
#define GROUP1_R_GPIO_Port GPIOD
#define GROUP1_G_Pin GPIO_PIN_1
#define GROUP1_G_GPIO_Port GPIOD
#define GROUP1_B_Pin GPIO_PIN_2
#define GROUP1_B_GPIO_Port GPIOD
#define GROUP4_R_Pin GPIO_PIN_3
#define GROUP4_R_GPIO_Port GPIOD
#define GROUP10_G_Pin GPIO_PIN_3
#define GROUP10_G_GPIO_Port GPIOB
#define GROUP9_R_Pin GPIO_PIN_4
#define GROUP9_R_GPIO_Port GPIOB
#define GROUP8_B_Pin GPIO_PIN_5
#define GROUP8_B_GPIO_Port GPIOB
#define GROUP4_G_Pin GPIO_PIN_8
#define GROUP4_G_GPIO_Port GPIOB
#define GROUP4_B_Pin GPIO_PIN_9
#define GROUP4_B_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
