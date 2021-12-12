/*------------------------------------------------------------------------------
MCP23008.c
	MCP23008 8-bit port expander with I2C (TWI) interface.
	init_TWI() must be called first.
	
	The 7-bit address is 0b0100AAAX where AAA are the three address pins in
	A2, A1, and A0 order. If all three pins are grounded, the 7-bit address
	is 0x20.

	Convention:
		Write to OLAT, read from GPIO
------------------------------------------------------------------------------*/

#include "globals.h"
#include "errors.h"
#include "twi.h"
#include "mcp23008.h"

/*------------------------------------------------------------------------------
uint8_t read_MCP23008(uint8_t addr, uint8_t reg, uint8_t *val)

	Reads an MCP23008 register, puts the result into val.

	Input:
		addr: MCP23008 hardware address
		reg: MCP23008 register to read

	Output:
		val - data in that MCP23008 register

	Returns:
		ERROR if TWI error
		NOERROR Otherwise
------------------------------------------------------------------------------*/
uint8_t read_MCP23008(uint8_t addr, uint8_t reg)
{

	uint8_t value;

	if (start_TWI(addr, TWIWRITE) == ERROR) {
		printError(ERR_MCP23008, "MCP23008 read error");
		stop_TWI();
		return(0xFF);
	}
	write_TWI(reg);
	if (start_TWI(addr, TWIREAD) == ERROR) {
		printError(ERR_MCP23008, "MCP23008 read error");
		stop_TWI();
		return(0xFF);
	}
	value = readlast_TWI();
	stop_TWI();
	return(value);

}

/*
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
*/


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

	if (start_TWI(addr, TWIWRITE) == ERROR) {
		printError(ERR_MCP23008, "MCP23008 write error");
		stop_TWI();
		return(ERROR);
	}
	if ((retval = write_TWI(reg))) {
		stop_TWI();
		return(ERROR);
	}
	if ((retval = write_TWI(val))) {
		stop_TWI();
		return(ERROR);
	}
	stop_TWI();
	return(NOERROR);
	
}
