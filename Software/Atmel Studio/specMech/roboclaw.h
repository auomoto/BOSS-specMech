#ifndef ROBOCLAWH
#define ROBOCLAWH

#include "globals.h"

#define MOTORAADDR	128
#define MOTORBADDR	129
#define MOTORCADDR	130
#define ROBOREADENCODERCOUNT	16
#define ROBOREADENCODERSPEED	18
#define ROBOREADFIRMWARE		21
#define ROBOREADMAINVOLTAGE		24
#define ROBOREADCURRENT			49
#define ROBOREADTEMPERATURE		82
#define ROBOFAST				64
#define ROBOSLOW				16

uint16_t crc16(uint8_t*, uint16_t);
//uint8_t get_ROBOEncoderValue(uint8_t, uint32_t*);
float get_ROBOFloat(uint8_t, uint8_t);
uint8_t get_ROBOEncoder(uint8_t, uint8_t, uint32_t*);
uint32_t get_ROBOInt32(uint8_t, uint8_t);
uint8_t ROBOMove(uint8_t, uint8_t);
//float get_ROBOTemperature(uint8_t);
//float get_ROBOVoltage(uint8_t);

#endif