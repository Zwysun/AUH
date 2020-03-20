#include "stm32f10x.h"//B包含相应的驱动头文件

void motor_init(void); // 电机继电器初始化
void usbl_init(void);//   逆超短基线继电器初始化
void sys_init(void);//    系统初始化
void sw_ctrl(unsigned char sw_ch,unsigned char sw_stage);//配置状态

#define MOTOR_0_EN  0  //宏定义电机0代号
#define MOTOR_1_EN  1
#define MOTOR_2_EN  2
#define MOTOR_3_EN  3
#define COIL_EN 		4

#define USBL_EN     4 //逆超短基线使能


#define sw_switch_off 0  //开关关闭
#define sw_switch_on  1  //开关打开

