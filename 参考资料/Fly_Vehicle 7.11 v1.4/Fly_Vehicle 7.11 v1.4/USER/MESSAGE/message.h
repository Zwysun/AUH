#ifndef _MESSAGE_H_
#define _MESSAGE_H_

//mavlinkSend.c
#include <string.h>
#include "mathTool.h"
#include "common/mavlink.h"
#include "mavlinkNotice.h"
#include "mavlinkSend.h"
#include "mavlinkDecode.h"
#include "mavlinkParam.h"
//mathTool.c
//#include "mathTool.h"
//#include "bsklink.h"
//#include "common/mavlink.h"
//mavlinkParam.c
//#include "mavlinkParam.h"
//#include <string.h>
//mavlinkDecode.c
//#include "mavlinkDecode.h"
//#include "mavlinkSend.h"
//#include "mavlinkParam.h"
//#include "mavlinkNotice.h"
//#include "common/mavlink.h"
//#include <string.h>
//mavlinkSend.c
//#include "common/mavlink.h"
//#include "mavlinkParam.h"
//#include "mavlinkNotice.h"
//#include "mathTool.h"
//#include "mavlinkNotice.h"
#include "includes.h"
#define MAX_SEND_FREQ   100         //最大发送频率 单位:Hz

#define MAVLINK_SYSTEM_ID       1
#define MAVLINK_COMPONENT_ID    MAV_COMP_ID_AUTOPILOT1

typedef union
{
    int8_t  i8;
    int16_t i16;
    int32_t i32;
    float   f32;
    uint8_t byte[4];
} DATA_TYPE_t;

void MessageInit(void);
void MessageSendLoop(void);
void MessageSensorCaliFeedbackEnable(uint8_t type, uint8_t step, uint8_t success);
//void BsklinkSetMsgFreq(BSKLINK_MSG_ID_FREQ_SETUP_t payload);
//void BsklinkSendEnable(uint8_t msgid);
void MavlinkSendEnable(uint8_t msgid);
void DataSend(uint8_t *data, uint8_t length);
bool GetMessageStatus(void);

#endif


