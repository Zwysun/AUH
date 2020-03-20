/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   摄像头火眼ov7725测试例程
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火 ISO-MINI STM32 开发板 
  * 论坛    :http://www.chuxue123.com
  * 淘宝    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */
	
/*

版权声明:

本代码为MINI-AUH的完整控制代码，本次公开仅限于小范围学院内部学习，
且在正式比赛中不得复制本程序任何一段代码,禁止用于商业用途.
此授权之外的任何用途，本团队将保留追究责相关责任的权利。

浙江大学海洋学院海工所
水下直升机课题组
陆培 刘勋

*/	
  
#include "stm32f10x.h"
#include "bsp_ov7725.h"
#include "bsp_ili9341_lcd.h"
#include "bsp_usart1.h"





extern uint8_t Ov7725_vsync;

/**
  * @brief  主函数
  * @param  无  
  * @retval 无
  */
int main(void) 	
{
	/* 液晶初始化 */
	LCD_Init();
	
	USART1_Config();//初始化一组串口方便调试
	
  //while(1);
  #if 1
	/* ov7725 gpio 初始化 */
	Ov7725_GPIO_Config();
	
	/* ov7725 寄存器配置初始化 */
	while(Ov7725_Init() != SUCCESS);
	
	/* ov7725 场信号线初始化 */
	VSYNC_Init();	
	Ov7725_vsync = 0;
	
	while(1)
	{
		if( Ov7725_vsync == 2 )
		{
			FIFO_PREPARE;  			/*FIFO准备*/					
			ImagDisp();					/*采集并显示*/
			Ov7725_vsync = 0;			
		}
	}
  #endif
}

/*********************************************END OF FILE**********************/

