#ifndef __STM32_PI_H
#define __STM32_PI_H

#include "my_scc.h"
/****************remote message id*****************/
#define MSG_TEMP_HUMI_ID    0x00
#define MSG_PRESSURE_ID     0x01
#define MSG_COMPASS_ID      0x02
#define MSG_POWER_ID        0x03
#define MSG_CMD_ID          0x04
#define MSG_GIMBAL_ID       0x05
#define MSG_LIGHT_ID        0x06
#define MSG_PID_ID          0x07
#define MSG_STATE_ID        0x08
#define MSG_TEST_ID         0x09
#define MSG_PHONE_ID  			0X22
#define MSG_GPS_ID          0x20
#define MSG_BLANCE_EN_ID    0X21

#define MSG_NAVI_ID         0X23
#define MSG_DAKA_ID         0X24
#define MSG_SHIBIE_ID       0X25
extern float navi_center_to_line_angle;   //中心到色带角度
extern float navi_center_to_line_distance;//中心到色带距离
extern u8 find_line_flag;
void Pi_Stm32_unpackData(unsigned char ucData);
#endif
