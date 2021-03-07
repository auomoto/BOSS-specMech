#ifndef ROBOCLAWH
#define ROBOCLAWH

#include "globals.h"

#define ROBOCOUNTSPERMICRON		268	// Needs checking
//#define ACCELERATION			4096
//#define DECELERATION			4096
//#define SPEED					8192
#define ACCELERATION			8192
#define DECELERATION			8192
#define SPEED					16384
#define MOTORAADDR	128
#define MOTORBADDR	129
#define MOTORCADDR	130
#define ROBOREADENCODERCOUNT	16
#define ROBOREADENCODERSPEED	18
#define ROBOREADFIRMWARE		21
#define ROBOREADMAINVOLTAGE		24
#define ROBOREADCURRENT			49
#define ROBODRIVETO				65
#define ROBOREADTEMPERATURE		82
#define ROBOMOVETO				119
#define ROBOFAST				64
#define ROBOSLOW				16

uint16_t crc16(uint8_t*, uint16_t);
//uint8_t get_ROBOEncoderValue(uint8_t, uint32_t*);
uint8_t get_ROBOEncoder(uint8_t, uint8_t, int32_t*);
float get_ROBOFloat(uint8_t, uint8_t);
uint8_t ROBOGoTo(uint8_t, int32_t);
uint32_t get_ROBOInt32(uint8_t, uint8_t);
uint8_t ROBOMoveAbsolute(uint8_t, int32_t);
uint8_t ROBOMove(uint8_t);
//float get_ROBOTemperature(uint8_t);
//float get_ROBOVoltage(uint8_t);

#endif