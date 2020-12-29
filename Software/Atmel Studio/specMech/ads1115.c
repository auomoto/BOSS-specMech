/*------------------------------------------------------------------------------
ads1115.c
	16-bit ADC

	There are four possible TWI addresses, obtained by connecting the addr
	pin to one of the other pins on the chip. Device addresses obtained by
	connecting the addr pin to :
		GND - 90h (left shifted one bit)
		VDD - 92h
		SCL - 96h
		SDA - 94h (see special note if you use this)

	We use two registers, CONFIG and CONVERSION and don't do the thresholds.
------------------------------------------------------------------------------*/

#ifndef ADS1115C
#define ADS1115C

#define ADS1115CONFIG		(0x01)	// Config register
#define ADS1115CONVERSION	(0x00)	// Conversion register

#include "twi.c"

// Function prototypes
uint8_t read_ADS1115(uint8_t, uint8_t, uint8_t, uint16_t*);

/*------------------------------------------------------------------------------
uint8_t config_ADS1115(uint8_t, uint8_t confighi, uint8_t configlo)
	Write the CONFIG register (0x01). This is a 16-bit register you load with
	the two bytes, confighi and configlo. Use these instructions to build these
	two bytes before calling this routine.

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
	1: WIndow

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

uint8_t read_ADS1115(uint8_t addr, uint8_t confighi, uint8_t configlo,
	uint16_t *result)
{

	uint8_t retval, hi, lo;

	if ((retval = start_TWI(addr, TWIWRITE))) {
		stop_TWI();
		return(retval);
	}
	if ((retval = write_TWI(ADS1115CONFIG))) {
		stop_TWI();
		return(retval);
	}
	if ((retval = write_TWI(confighi))) {
		stop_TWI();
		return(retval);
	}
	if ((retval = write_TWI(configlo))) {
		stop_TWI();
		return(retval);
	}

	if ((retval = start_TWI(addr, TWIWRITE))) {
		stop_TWI();
		return(retval);
	}
	if ((retval = write_TWI(ADS1115CONVERSION))) {
		stop_TWI();
		return(retval);
	}

	if ((retval = start_TWI(addr, TWIREAD))) {
		stop_TWI();
		return(retval);
	}
	stop_TWI();

	hi = read_TWI();
	lo = read_TWI();
	*result = hi << 8;
	*result &= lo;
	return(0);

}

#endif
