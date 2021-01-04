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

#define ADS1115ERROR		(-32768)

// ADS1115 Registers
#define ADS1115CONFIG		(0x01)	// Config register
#define ADS1115CONVERSION	(0x00)	// Conversion register

// ADS1115 TWI device addresses
#define ADC_TE	0b10010000		// ADDR pin connected to GND (temperature)
#define ADC_RH	0b10010000		// Relative Humidity (same unit as temperature)
#define ADC_IP	0b10010010		// ADDR pin connected to VDD (ion pump)

// ADS1115 analog input pin (bits 6, 5, & 4)
#define AIN0	0b01000000		// 100
#define AIN1	0b01010000		// 101
#define AIN2	0b01100000		// 110
#define AIN3	0b01110000		// 111
#define AIN01	0b00000000		// 000 Differential between AIN0 and AIN1
#define AIN23	0b00110000		// 011 Differential between AIN2 and AIN3

// ADS1115 programmable gain amp setting (bits 3, 2, 1 in MSB config)
#define PGA0256	0b00001010		// 101	+/-0.256V
#define PGA0512	0b00001000		// 100	+/-0.512V
#define PGA1024	0b00000110		// 011	+/-1.024V
#define PGA2048	0b00000100		// 010	+/-2.048V
#define PGA4096	0b00000010		// 001	+/-4.096V
#define PGA6144	0b00000000		// 000	+/-6.144V
#define SCA0256	(7.812738E-6)	// 15-bit scale
#define SCA0512	(1.562547E-5)
#define SCA1024	(3.125095E-5)
#define SCA2048	(6.250119E-5)
#define SCA4096	(1.250038E-4)
#define SCA6144	(1.875057E-4)

// ADC data rates in samples per second
// Top 3 bits of lower CONFIG register
#define DR008	0x00000000
#define DR016	0b00100000
#define DR032	0b01000000
#define DR064	0b01100000
#define DR128	0b10000000	// Default
#define DR250	0b10100000
#define DR475	0b11000000
#define DR860	0b11100000

// Function Prototypes
float read_ADS1115(uint8_t, uint8_t, uint8_t, uint8_t);

