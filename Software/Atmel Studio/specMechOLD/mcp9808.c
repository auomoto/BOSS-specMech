/*------------------------------------------------------------------------------
mcp9808.c
	MCP9808 TWI temperature sensor

	This sensor is soldered onto the specMech board and has high accuracy.
	
	The only thing we do is read the ambient temperature register. We leave
	the other registers at their power-up defaults. Most of the other
	registers deal with trip points and alarms. The resolution register
	defaults to the highest resolution of 0.0625 C in the lowest bit.

	Conversion time is 250 ms (4 Hz) at that resolution so the first read
	should wait for that amount of time.

	NOT VERIFIED FOR NEGATIVE TEMPERATURES YET 2021-01-03
------------------------------------------------------------------------------*/

#ifndef MCP9808C
#define MCP9808C

#define MCP9808ADDR		(0x30)	// TWI address
#define TEMPREGISTER	(0x05)	// Ambient temperature register

#include "twi.c"

// Function prototypes
float read_MCP9808(void);

float read_MCP9808(void)
{

	uint8_t highbyte, lowbyte, sign;
	float temperature;

	start_TWI(MCP9808ADDR, TWIWRITE);
	write_TWI(TEMPREGISTER);
	start_TWI(MCP9808ADDR, TWIREAD);
	highbyte = read_TWI();
	lowbyte = readlast_TWI();
	stop_TWI();

	sign = highbyte & 0x10;				// Save sign
	highbyte &= 0x0F;					// Clear threshold & sign bits
	temperature = (((float)highbyte * 16.0) + ((float)lowbyte / 16.0));
	if (sign) {
		temperature = 256.0 - temperature;
	}
	return(temperature);
}

#endif
