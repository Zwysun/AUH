/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
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
#define LED8_Pin GPIO_PIN_2
#define LED8_GPIO_Port GPIOE
#define LED7_Pin GPIO_PIN_3
#define LED7_GPIO_Port GPIOE
#define LED6_Pin GPIO_PIN_4
#define LED6_GPIO_Port GPIOE
#define LED5_Pin GPIO_PIN_5
#define LED5_GPIO_Port GPIOE
#define POWER_KEY_STATE_Pin GPIO_PIN_2
#define POWER_KEY_STATE_GPIO_Port GPIOC
#define POWER_KEY_STATE_EXTI_IRQn EXTI2_IRQn
#define POWER_KEY_Pin GPIO_PIN_3
#define POWER_KEY_GPIO_Port GPIOC
#define CURRENT_INA282_Pin GPIO_PIN_0
#define CURRENT_INA282_GPIO_Port GPIOA
#define VOLTAGE_Pin GPIO_PIN_1
#define VOLTAGE_GPIO_Port GPIOA
#define PI_USART_TX_Pin GPIO_PIN_2
#define PI_USART_TX_GPIO_Port GPIOA
#define PI_USART_RX_Pin GPIO_PIN_3
#define PI_USART_RX_GPIO_Port GPIOA
#define SHT35_IIC_SCL_Pin GPIO_PIN_10
#define SHT35_IIC_SCL_GPIO_Port GPIOB
#define SHT35_IIC_SDA_Pin GPIO_PIN_11
#define SHT35_IIC_SDA_GPIO_Port GPIOB
#define LORA_USART_TX_Pin GPIO_PIN_8
#define LORA_USART_TX_GPIO_Port GPIOD
#define LORA_USART_RX_Pin GPIO_PIN_9
#define LORA_USART_RX_GPIO_Port GPIOD
#define LORA_AUX_Pin GPIO_PIN_10
#define LORA_AUX_GPIO_Port GPIOD
#define LORA_M0_Pin GPIO_PIN_11
#define LORA_M0_GPIO_Port GPIOD
#define LORA_M1_Pin GPIO_PIN_12
#define LORA_M1_GPIO_Port GPIOD
#define JY901_USART_TX_Pin GPIO_PIN_9
#define JY901_USART_TX_GPIO_Port GPIOA
#define JY901_USART_RX_Pin GPIO_PIN_10
#define JY901_USART_RX_GPIO_Port GPIOA
#define HP206F_IIC_SCL_Pin GPIO_PIN_6
#define HP206F_IIC_SCL_GPIO_Port GPIOB
#define HP206F_IIC_SDA_Pin GPIO_PIN_7
#define HP206F_IIC_SDA_GPIO_Port GPIOB
#define LED1_Pin GPIO_PIN_8
#define LED1_GPIO_Port GPIOB
#define LED2_Pin GPIO_PIN_9
#define LED2_GPIO_Port GPIOB
#define LED3_Pin GPIO_PIN_0
#define LED3_GPIO_Port GPIOE
#define LED4_Pin GPIO_PIN_1
#define LED4_GPIO_Port GPIOE
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