/*------------------------------------------------------------------------------
float read_ADS1115(uint8_t addr, uint8_t gain, uint8_t pins, uint8_t datarate)
	Returns the voltage read by an ADS1115 ADC. The address is the TWI address
	left-shifted and zero-filled by one bit (for the R/W bit). The gain selects
	the programmable gain amplifier setting (values are in the #defines at the
	top of this listing). The pin selects the input pin configuration, either
	single-ended or differential. The datarate selects the obvious.

	Quickstart guide is on page 30 of the data sheet. The steps are:

	1. Write to the config register. This example starts a conversion.
		- First byte:	0b10010000	(low bit 0 says we're TWI writing)
		- Second byte:	0b00000001	(select config register)
		- Third byte:	0b10000100	(MSB of config register to be written)
		- Fourth byte:	0b10000011	(LSB of config register to be written)

	2. Write to address pointer register
		- First byte:	0b10010000	(low bit 0 says we're writing)
		- Second byte:	0b00000000	(select conversion register)

	3. Read conversion register
		- First byte:	0b10010001	(high bit 0 says we're reading)
		- Second byte:	(MSB of conversion register)
		- Third byte:	(LSB of conversion register)

	TWI addresses are set by connecting the address pin (pin 1 on the chip)
	to one of GND, VDD, SCL (pin 10). These are the resulting 7-bit TWI
	addresses:
		---------------------
		|	GND	|	1001000	|
		|	VDD	|	1001001	|
		|	SCL	|	1001011	|
		---------------------
	There are more address options but things get complicated. We only use two
	of these so we don't need to worry about that. The above are the 7-bit
	device addresses and our TWI routines need to left-shift by one bit to
	make room for the R/W bit.

	The 2020-07-28 ADS1115 ADC prototype board ties the address pin to GND so
	the 8-bit TWI address is 0b10010000.

	The ADS1115 has 5 registers:
		Address pointer (8-bit, points to the others)
		Conversion (16-bit, holds the result)
		Config (16-bit sets input pin, data rates, reference voltages, etc.)
		Lo_thresh (16-bit, comparator setup)
		Hi_thresh (16-bit, comparator setup)
	We do not use the threshhold registers.

	ADDRESS POINTER
	Write to this register to select the other register to read/write. When
	writing to the config register, immediately send the two bytes to load.
	This byte is zero except for bits 0 and 1. Values are:
		00:	Conversion register
		01:	Config register
		10: Lo_thresh register
		11:	Hi_thresh register
	The two we use in this routine are:
	Conversion register:	0b00000000
	Config register:		0b00000001

	CONFIGURATION REGISTER (address pointer 0b00000001)
	The configuration Register has 16 bits. The top eight are:
	-----------------------------------------------------------------
	|  15	|  14	|  13	|  12	|  11	|  10	|   9	|   8	| BIT
	|  OS	|		  MUX			|		   PGA			|  MODE	| NAME
	-----------------------------------------------------------------
	|  1h	|			0h			|			2h			|	1h	| DEFAULTS
	-----------------------------------------------------------------

	OS is the operational state. Setting OS=1 when means start conversion.
	On reading, this bit is 0 during a conversion and becomes 1 when a
	conversion is done. Since conversions take a long time, we poll this
	bit before reading the conversion register. Perhaps we should use
	the RDY pin in hardware to generate an interrupt instead of polling.

	MUX value selects the input pin to use. Single-ended 100, 101, 110,
		& 111 indicate AIN0, AIN1, AIN2, and AIN3.

	PGA selects the programmable amplifier gain. Values of
		=101 (+/-0.256V), =100 (+/-0.512V), =011 (+/-1.024V),
		=010 (+/-2.048V), =001 (+/-4.096V), =000 (+/-6.144V)
	set those full scale voltages.
	These numbers indicate the full scale (16-bit) values but the actual
	measurable voltage range is limited by VDD.

	MODE bit =0 for continuous conversion, =1 for single conversion.
	Example for high byte:
		0b11100101	Start conversion (bit 15) on AIN2 (bits 14..12)
		using +/-2.048V range (bits 11..9), single shot (bit 8)

	The lower eight bits are:
	-----------------------------------------------------------------
	|	7	|   6	|   5	|   4	|   3	|   2	|   1	|   0	|
	|		   DR			|COMP_MD|COMP_PO|COMP_LA|	COMP_QUE	|
	-----------------------------------------------------------------
	|			4h			|  0h	|  0h	|  0h	| 3h (disabled)	|
	-----------------------------------------------------------------
	DR (Data Rate in Samples per Second). Values may be:
		=000 (8), =001 (16), =010 (32), =011 (64),
		=100 (128 SPS), =101 (250), =110 (475), =111 (860)

	COMP_<thing> are comparator settings we won't use (COMP_QUE=3h)

	Default value is 0b10000011 for 128 samples per second. We use this
	exclusively.

	CONVERSION REGISTER
	This contains the result. First send a read command to the ADS1115 then
	read the two bytes coming back.
---
	COMPUTING THE RESULT
	The high end result (CONVERSION REGISTER) is 0x7FFF or 32767. The scaling
	for the available amplifier gains are:

	-------------------------------------------------
	|  PGA	| Range	|	Compute		|    Scale		|
	-------------------------------------------------
	|  101	| 0.256	|  0.256/32767	|  7.812738E-6	|
	|  100	| 0.512	|  0.512/32767	|  1.562547E-5	|
	|  011	| 1.024	|  1.024/32767	|  3.125095E-5	|
	|  010	| 2.048	|  2.048/32767	|  6.250119E-5	|
	|  001	| 4.096	|  4.096/32767	|  1.250038E-4	|
	|  000	| 6.144	|  6.144/32767	|  1.875057E-4	|
	-------------------------------------------------
	where the Scale is the voltage change in the LSB.

	Inputs:
		addr - TWI address of the ADS1115, left shifted one bit with bit 0, the
			TWI R/W bit, being 0.
		gain - One of the six possible gain settings.
		pins - The input pin or pins to be read. These can be either single-
			ended (positive only) or differential.
		datarate - one of the eight selectable data rates. We chose the
			default of 128 samples per second although we haven't explored
			any other options.
------------------------------------------------------------------------------*/
float read_ADS1115(uint8_t addr, uint8_t gain, uint8_t pins, uint8_t datarate)
{

	int value;
	float scale, voltage;
	uint8_t confighi, configlo, retval, flag;
	uint8_t converting, lowbyte, highbyte;

	// Write the CONFIG register
	confighi = 0b10000001 | gain | pins;
	configlo = datarate;					// Change to add conversion ready?

	if ((retval = start_TWI(addr, TWIWRITE))) {		// TWI start condition
		stop_TWI();
		return(retval);
	}
	if ((retval = write_TWI(ADS1115CONFIG))) {		// Write the CONFIG register
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

	_delay_us(25);									// Power-up time

	converting = YES;
	while (converting) {							// Wait for conversion to finish
		start_TWI(addr, TWIREAD);
		flag = readlast_TWI();				// CHANGE TO HARDWARE SENSE ON RDY PIN?
		if (flag & 0b10000000) {			// or add a timer timeout
			converting = NO;
		}
	}
	stop_TWI();

	start_TWI(addr, TWIWRITE);
	write_TWI(ADS1115CONVERSION);					// Start the ADC conversion
	start_TWI(addr, TWIREAD);						// Read the data
	highbyte = read_TWI();
	lowbyte = readlast_TWI();
	stop_TWI();

	value = highbyte;
	value = value << 8;
	value |= lowbyte;

	switch (gain) {									// Convert to voltage
		case PGA0256:
			scale = SCA0256;
			break;
		case PGA0512:
			scale = SCA0512;
			break;
		case PGA1024:
			scale = SCA1024;
			break;
		case PGA2048:
			scale = SCA2048;
			break;
		case PGA4096:
			scale = SCA4096;
			break;
		case PGA6144:
			scale = SCA6144;
			break;
		default:
			scale = 0.0;
			break;
	}

	voltage = scale * (float) value;
	return(voltage);

}

#endif