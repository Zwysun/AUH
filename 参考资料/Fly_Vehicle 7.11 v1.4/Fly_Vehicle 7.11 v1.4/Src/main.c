/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
#include "main.h"
#include "cmsis_os.h"
#include "adc.h"
#include "dma.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

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

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void MX_FREERTOS_Init(void);

extern uint16_t pwm_left;
extern uint16_t pwm_right;
extern uint16_t pwm_front;
extern uint16_t pwm_back;

extern uint16_t test_pwm;//所有默认值都是1500
extern uint16_t *test_pwm1;
extern uint16_t test_pwm2;
extern uint16_t *test_pwm3;
extern uint16_t test_pwm4;
extern uint16_t *test_pwm5;
extern uint16_t test_pwm6;
extern uint16_t *test_pwm7;
extern uint16_t test_pwm8;

extern uint8_t enter[];

uint8_t lora_ch;
bool osStart = 0;

extern void Control_0(void);
extern void Control_t(void);
extern void LED_SW(u8 num,u8 sta);

extern PI_UsartRec_t stm32_pi_Rec;
extern ControlMsg_t control_data;
extern UART_HandleTypeDef huart3;
extern enum MODE_CHOOSE{MODE0=0,MODEt}mode;


extern bool stop_startflag;
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */


//void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
//{
//  /* Prevent unused argument(s) compilation warning */
//  HAL_UART3_Receive_IDLE(huart);
//  /* NOTE: This function Should not be modified, when the callback is needed,
//           the HAL_UART_TxCpltCallback could be implemented in the user file
//   */ 
//}这个没用


/**
  * @brief  The application entry point.
  * @retval int
  */
	
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */
  

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_ADC1_Init();
  MX_I2C2_Init();
  MX_TIM1_Init();
  MX_TIM3_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  MX_USART3_UART_Init();
  MX_TIM6_Init();
	
	
	
//	MX_TIM5_Init();
  /* USER CODE BEGIN 2 */

  BSP_Init();
	
	LED_SW(1,1);
	stop_startflag=0;
	
//	while(1)//pwm测试
//	{
//		__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,1450);//1   上下
//		__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,1450);// 
//		__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3,1450);//2   前后
//		__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_4,1450);//
//		
//		__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_1,1450);//3   上下
//		__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_2,1450);// 
//		__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_3,1450);//4   前后
//		__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_4,1450);//

//		HAL_Delay(500);
//	}
	
//	__HAL_UART_ENABLE_IT(&SX1278_USART, UART_IT_RXNE);//
		
		
	delay_ms(3000);
  /* USER CODE END 2 */

	LED_SW(1,0);
	
//		pwm_right =1451;
//		pwm_left =1552;

////		__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,test_pwm1);//1   上下，前
////		__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,test_pwm2);// 
////		__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_1,test_pwm5);//3   上下，后
////		__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_2,test_pwm6);//
//		__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3,*test_pwm3);//2   前后，右
//		__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_4,*test_pwm3);//
//		__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_3,*test_pwm7);//4   前后，左
//		__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_4,*test_pwm7);//
//		
//		HAL_UART_Transmit(&SX1278_USART, (uint8_t*)"in", 2, 0x100);
//		HAL_UART_Transmit(&SX1278_USART, enter, 2, 0x100);
//		__HAL_UART_ENABLE_IT(&PI_USART, UART_IT_IDLE);


	/* Call init function for freertos objects (in freertos.c) */
  MX_FREERTOS_Init();

	/* Start scheduler */
  osKernelStart();
  
  /* We should never get here as control is now taken by the scheduler */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	

//	HAL_UART_Transmit(&SX1278_USART, (uint8_t*)"55", 2, 0x100);//TC被置1但未能进入中断入口
//	if((__HAL_UART_GET_FLAG(&huart3,UART_FLAG_TC) != RESET))LED_SW(2,1);
//	delay_ms(2000);
//	HAL_UART_Transmit(&SX1278_USART, (uint8_t*)"55", 2, 0x100);//TC被置1但未能进入中断入口
//	if((__HAL_UART_GET_FLAG(&huart3,UART_FLAG_TC) != RESET))LED_SW(3,1);
//	
	int count = 0;
	char string[20];
	uint8_t len_str;
//	control_data.navi_angle = 1;
//	control_data.navi_distance = 150;

	
  while (1)
  {
//		HAL_UART_Transmit(&SX1278_USART, (uint8_t*)"55", 2, 0x100);
//		delay_ms(500);

    /* USER CODE END WHILE */

		
//		if(stm32_pi_Rec.RX_flag)
//		{
////			HAL_UART_Transmit(&SX1278_USART, (uint8_t*)"55", 2, 0x100);
//			count++;
//			if(count==8){
//				//将control_data.navi_angle转为字符串，然后用串口分别发给树莓派和lora电脑端
//				len_str=sprintf(string,"dis=%3.3f\r\n",control_data.navi_distance);
//				HAL_UART_Transmit(&SX1278_USART, (uint8_t*)string, len_str, 0x100);
//				len_str=sprintf(string,"ang=%3.3f\r\n",control_data.navi_angle*180/3.14);
//				HAL_UART_Transmit(&SX1278_USART, (uint8_t*)string, len_str, 0x100);
//				count=0;
//			}
			
//			len_str=sprintf(string,"PWMinMAIN%d\r\n",test_pwm1);
//			HAL_UART_Transmit(&SX1278_USART, (uint8_t*)string, len_str, 0x100);
			
//		switch(mode){
//			case MODE0:{//工作模式0
//				Control_0();
//				delay_ms(300);
//				break;
//			}
//			case MODEt:{//工作模式t
//				Control_t();
//				break;
//			}
//			default:break;
//		}
//			stm32_pi_Rec.RX_flag=0;
//		}
//    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM7 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM7) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */
//	if (htim->Instance == TIM5) {
//		//code here
//		
//		
//		
//	}
  /* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
