#include "my_scc.h"
/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */

void SysTick_Handler(void)
{
} 

void TIM2_IRQHandler(void)
{
	TIM_ClearITPendingBit(TIM2,TIM_IT_CC1);
}
void TIM3_IRQHandler(void)
{
	TIM_ClearITPendingBit(TIM3,TIM_IT_CC1);

}

u16 ms_count=0;

extern u8 fuck;
void TIM4_IRQHandler(void)
{
	TIM_ClearITPendingBit(TIM4,TIM_IT_CC1);
	if(!(ms_count%10))
	{
		JY901_GetData(&imu_dev);
		
		//角速度内环
//		angular_speed_pid_calculate(&pitch_pid,imu_dev.y_angular_velocity);
//		angular_speed_pid_calculate(&roll_pid,imu_dev.x_angular_velocity);
//		angular_speed_pid_calculate(&yaw_pid,imu_dev.z_angular_velocity);
		up_down_control();
		motor_control();
	}
	if(!(ms_count%20))
	{
		key_scanf();
		//角度外环
//		angle_pid_calculate(&pitch_pid,imu_dev.pitch,imu_dev.y_angular_velocity);
//		angle_pid_calculate(&roll_pid,imu_dev.roll,imu_dev.x_angular_velocity);
		//方向环
	  direction_control();
		
		//深度环
		deepth_pid();
		
		//电压计算
		calculate_bat();
		
		//闪烁
		led_flash_int();
	}
	
	if(!(ms_count%50))
	{
		if(daka_count==1) 
		{
	//		direction_point_offset=24;
			if(direction_point_offset<24)
			{
				direction_point_offset+=0.5;
			}
		}
		else if(daka_count==2) 
		{
	//		direction_point_offset=19;
			if(direction_point_offset>0)
			{
				direction_point_offset-=0.07;
			}
		}
		else direction_point_offset=0;
	}
	
	if(!(ms_count%1000))
	{
		second++;
	}
	
	
	
	ms_count++;
	if(ms_count>999) ms_count=0;
}
void EXTI15_10_IRQHandler(void)
{
  EXTI_ClearITPendingBit(EXTI_Line11);
}

void USART1_IRQHandler(void)
{
	if(USART_GetITStatus(USART1,USART_IT_RXNE)==SET)
	{
		USART_ClearITPendingBit(USART1,USART_IT_RXNE);
    CopeSerialData((u8)USART_ReceiveData(USART1));
	}
	if(USART_GetITStatus(USART1,USART_IT_TC)==SET)
	{
		USART_ClearITPendingBit(USART1,USART_IT_TC);
		tx_it_deal(&uart1);
  }
	if(USART_GetFlagStatus(USART1,USART_FLAG_ORE)==SET) 
  { 
    USART_ClearFlag(USART1,USART_FLAG_ORE); 
  }
}
void USART2_IRQHandler(void)
{
	if(USART_GetITStatus(USART2,USART_IT_RXNE)==SET)
	{
		USART_ClearITPendingBit(USART2,USART_IT_RXNE);
    Pi_Stm32_unpackData((u8)USART_ReceiveData(USART2));
	}
	if(USART_GetITStatus(USART2,USART_IT_TC)==SET)
	{
		USART_ClearITPendingBit(USART2,USART_IT_TC);
		tx_it_deal(&uart2);
  }
	if(USART_GetFlagStatus(USART2,USART_FLAG_ORE)==SET) 
  { 
    USART_ClearFlag(USART2,USART_FLAG_ORE); 
  }
}
void USART3_IRQHandler(void)
{
	if(USART_GetITStatus(USART3,USART_IT_RXNE)==SET)
	{
		USART_ClearITPendingBit(USART3,USART_IT_RXNE);
	}
	if(USART_GetITStatus(USART3,USART_IT_TC)==SET)
	{
		USART_ClearITPendingBit(USART3,USART_IT_TC);
		tx_it_deal(&uart3);
  }
	if(USART_GetFlagStatus(USART3,USART_FLAG_ORE)==SET) 
  { 
    USART_ClearFlag(USART3,USART_FLAG_ORE); 
  }
}
