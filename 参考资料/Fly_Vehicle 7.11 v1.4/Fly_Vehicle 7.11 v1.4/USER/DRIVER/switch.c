#include "switch.h"
/**
  * @brief 电源开关 
  * @param  
  * @note  0  : 关机 1 : 开机
  */	
void PowerON_SwitchSelfHold(void)
{
	POWER_KEY(1);
}
/**
  * @brief 电源开关状态 
  * @param  
	* @note  0  已上电  1 : 已断电
  */	
u8 PowerKeyState(void)
{
	u8 res = 0;
	if(HAL_GPIO_ReadPin(POWER_KEY_STATE_GPIO_Port,POWER_KEY_STATE_Pin) == GPIO_PIN_SET)
	{
		res = 1;
	}
	return res;
}

u8 power_key_sta ;
u8 power_sw = 1 ;  //开机
void SW_Test_Demo(void)
{
	while(1)
	{
		power_key_sta = PowerKeyState();
		POWER_KEY(power_sw);
		delay_ms(100);
	}
	
	
}