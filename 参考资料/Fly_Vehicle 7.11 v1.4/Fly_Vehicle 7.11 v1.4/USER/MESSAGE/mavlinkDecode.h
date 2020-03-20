#ifndef _MAVLINKDECODE_H_
#define _MAVLINKDECODE_H_

//#include "mathTool.h"
#include "message.h"
void MavlinkDecode(uint8_t data);
void MavlinkDecodeCommand(mavlink_command_long_t command);
void MavlinkSetFlightMode(mavlink_set_mode_t set_mode);
#endif

