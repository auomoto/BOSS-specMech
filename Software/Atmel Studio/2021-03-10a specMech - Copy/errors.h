#ifndef ERRORSH
#define ERRORSH

#define NOERROR			(0)
#define	ERROR			(99)
#define ERR_GETTIME		(101)	// DS3231 day-time clock not responding
#define ERR_BADCOMMAND	(201)	// Command not recognized
#define ERR_BADOBJECT	(202)	// Object not recognized

#define ERR_UNKNOWNMTR	(301)	// Motor not a, b, c, A, B, or C
#define ERR_MOVEREL		(302)	// Relative move, collimator motor
#define ERR_MTRTIMEOUT	(303)	// Roboclaw does not respond to command
#define ERR_MTRREADENC	(304)	// Error reading motor encoder position or speed
#define ERR_MTRENCCRC	(305)	// CRC error from RoboClaw
#define ERR_MTRSETENC	(306)	// Error setting encoder value at initialization
#define ERR_MTRNULLMOVE	(307)	// No distance or target position specified
#define ERR_MCP23008	(401)	// MCP23008 fail to respond to start condition

void printError(uint16_t, char*);

#endif /* ERRORSH */