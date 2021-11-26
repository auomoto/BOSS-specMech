#ifndef ROBOCLAWH
#define ROBOCLAWH

#include "globals.h"

#define ENC_COUNTS_PER_MICRON	268			// Needs checking
#define ENC_ZEROPOINT			0x3FFFFFFF	// 0x7FFFFFFF is mid-range
#define ACCELERATION			8192		// about 1/2 speed?
#define DECELERATION			8192
#define SPEED					16384		// cts/sec; check for PI motors
#define SAVEENCODERFREQUENCY	11			// Save encoder period (sec)
#define MOTOR_A					128
#define MOTOR_B					129
#define MOTOR_C					130
#define ENCODERCOUNT			16
#define ENCODERSPEED			18
#define ROBOREADFIRMWARE		21
#define PUTENCODER				22
#define SETENCODER				22
#define ROBOSETENCODER			22
#define READMAINVOLTAGE			24
#define ROBOREADCURRENT			49
#define READPID					63			// Position PID
#define SETPID					61			// Position PID constants
#define ROBODRIVETO				65
#define DRIVETO					65			// Buffered drive command
#define READTEMPERATURE			82
#define ROBOMOVETO				119
#define ROBOFAST				64
#define ROBOSLOW				16

extern uint8_t timerSAVEENCODER, timeoutSAVEENCODER;

typedef struct {
	float p, i, d;
	int32_t maxI, deadZone, minPos, maxPos;
} PID;

uint16_t crc16(uint8_t*, uint16_t);
int16_t enc2microns(uint32_t);
uint8_t get_FRAM_MOTOR_ENCODER(uint8_t, int32_t*);
uint8_t get_MOTOR(uint8_t, uint8_t, uint8_t*, uint8_t);
uint8_t get_MOTORCurrent(uint8_t, uint8_t, uint32_t*);
uint8_t get_MOTOR_ENCODER(uint8_t, int32_t*);
uint8_t get_MOTOR_SPEED(uint8_t, int32_t*);
uint8_t get_MOTORFloat(uint8_t, uint8_t, float*);
uint8_t get_MOTOR_PID(uint8_t, PID*);
uint8_t get_MOTORSpeed(uint8_t, int32_t*, uint8_t*);
uint8_t init_MOTORS(void);
uint32_t microns2enc(int16_t);
uint8_t motorsMoving(void);
uint8_t move_MOTOR(uint8_t, int32_t);
uint8_t move_MOTOR_CMD(uint8_t);
//uint8_t move_MOTORAbsolute(uint8_t, uint32_t);
uint8_t move_MOTORAbsolute(uint8_t, int32_t);
//uint8_t putFRAM_MOTOREncoder(uint8_t);
uint8_t putFRAM_MOTOREncoder(uint8_t);
uint8_t put_MOTOR(uint8_t, uint8_t, uint8_t*, uint8_t);
uint8_t put_MOTOR_ENCODER(uint8_t, int32_t);
uint8_t saveFRAM_MOTOREncoders(void);
//uint8_t set_MOTOREncoder(uint8_t, uint32_t);
uint8_t set_MOTOREncoder(uint8_t, int32_t);
uint8_t set_MOTOR_PID(uint8_t, PID);

#endif