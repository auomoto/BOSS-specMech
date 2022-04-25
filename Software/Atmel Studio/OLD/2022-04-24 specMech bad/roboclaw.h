#ifndef ROBOCLAWH
#define ROBOCLAWH

#include "usart.h"
#include "timers.h"
#include "commands.h"
#include "fram.h"
#include "ds3231.h"
#include "errors.h"

#define NMOTORS					(3)
#define SAVEENCODERPERIOD	11			// Save encoder period (sec)

#define SPEED					(7077)		// cts/sec 7077->25um/s
#define ACCELERATION			(SPEED)		// Counts/sec
#define DECELERATION			(SPEED/2)
#define MAXCURRENT				(400)		// mA (Portescap datasheet max is 380 mA)
#define S4MODE					(0x42)		// Home(user)/Limit(fwd)

#define MOTOR_A					128
#define MOTOR_B					129
#define MOTOR_C					130
#define ENC_COUNTS_PER_MICRON	(283)		// 283.1, actually
#define PID_P					(120.0)		// Portescap
#define PID_I					(1.6)		// Portescap
#define PID_D					(480.0)		// Portescap
#define PID_MAXI				(234)		// Portescap
#define PID_DEADZONE			(5)
#define PID_MINPOS				(-850000)	// Encoder counts (3 mm)
#define PID_MAXPOS				(850000)	// 3 mm (894,596 is 3.16 mm)
#define PID_QPPS				(23000)		// Encoder cts/s at max motor speed

// RoboClaw commands
#define STOP					(0)			// Motor stop
#define ENCODERCOUNT			16
#define ENCODERSPEED			18
#define READFIRMWARE			21
#define PUTENCODER				22
#define READMAINVOLTAGE			24
#define SETQPPS					28			// speed
#define READCURRENT				49
#define READQPPS				55
#define SETPID					61			// Position PID constants
#define READPID					63			// Position PID constants
#define DRIVETO					65			// Buffered drive command
#define SETS4MODE				74
#define GETS4MODE				75
#define READTEMPERATURE			82
#define ROBOSTATUS				90			// For limit switch state
#define PUTMAXCURRENT			133
#define GETMAXCURRENT			135

extern volatile uint8_t timerSAVEENCODER;
extern uint8_t timeoutSAVEENCODER;

typedef struct {
	float p, i, d;
	int32_t maxI, deadZone, minPos, maxPos, qpps;
} PID;

uint16_t crc16(uint8_t*, uint16_t);
uint8_t get_FRAM_ENCSAVETIME(char*);
uint8_t get_FRAM_MOTOR_ENCODER(uint8_t, int32_t*);
uint8_t get_MOTOR(uint8_t, uint8_t, uint8_t*, uint8_t);
uint8_t get_MOTOR_CURRENT(uint8_t, uint16_t*);
uint8_t get_MOTOR_ENCODER(uint8_t, int32_t*);
uint8_t get_MOTOR_FLOAT(uint8_t, uint8_t, float*);
//uint8_t get_MOTOR_LIMIT(uint8_t, uint8_t*);
uint8_t get_MOTOR_MAXCURRENT(uint8_t, int32_t*);
uint8_t get_MOTOR_PID(uint8_t, PID*);
uint8_t get_MOTOR_S4MODE(uint8_t, uint8_t*);
uint8_t get_MOTOR_SPEED(uint8_t, int32_t*);
uint8_t get_MOTOR_STATUS(uint8_t, uint32_t*);
uint8_t init_MOTORS(void);
uint8_t motorMoving(uint8_t);
uint8_t motorsMoving(void);
uint8_t move_MOTOR(uint8_t, int32_t);
uint8_t move_MOTOR_CMD(uint8_t);
uint8_t move_MOTOR_HOME(void);
uint8_t move_MOTORS_PISTON(int32_t);
uint8_t put_FRAM_ENCODERS(void);
uint8_t put_MOTOR(uint8_t, uint8_t, uint8_t*, uint8_t);
uint8_t put_MOTOR_ENCODER(uint8_t, int32_t);
//uint8_t put_MOTOR_MAXCURRENT(uint8_t, int32_t);
//uint8_t put_MOTOR_PID(uint8_t, PID);
//uint8_t put_MOTOR_S4MODE(uint8_t, uint8_t);
//uint8_t set_MOTOR_PARAMS(void);
uint8_t stop_MOTOR(uint8_t);
uint8_t stop_MOTORS(void);
void zero_MOTOR_CMD(uint8_t);

#endif