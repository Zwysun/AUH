#include "switch.h"
/**
  * @brief ��Դ���� 
  * @param  
  * @note  0  : �ػ� 1 : ����
  */	
void PowerON_SwitchSelfHold(void)
{
	POWER_KEY(1);
}
/**
  * @brief ��Դ����״̬ 
  * @param  
	* @note  0  ���ϵ�  1 : �Ѷϵ�
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
u8 power_sw = 1 ;  //����
void SW_Test_Demo(void)
{
	while(1)
	{
		power_key_sta = PowerKeyState();
		POWER_KEY(power_sw);
		delay_ms(100);
	}
	
	
}