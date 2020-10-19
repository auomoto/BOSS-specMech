/*------------------------------------------------------------------------------
MCP23008 8-bit Port Expander with I2C (TWI) interface

	init_TWI() must be called first.

------------------------------------------------------------------------------*/ 

#ifndef MCP23008C
#define MCP23008C
#endif

// MCP23008 Registers
#define IODIR	(0x00)	// Pin direction; 1 for input, 0 for output
#define IPOL	(0x01)	// Pin polarity on GPIO; 1 for inverted, not used here
#define GPINTEN	(0x02)	// Interrupt on change; 1 to enable that pin, set DEFVAL & INTCON
#define DEFVAL	(0x03)	// Default comparison value for GPINTEN
#define INTCON	(0x04)	// Interrupt control; 1 compares w/DEFVAL, 0 for pin change
#define IOCON	(0x05)	// Set parameters (see page 15)
#define GPPU	(0x06)	// Pullups; 100K pullup if 1, tri-state if 0 (default)
#define INTF	(0x07)	// Interrupt flag (find out which pin caused the interrupt)
#define INTCAP	(0x08)	// GPIO state when interrupt occurred; cleared when GPIO is read
#define GPIO	(0x09)	// Read for input
#define OLAT	(0x0A)	// Write for output

// Function prototypes
uint8_t read_MCP23008(uint8_t, uint8_t, uint8_t*);
uint8_t write_MCP23008(uint8_t, uint8_t, uint8_t);

/*------------------------------------------------------------------------------
uint8_t read_MCP23008(uint8_t addr, uint8_t reg, uint8_t *val)

	Reads an MCP23008 register, puts the result into val, and returns an error
	code.

	Input:
		addr - MCP23008 hardware address, left shifted to make room for R/W bit.
		reg - MCP23008 register to read

	Returns:
		val - data in that MCP23008 register
		Function return value is 0 if OK, TWI-error if not (see twi.c)

------------------------------------------------------------------------------*/
uint8_t read_MCP23008(uint8_t addr, uint8_t reg, uint8_t *val)
{

	uint8_t retval;

	if ((retval = start_TWI(addr, TWIWRITE))) {
		return(retval);
	}
	write_TWI(&reg, 1);
	if ((retval = start_TWI(addr, TWIREAD))) {
		return(retval);
	}
	read_TWI(val, 1);
	stop_TWI();
	return(0);

}

/*------------------------------------------------------------------------------
uint8_t write_MCP23008(uint8_t addr, uint8_t reg, uint8_t val)

	Writes to an MCP23008 register.

	Input:
		addr - MCP23008 hardware address, left shifted to make room for R/W bit
		reg - The register to select (see #defines above)
		val - The value to put into that register

	Returns:
		0 if OK, TWI error if not (see twi.c)

------------------------------------------------------------------------------*/
uint8_t write_MCP23008(uint8_t addr, uint8_t reg, uint8_t val)
{

	uint8_t retval, data[2];

	addr &= ~0x01;
	if ((retval = start_TWI(addr, TWIWRITE))) {
		return(retval);
	}
	data[0] = reg;
	data[1] = val;
	if ((retval = write_TWI(data, 2))) {
		return(retval);
	}
	stop_TWI();
	return(0);
	
}
