/*------------------------------------------------------------------------------
ads1115.c
	16-bit ADC with programmable amplifier

	There are four possible TWI addresses, obtained by connecting the addr
	pin to one of the other pins on the chip. These addresses, left-justified
	to leave byte 0 available for TWI r/w, are (connect the ADDR pin to):
		GND - 90h
		VDD - 92h
		SCL - 96h
		SDA - 94h (see special note in the data sheet if you use this)

	We use only two registers, CONFIG and CONVERSION and don't look at the
	threshold registers.
------------------------------------------------------------------------------*/

#ifndef ADS1115C
#define ADS1115C

#define ADS1115CONFIG		(0x01)	// Config register
#define ADS1115CONVERSION	(0x00)	// Conversion register
#define ADS1115ERROR		(-32768)

#include "twi.c"

// Function prototypes
int16_t read_ADS1115(uint8_t, uint8_t, uint8_t);

/*------------------------------------------------------------------------------
unt16_t read_ADS1115(uint8_t addr, uint8_t confighi, uint8_t configlo)
	Write the CONFIG register (0x01), then perform a single conversion,
	returning the value in 16-bit twos complement. You are responsible for
	creating the CONFIG register values, confighi and configlo.

	Inputs
		addr - TWI address, left shifted one bit
		confighi, config low - CONFIG register

	Output
		Returns the value in the CONVERSION register



	The config register is loaded with the two bytes, confighi and configlo.
	High byte of config register (confighi):
	================================================================|
	|	15	|	14	|	13	|	12	|	11	|	10	|	9	|	8	|
	|---------------------------------------------------------------|
	|	OS	|			MUX			|			PGA			|	MODE|
	|===============================================================|

	OS - Operational Status
	Writing:
	0: No effect
	1: Start a single conversion
	Reading:
	0: Device performing a conversion
	1: Device not performing a conversion
	MUX - Input mux configuration
	000: AINp = AIN0 and AINn = AIN1 (differential, default)
	001: AINp = AIN0 and AINn = AIN3 (differential)
	010: AINp = AIN1 and AINn = AIN3 (differential)
	011: AINp = AIN2 and AINn = AIN3 (differential)
	100: AINp = AIN0 and AINn = GND
	101: AINp = AIN1 and AINn = GND
	110: AINp = AIN2 and AINn = GND
	111: AINp = AIN3 and AINn = GND
	
	PGA - Programmable gain amplifier full scale range in Volts
	000: +/-6.144
	001: +/-4.096
	010: +/-2.048 (default)
	011: +/-1.024
	100: +/-0.512
	101: +/-0.256
	110: +/-0.256
	111: +/-0.256

	MODE - Operating Mode
	0: Continuous conversion
	1: Single shot (default)

	Low byte of config register (configlo):
	====================================================================|
	|	7	|	6	|	5	|	4	  |   3	   |   2	|   1	|	0	|
	|-------------------------------------------------------------------|
	|			DR			|COMP_MODE|COMP_POL|COMP_LAT|   COMP_QUE	|
	|===================================================================|

	DR - Data rate
	000:   8 sps
	001:  16 sps
	101:  32 sps
	011:  64 sps
	100: 128 sps (default)
	101: 250 sps
	110: 475 sps
	111: 860 sps

	COMP_MODE - Comparator mode
	0: Traditional (default)
	1: Window

	COMP_POL - Comparator polarity (ALERT/RDY pin)
	0: Active low (default)
	1: Active high

	COMP_LAT - Latching comparator
	0: Nonlatching
	1: Latching, ALERT/RDY pin latched until data are read

	COMP_QUE - Comparator queue (hysteresis)and disable
	00: Assert after one conversion
	01: Assert after two conversions
	10: Assert after four conversions
	11: Disable comparator and set ALERT/RDY to high impedance (default)
------------------------------------------------------------------------------*/

int16_t read_ADS1115(uint8_t addr, uint8_t confighi, uint8_t configlo)
{

	uint8_t retval;
	uint16_t hi, lo, result;

	if ((retval = start_TWI(addr, TWIWRITE))) {		// TWI start condition
		stop_TWI();
		return(retval);
	}
	if ((retval = write_TWI(ADS1115CONFIG))) {		// Select CONFIG register
		stop_TWI();
		return(retval);
	}

	if ((retval = write_TWI(confighi))) {			// Write high byte
		stop_TWI();
		return(retval);
	}
	if ((retval = write_TWI(configlo))) {			// Write low byte
		stop_TWI();
		return(retval);
	}

	stop_TWI();

// wait for conversion here (1/DataRate time)? Read the OS bit (15) in the
// CONFIG register? Or use hardware interrupts
_delay_ms(8);

	if ((retval = start_TWI(addr, TWIWRITE))) {		// Start condition
		stop_TWI();
		return(ADS1115ERROR);
	}
	if ((retval = write_TWI(ADS1115CONVERSION))) {	// Select conversion register
		stop_TWI();
		return(ADS1115ERROR);
	}

	stop_TWI();

	if ((retval = start_TWI(addr, TWIREAD))) {		// Start condition
		stop_TWI();
		return(ADS1115ERROR);
	}

	hi = (uint16_t) read_TWI();						// Read the conversion register
	lo = (uint16_t) readlast_TWI();
	result = (hi << 8) & lo;						// Pack the result

	return((int16_t) result);

}

#endif
