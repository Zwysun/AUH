#include "led.h"

/**
  * @brief  LED 灯组控制
  * @param  num : led id  ,sta : 0(关) 1(开)
  * @note  
  */
void LED_SW(u8 num,u8 sta)
{
	switch(num)
	{
		case 1:HAL_GPIO_WritePin(LED1_GPIO_Port,LED1_Pin,(sta?GPIO_PIN_SET:GPIO_PIN_RESET));break;
		case 2:HAL_GPIO_WritePin(LED2_GPIO_Port,LED2_Pin,(sta?GPIO_PIN_SET:GPIO_PIN_RESET));break;
		case 3:HAL_GPIO_WritePin(LED3_GPIO_Port,LED3_Pin,(sta?GPIO_PIN_SET:GPIO_PIN_RESET));break;
		case 4:HAL_GPIO_WritePin(LED4_GPIO_Port,LED4_Pin,(sta?GPIO_PIN_SET:GPIO_PIN_RESET));break;
		case 5:HAL_GPIO_WritePin(LED5_GPIO_Port,LED5_Pin,(sta?GPIO_PIN_SET:GPIO_PIN_RESET));break;
		case 6:HAL_GPIO_WritePin(LED6_GPIO_Port,LED6_Pin,(sta?GPIO_PIN_SET:GPIO_PIN_RESET));break;
		case 7:HAL_GPIO_WritePin(LED7_GPIO_Port,LED7_Pin,(sta?GPIO_PIN_SET:GPIO_PIN_RESET));break;
		case 8:HAL_GPIO_WritePin(LED8_GPIO_Port,LED8_Pin,(sta?GPIO_PIN_SET:GPIO_PIN_RESET));break;
	}
}
/**
  * @brief  LED 测试demo
  * @param   
  * @note  
  */
void LED_Test_Demo(void)
{
	
	while(1)
	{
		delay_ms(100);//100
 	  for(int i=1;i<=8;i++)
		{
			LED_SW(i,0);
			delay_ms(1000);//100
		}
	  for(int i=8;i>=1;i--)
		{
			LED_SW(i,1);
			delay_ms(1000);//100
		}
	}
}