#ifndef ROBOCLAWH
#define ROBOCLAWH

#include "globals.h"

#define MOTORAADDR	128
#define MOTORBADDR	129
#define MOTORCADDR	130
#define ROBOREADFIRMWAREVERSION	21
#define ROBOREADMAINVOLTAGE		24

uint16_t crc16(uint8_t*, uint16_t);
float get_ROBOVoltage(uint8_t);

#endif