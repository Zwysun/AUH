#ifndef __INCLUDES_H
#define __INCLUDES_H
//Cube生成
#include "main.h"
#include "cmsis_os.h"
#include "adc.h"
#include "dma.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h" 
//C标准库
#include "string.h"
#include "stdint.h"
#include "stdbool.h"
#include "math.h"
//自定义 
#define	u8  unsigned char
#define u16 unsigned short int
#define	u32 unsigned int
//Drivers
#include "board.h"
#include "SHT35.h"
#include "batt.h"
#include "hp206c_10ba.h"
#include "jy901.h"
#include "sx1278.h"
#include "led.h"
#include "switch.h"
#include "pwm.h"
#include "delay.h"
#include "myiic.h"


//Control
#include "control.h"
#include "stm32_pi.h"
#include "PID.h"

//任务
#include "Compass_Task.h" 
#include "Control_Task.h"
#include "Message_Task.h"
#include "Pressure_Task.h"
#include "Sensor_Task.h"
#include "ledTasks.h"


//通信协议
//#define DEBUG

 

#endif
