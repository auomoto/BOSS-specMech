#include "globals.h"
#include "temperature.h"
#include "mcp9808.h"
#include "ad590.h"

/*------------------------------------------------------------------------------
float get_temperature(uint8_t sensor)
	Return the temperature in degrees C.

	Input
		sensor - 0, 1, 2, or 3 depending on which AD590 sensor you want.
			sensor=3 turns them all on and you get the average.

	Output
		The temperature in C

	We assume that the ADC and sensor have no zero-point errors, that is, we
	did not calibrate the devices but used them as-is out of the box. For our
	purposes this is probably OK but we might want to at least measure a zero-
	point for the actual devices.

	The signal is 1.5E-3 mV/K with a 1500 kOhm dropping resistor.
------------------------------------------------------------------------------*/
float get_temperature(uint8_t sensor)
{

	float temperature;

	switch (sensor) {
		case 0:
		case 1:
		case 2:
			temperature = read_AD590(sensor);
			break;
		case 3:
			temperature = read_MCP9808();
			break;
		default:
			temperature = -666.0;
			break;
	}

	return(temperature);

}
