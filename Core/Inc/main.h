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
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
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

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define CC1In_Pin GPIO_PIN_13
#define CC1In_GPIO_Port GPIOC
#define EleLock_Pin GPIO_PIN_14
#define EleLock_GPIO_Port GPIOC
#define EleUnlock_Pin GPIO_PIN_15
#define EleUnlock_GPIO_Port GPIOC
#define BoostEN_Pin GPIO_PIN_2
#define BoostEN_GPIO_Port GPIOA
#define StartON_Pin GPIO_PIN_3
#define StartON_GPIO_Port GPIOA
#define KeySta_Pin GPIO_PIN_4
#define KeySta_GPIO_Port GPIOA
#define VbatChg_Pin GPIO_PIN_7
#define VbatChg_GPIO_Port GPIOA
#define VbatNeg_Pin GPIO_PIN_4
#define VbatNeg_GPIO_Port GPIOC
#define RunLed_Pin GPIO_PIN_5
#define RunLed_GPIO_Port GPIOC
#define BmsON_Pin GPIO_PIN_6
#define BmsON_GPIO_Port GPIOC
#define IMDLrly_Pin GPIO_PIN_7
#define IMDLrly_GPIO_Port GPIOC
#define IMDHrly_Pin GPIO_PIN_8
#define IMDHrly_GPIO_Port GPIOC
#define PeRly_Pin GPIO_PIN_9
#define PeRly_GPIO_Port GPIOC
#define LeakIn_Pin GPIO_PIN_8
#define LeakIn_GPIO_Port GPIOA
#define Lrly_Pin GPIO_PIN_10
#define Lrly_GPIO_Port GPIOC
#define Hrly_Pin GPIO_PIN_11
#define Hrly_GPIO_Port GPIOC
#define LockSW_Pin GPIO_PIN_12
#define LockSW_GPIO_Port GPIOC
#define OBSBtn_Pin GPIO_PIN_2
#define OBSBtn_GPIO_Port GPIOD
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
