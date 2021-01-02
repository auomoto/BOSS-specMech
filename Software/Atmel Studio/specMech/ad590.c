/*------------------------------------------------------------------------------
ad590.c
	Three AD590 sensors are at GP0, GP2, and GP4 on the MCP23008 port expander.
	GP6 is also wired up on the prototype board (we planned for 4 sensors but
	are implementing only 3). Odd numbered pins are not connected on the
	MCP23008.
	
	The return line on all the AD590 sensors connect to a single the ADC input
	port shunted by a load resistor (1k0 Ohm on the test items, 1k5 Ohm onthe
	production version). Individual sensors are chosen by powering them
	individually from the MCP23008 port.
	
	If we power all the sensors, we can measure the average temperature of the
	three sensors by setting the reference voltage to, say, 2.048 instead of
	0.512 for a single sensor.

	Inputs:
		Sensor number: 0, 1, 2, or 3 where sensor 3 returns the average of
		the sensors.

	Output:
		The digital value on the ADS1115 ADC, in the 0 V to 0.512 V full scale
		range. Or ADS1115ERROR if an invalid sensor is chosen (that is, not
		0, 1, or 2)
------------------------------------------------------------------------------*/

#ifndef AD590C
#define AD590C

// MCP23008 address
//#define AD590DRIVER		(0x4E)	// MCP23008 address
#define AD590DRIVER			(0x40)	// Test unit MCP23008 address

// ADS1115 (ADC) TWI addresses
#define ADC_TE	0b10010000			// ADDR pin connected to GND (temperature)
#define ADC_RH	0b10010000			// Relative Humidity (same unit as temperature)
#define ADC_IP	0b01001001			// ADDR pin connected to VDD (ion pump)

//#define AD590RESISTOR	(1500.0)	// Ohms
#define AD590RESISTOR	(1000.0)	// Ohms

#include "mcp23008.c"
#include "ads1115.c"

// Function prototypes
float read_AD590(uint8_t sensor);

/*------------------------------------------------------------------------------
float read_AD590(uint8_t sensor)
	Reports the voltage across the AD590 load resistor for the selected sensor.

	Input:
		sensor - 0, 1, or 2

	Output:
		Returns the voltage at the load resistor.
------------------------------------------------------------------------------*/
float read_AD590(uint8_t sensor)
{

	uint8_t pins;
	float value;

	switch (sensor) {		// Select the sensor(s) to turn on
		case 0:
// The alternates are for the prototype ADS1115 board:
			pins = 0x80;
//			pins = 0x01;	// t0
			break;
		case 1:
			pins = 0x40;
//			pins = 0x04;	// t1
			break;
		case 2:
			pins = 0x20;
//			pins = 0x10;	// t2
			break;
		default:
			pins = 0x00;
			break;
	}

	// Turn on the selected AD590
	write_MCP23008(AD590DRIVER, GPPU, 0x00);	// Disable pullups on input pins
	write_MCP23008(AD590DRIVER, IODIR, ~pins);	// Pins are inputs if the bit is high
	write_MCP23008(AD590DRIVER, OLAT, pins);	// Set high the selected pins
	_delay_us(20);	// AD590 turn-on time

	// Use 0.512 volts range and 128 samples per second
//	value = read_ADS1115(ADC_TE, PGA0512, AIN3, DR128);	// on specMech board
	value = read_ADS1115(ADC_TE, PGA0512, AIN2, DR128);	// on test board

	return(value);

}

#endif