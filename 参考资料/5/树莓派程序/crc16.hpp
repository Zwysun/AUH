#ifndef __CRC16_H
#define __CRC16_H

#include <stdio.h>
#include <stdint.h>

unsigned short crc_check( uint8_t * pucFrame, unsigned short usLen );
#endif
