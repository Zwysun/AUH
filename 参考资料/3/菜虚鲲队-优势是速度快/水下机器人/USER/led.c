#include "led.h" 
#include "delay.h" 
u8 flash_count=0;
u8 led_count=0;
u8 led_int_flag=0;
void LED_Init(void)
{    	 
  GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOE, ENABLE);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOE, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_ResetBits(GPIOB,GPIO_Pin_8|GPIO_Pin_9);
	GPIO_ResetBits(GPIOE,GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5);
}
void LED_SW(u8 num,u8 sta)
{
	if(num==1 || num==9) GPIO_WriteBit(GPIOE,GPIO_Pin_3,(sta?Bit_SET:Bit_RESET));
	if(num==2 || num==9) GPIO_WriteBit(GPIOE,GPIO_Pin_2,(sta?Bit_SET:Bit_RESET));
	if(num==3 || num==9) GPIO_WriteBit(GPIOE,GPIO_Pin_1,(sta?Bit_SET:Bit_RESET));
	if(num==4 || num==9) GPIO_WriteBit(GPIOE,GPIO_Pin_0,(sta?Bit_SET:Bit_RESET));
	if(num==5 || num==9) GPIO_WriteBit(GPIOB,GPIO_Pin_9,(sta?Bit_SET:Bit_RESET));
	if(num==6 || num==9) GPIO_WriteBit(GPIOB,GPIO_Pin_8,(sta?Bit_SET:Bit_RESET));
	if(num==7 || num==9) GPIO_WriteBit(GPIOE,GPIO_Pin_5,(sta?Bit_SET:Bit_RESET));
	if(num==8 || num==9) GPIO_WriteBit(GPIOE,GPIO_Pin_4,(sta?Bit_SET:Bit_RESET));
}
void start_led(void)
{
	u8 i;
	LED_SW(9,0);
	for(i=1;i<=8;i++)
	{
		LED_SW(i,1);
		delay_ms(400);
	}
}
void led_flash(u16 i)
{
	while(i--)
	{
		LED_SW(9,0);
		delay_ms(30);
    LED_SW(9,1);
		delay_ms(30);
	}
}

void led_flash_int(void)
{
	if(led_int_flag)
	{
		if(flash_count>0)
		{
			if(led_count<5) LED_SW(9,1);
			else LED_SW(9,0);
			led_count++;
			if(led_count>9) 
			{
				led_count=0;
				flash_count--;
			}
		}
		else 
		{
			led_count=0;
			LED_SW(9,0);
		}
	}
}




