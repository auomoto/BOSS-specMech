#ifndef ROBOCLAWH
#define ROBOCLAWH

#include "globals.h"

#define ROBOCOUNTSPERMICRON		268	// Needs checking
#define ACCELERATION			8192
#define DECELERATION			8192
#define SPEED					16384
#define SAVEENCODERFREQUENCY	11	// Save encoder period (sec)
#define MOTORAADDR	128
#define MOTORBADDR	129
#define MOTORCADDR	130
#define ROBOREADENCODERCOUNT	16
#define ROBOREADENCODERSPEED	18
#define ROBOREADFIRMWARE		21
#define ROBOSETENCODER			22
#define ROBOREADMAINVOLTAGE		24
#define ROBOREADCURRENT			49
#define ROBODRIVETO				65
#define ROBOREADTEMPERATURE		82
#define ROBOMOVETO				119
#define ROBOFAST				64
#define ROBOSLOW				16

extern uint8_t timerSAVEENCODER, timeoutSAVEENCODER;

uint16_t crc16(uint8_t*, uint16_t);
uint8_t getFRAM_MOTOREncoder(uint8_t, int32_t*);
uint8_t get_MOTOREncoder(uint8_t, uint8_t, int32_t*);
uint8_t get_MOTORFloat(uint8_t, uint8_t, float*);
uint8_t get_MOTORInt32(uint8_t, uint8_t, uint32_t*);
uint8_t init_MOTORS(void);
uint8_t motorsMoving(void);
uint8_t move_MOTOR(uint8_t);
uint8_t move_MOTORAbsolute(uint8_t, int32_t);
uint8_t putFRAM_MOTOREncoder(uint8_t);
uint8_t saveFRAM_MOTOREncoders(void);
uint8_t set_MOTOREncoder(uint8_t, int32_t);

#endif