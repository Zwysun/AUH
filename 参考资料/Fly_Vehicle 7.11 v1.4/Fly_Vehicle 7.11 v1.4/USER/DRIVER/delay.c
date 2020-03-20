#include "delay.h"

////////////////////////////////////////////////////////////////////////////////// 	 
//�����Ҫʹ��OS,����������ͷ�ļ�����.
#if SYSTEM_SUPPORT_OS
#include "FreeRTOS.h"					//FreeRTOSʹ��		  
#include "task.h" 
#endif
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32������
//ʹ��SysTick����ͨ����ģʽ���ӳٽ��й���(֧��OS)
//����delay_us,delay_ms
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2016/11/28
//�汾��V1.8
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved
//********************************************************************************
//�޸�˵��
//////////////////////////////////////////////////////////////////////////////////  

static u8  fac_us=0;							//us��ʱ������			   
static u16 fac_ms=0;							//ms��ʱ������,��ucos��,����ÿ�����ĵ�ms��
 
 
extern void xPortSysTickHandler(void);

//systick�жϷ�����,ʹ��ucosʱ�õ�
//void SysTick_Handler(void)
//{	
//    if(xTaskGetSchedulerState()!=taskSCHEDULER_NOT_STARTED)//ϵͳ�Ѿ�����
//    {
//        xPortSysTickHandler();	
//    }
//}
//��ʼ���ӳٺ���
//SYSTICK��ʱ�ӹ̶�ΪAHBʱ�ӣ�������������SYSTICKʱ��Ƶ��ΪAHB/8
//����Ϊ�˼���FreeRTOS�����Խ�SYSTICK��ʱ��Ƶ�ʸ�ΪAHB��Ƶ�ʣ�
//SYSCLK:ϵͳʱ��Ƶ��
void delay_init()
{
	u32 reload;
	
	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);//ѡ���ⲿʱ��  HCLK
	fac_us=SystemCoreClock/1000000;				      //�����Ƿ�ʹ��OS,fac_us����Ҫʹ��
	reload=SystemCoreClock/1000000;				      //ÿ���ӵļ������� ��λΪM  
	reload*=1000000/configTICK_RATE_HZ;			    //����configTICK_RATE_HZ�趨���ʱ��
						           //reloadΪ24λ�Ĵ���,���ֵ:16777216,��72M��,Լ��0.233s����	
	fac_ms=1000/configTICK_RATE_HZ;		          //����OS������ʱ�����ٵ�λ	   

	SysTick->CTRL|=SysTick_CTRL_TICKINT_Msk;   	//����SYSTICK�ж�
	SysTick->LOAD=reload; 											//ÿ1/configTICK_RATE_HZ���ж�һ��	
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk;   	//����SYSTICK    
}			   
void delay_sys_us(uint16_t us)
{
   
	 for(u16 time = 0;time<us*72;time++)
		{
			__NOP;
		}
}							    
//void delay_sys_us(uint16_t us)
//{
//    uint16_t differ=0xffff-us-5;
//    /*Ϊ��ֹ���жϴ����ʱ����ɼ�������.
//     ���0xfffE��ʼ��ʱ1us,�������жϴ��
//    ����ʱ���������ڼ����������������0xffff��
//    ��ֹͣ�����������ڴ������ֵ��������arrֵ��
//    ����ʵ����ʱ(0xffff+1)us
//    */

//    HAL_TIM_Base_Start(&htim6);

//    __HAL_TIM_SET_COUNTER(&htim6,differ);

//    while(differ<0xffff-5)
//    {
//        differ=__HAL_TIM_GET_COUNTER(&htim6);
//    }

//    HAL_TIM_Base_Stop(&htim6);
//}

//��ʱnus
//nus:Ҫ��ʱ��us��.	
//nus:0~204522252(���ֵ��2^32/fac_us@fac_us=168)	    								   
//void delay_sys_us(u16 nus)
//{		
//	u32 ticks;
//	u32 told,tnow,tcnt=0;
//	u32 reload=SysTick->LOAD;				//LOAD��ֵ	    	 
//	ticks=nus*fac_us; 						  //��Ҫ�Ľ����� 
//	told=SysTick->VAL;        		  //�ս���ʱ�ļ�����ֵ
//	while(1)
//	{
//		tnow=SysTick->VAL;	
//		if(tnow!=told)
//		{	    
//			if(tnow<told)tcnt+=told-tnow;	//����ע��һ��SYSTICK��һ���ݼ��ļ������Ϳ�����.
//			else tcnt+=reload-tnow+told;	    
//			told=tnow;
//			if(tcnt>=ticks)break;			//ʱ�䳬��/����Ҫ�ӳٵ�ʱ��,���˳�.
//		}  
//	};	
//	
// 	
//}  
