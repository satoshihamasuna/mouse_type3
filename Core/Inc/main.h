/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
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

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define sens_fr_Pin GPIO_PIN_0
#define sens_fr_GPIO_Port GPIOA
#define sens_dl_Pin GPIO_PIN_1
#define sens_dl_GPIO_Port GPIOA
#define sens_dr_Pin GPIO_PIN_2
#define sens_dr_GPIO_Port GPIOA
#define sens_fl_Pin GPIO_PIN_3
#define sens_fl_GPIO_Port GPIOA
#define sens_batt_Pin GPIO_PIN_4
#define sens_batt_GPIO_Port GPIOA
#define GYRO_CS_Pin GPIO_PIN_10
#define GYRO_CS_GPIO_Port GPIOB
#define LENC_CS_Pin GPIO_PIN_14
#define LENC_CS_GPIO_Port GPIOB
#define photo_dl_Pin GPIO_PIN_15
#define photo_dl_GPIO_Port GPIOB
#define photo_fl_Pin GPIO_PIN_8
#define photo_fl_GPIO_Port GPIOA
#define LED5_Pin GPIO_PIN_11
#define LED5_GPIO_Port GPIOA
#define LED4_Pin GPIO_PIN_12
#define LED4_GPIO_Port GPIOA
#define LED3_Pin GPIO_PIN_13
#define LED3_GPIO_Port GPIOA
#define LED2_Pin GPIO_PIN_14
#define LED2_GPIO_Port GPIOA
#define LED1_Pin GPIO_PIN_15
#define LED1_GPIO_Port GPIOA
#define RENC_CS_Pin GPIO_PIN_3
#define RENC_CS_GPIO_Port GPIOB
#define photo_fr_Pin GPIO_PIN_6
#define photo_fr_GPIO_Port GPIOB
#define photo_dr_Pin GPIO_PIN_7
#define photo_dr_GPIO_Port GPIOB
#define BUTTON_Pin GPIO_PIN_8
#define BUTTON_GPIO_Port GPIOB
#define LED6_Pin GPIO_PIN_9
#define LED6_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/