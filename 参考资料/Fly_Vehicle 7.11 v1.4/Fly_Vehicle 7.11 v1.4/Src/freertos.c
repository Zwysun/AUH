/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
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

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */     
#include "includes.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
/* USER CODE END Variables */
osThreadId messageTaskHandle;
osThreadId sensorTaskHandle;
osThreadId controlTaskHandle;
osThreadId imuTaskHandle;
osThreadId ledTaskHandle;
osThreadId pressureTaskHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
   
/* USER CODE END FunctionPrototypes */

void message_Task_Function(void const * argument);
void sensor_Task_Function(void const * argument);
void control_Task_Function(void const * argument);
void control_Ward_Function(void const * argument);
void imu_Task_Function(void const * argument);
void led_Task_Function(void const * argument);
void pressure_Task_Function(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */
       
  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  Task_Queue_Semaphore_Timers_Create();
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of messageTask */
  osThreadDef(messageTask, message_Task_Function, osPriorityNormal, 0, 500);
  messageTaskHandle = osThreadCreate(osThread(messageTask), NULL);

  /* definition and creation of sensorTask */
  osThreadDef(sensorTask, sensor_Task_Function, osPriorityIdle, 0, 128);
  sensorTaskHandle = osThreadCreate(osThread(sensorTask), NULL);

  /* definition and creation of controlTask */
  osThreadDef(controlTask, control_Task_Function, osPriorityIdle, 0, 128);
  controlTaskHandle = osThreadCreate(osThread(controlTask), NULL);
	
	 /* definition and creation of controlWardTask */
  osThreadDef(controlWardTask, control_Ward_Function, osPriorityIdle, 0, 128);
  controlTaskHandle = osThreadCreate(osThread(controlWardTask), NULL);

  /* definition and creation of imuTask */
  osThreadDef(imuTask, imu_Task_Function, osPriorityIdle, 0, 128);
  imuTaskHandle = osThreadCreate(osThread(imuTask), NULL);

  /* definition and creation of ledTask */
  osThreadDef(ledTask, led_Task_Function, osPriorityIdle, 0, 128);
  ledTaskHandle = osThreadCreate(osThread(ledTask), NULL);

  /* definition and creation of pressureTask */
  osThreadDef(pressureTask, pressure_Task_Function, osPriorityHigh, 0, 128);
  pressureTaskHandle = osThreadCreate(osThread(pressureTask), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_message_Task_Function */
/**
  * @brief  Function implementing the messageTask thread.
  * @param  argument: Not used 
  * @retval None
  */
/* USER CODE END Header_message_Task_Function */
__weak void message_Task_Function(void const * argument)
{

  /* USER CODE BEGIN message_Task_Function */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END message_Task_Function */
}

/* USER CODE BEGIN Header_sensor_Task_Function */
/**
* @brief Function implementing the sensorTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_sensor_Task_Function */
__weak void sensor_Task_Function(void const * argument)
{
  /* USER CODE BEGIN sensor_Task_Function */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END sensor_Task_Function */
}

/* USER CODE BEGIN Header_control_Task_Function */
/**
* @brief Function implementing the controlTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_control_Task_Function */
__weak void control_Task_Function(void const * argument)
{
  /* USER CODE BEGIN control_Task_Function */
  /* Infinite loop */
  for(;;)
  {
		osDelay(1000);
  }
  /* USER CODE END control_Task_Function */
}

/* USER CODE BEGIN Header_control_Task_Function */
/**
* @brief Function implementing the controlTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_control_Task_Function */
__weak void control_Ward_Function(void const * argument)
{
  /* USER CODE BEGIN control_Task_Function */
  /* Infinite loop */
  for(;;)
  {
		osDelay(1000);
  }
  /* USER CODE END control_Task_Function */
}

/* USER CODE BEGIN Header_imu_Task_Function */
/**
* @brief Function implementing the imuTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_imu_Task_Function */
__weak void imu_Task_Function(void const * argument)
{
  /* USER CODE BEGIN imu_Task_Function */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1000);
  }
  /* USER CODE END imu_Task_Function */
}

/* USER CODE BEGIN Header_led_Task_Function */
/**
* @brief Function implementing the ledTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_led_Task_Function */
__weak void led_Task_Function(void const * argument)
{
  /* USER CODE BEGIN led_Task_Function */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END led_Task_Function */
}

/* USER CODE BEGIN Header_pressure_Task_Function */
/**
* @brief Function implementing the pressureTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_pressure_Task_Function */
__weak void pressure_Task_Function(void const * argument)
{
  /* USER CODE BEGIN pressure_Task_Function */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END pressure_Task_Function */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
     
/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
