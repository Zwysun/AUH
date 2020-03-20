#include "stm32_pi.h"
float navi_center_to_line_angle = 0.0;   //中心到色带角度
float navi_center_to_line_distance = 0.0;//中心到色带距离
unsigned char ucRxBuffer_[50];
unsigned char ucRxCnt_ = 0;	
u8 step = 0;
u8 find_line_flag=0;
void Pi_Stm32_unpackData(unsigned char ucData)
{
	ucRxBuffer_[ucRxCnt_++]=ucData;
	if(step==0)
	{
		if(ucRxBuffer_[0]!=0xFE)
		{
			ucRxCnt_=0;
			return;
		}
		else step=1;
	}
	else if(step==1) 
	{
		if(ucRxCnt_==ucRxBuffer_[1] && ucRxBuffer_[ucRxCnt_-1] == 0x0a)
		{
			step=0;
			ucRxCnt_=0;
			switch(ucRxBuffer_[3])
			{
					case MSG_NAVI_ID:
							memcpy((void*)&navi_center_to_line_angle,(void*)&(ucRxBuffer_[4]),4); 
							memcpy((void*)&navi_center_to_line_distance,(void*)&(ucRxBuffer_[4+4]),4);		
              find_line_flag=1;		
//					GPIOB->ODR ^= GPIO_Pin_8;		
							break;
					case MSG_DAKA_ID:
						if(motor_start)
						{
							flash_count++;
							if((second>3 && daka_count==0) || (second>2 && daka_count>0))
							{
								daka_count++;
								second=0;
							}
						}
					default:break;
			}
		}
		else if(ucRxCnt_>=ucRxBuffer_[1] || ucRxCnt_>=50) 
		{
			step=0;
			ucRxCnt_=0;
		}
	}
}



