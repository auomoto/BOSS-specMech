/*------------------------------------------------------------------------------
MCP23008.c
	MCP23008 8-bit port expander with I2C (TWI) interface
	init_TWI() must be called first
------------------------------------------------------------------------------*/ 

#include "globals.h"

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
	if ((retval = write_TWI(reg))) {
		return(retval);
	}
	if ((retval = start_TWI(addr, TWIREAD))) {
		return(retval);
	}
	*val = readlast_TWI();
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

	uint8_t retval;

	if ((retval = start_TWI(addr, TWIWRITE))) {
		stop_TWI();
		return(retval);
	}
	if ((retval = write_TWI(reg))) {
		stop_TWI();
		return(retval);
	}
	if ((retval = write_TWI(val))) {
		stop_TWI();
		return(retval);
	}
	stop_TWI();
	return(0);
	
}
