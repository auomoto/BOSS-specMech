/*------------------------------------------------------------------------------
ad590.c
	Three AD590 sensors are at GP0, GP2, and GP4 on the MCP23008 port expander.
	GP6 is also wired up on the prototype board (we planned for 4 sensors but
	are implementing only 3). Odd numbered pins are not connected on the
	MCP23008.
	
	The return line on all the AD590 sensors connect to a single the ADC input
	port shunted by a load resistor specified in AD590RESISTOR. Individual
	sensors are activated by powering them individually from the MCP23008 port.

	Inputs:
		Sensor number: 0, 1, or 2

	Output:
		The temperature in C
------------------------------------------------------------------------------*/

#include "globals.h"
#include "errors.h"
#include "ad590.h"
#include "ads1115.h"
#include "mcp23008.h"

/*------------------------------------------------------------------------------
float read_AD590(uint8_t sensor)
	Reports the voltage across the AD590 load resistor for the selected sensor.

	Input:
		sensor - 0, 1, or 2. The sensor is turned on by providing voltage to
			the MCP23008 port pin connected to the sensor.

	Output:
		Returns the temperature. -999.9 for invalid sensor value
------------------------------------------------------------------------------*/
uint8_t read_AD590(uint8_t sensor, float *temperature)
{

	uint8_t pins;
	float voltage, offset;

	offset = 0.0;
	switch (sensor) {		// Select the sensor(s) to turn on
		case 0:
			pins = 0x01;	// t0
			offset = 7.6;
			break;
		case 1:
			pins = 0x04;	// t1
			offset = 0.0;
			break;
		case 2:
			pins = 0x10;	// t2
			offset = 0.0;
			break;
		default:
			pins = 0x00;
			break;
	}

	// Turn on the selected AD590 sensor
	write_MCP23008(AD590DRIVER, GPPU, 0x00);	// Disable pullups on input pins
	write_MCP23008(AD590DRIVER, IODIR, ~pins);	// Pins are inputs if the bit is high
	write_MCP23008(AD590DRIVER, OLAT, pins);	// Set high the selected pins
	_delay_us(20);	// AD590 turn-on time

	// Use 0.512 volts range and 128 samples per second
	if (read_ADS1115(ADC_TE, PGA0512, AIN3, DR128, &voltage) == ERROR) {
		*temperature = BADFLOAT;
		return(ERROR);
	}
	*temperature = (AD590RESISTOR * voltage) - 273.15 + offset;	// Temperature & offset calibration
	return(NOERROR);

}
