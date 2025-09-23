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
#include "stm32l4xx_hal.h"

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
#define FAN_CON_Pin GPIO_PIN_14
#define FAN_CON_GPIO_Port GPIOC
#define FAN_SPEED_Pin GPIO_PIN_15
#define FAN_SPEED_GPIO_Port GPIOC
#define SiPM_Sig_Pin GPIO_PIN_1
#define SiPM_Sig_GPIO_Port GPIOA
#define PD_Sig_Pin GPIO_PIN_2
#define PD_Sig_GPIO_Port GPIOA
#define LD_Mon_Pin GPIO_PIN_3
#define LD_Mon_GPIO_Port GPIOA
#define HV_Chk_Pin GPIO_PIN_4
#define HV_Chk_GPIO_Port GPIOA
#define HV_CON_Pin GPIO_PIN_1
#define HV_CON_GPIO_Port GPIOB
#define FAN_RPM_Pin GPIO_PIN_8
#define FAN_RPM_GPIO_Port GPIOA
#define FAN_RPM_EXTI_IRQn EXTI9_5_IRQn
#define LD_CON_Pin GPIO_PIN_4
#define LD_CON_GPIO_Port GPIOB
#define PROM_WP_Pin GPIO_PIN_5
#define PROM_WP_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
