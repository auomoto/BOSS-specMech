/*------------------------------------------------------------------------------
fram.c
	Fujitsu MB85RC256V FRAM TWI interface. 32K x 8bit memory.
	
	MOVE ROUTINES TO DO THE SPECIFIC FUNCTIONS TO HERE
------------------------------------------------------------------------------*/

#include "globals.h"
#include "errors.h"
#include "twi.h"
#include "fram.h"

uint8_t get_SETTIME(char *lastsettime)
{

	const char timeset[] = "Time Set?";
	if (read_FRAM(FRAMADDR, SETTIMEADDR, (uint8_t*) lastsettime, ISOTIMELEN) == ERROR) {;
		strcpy(lastsettime, timeset);
		return(ERROR);
	}
	return(NOERROR);
}

/*------------------------------------------------------------------------------
uint8_t read_FRAM(uint8_t addr, uint16_t memaddr, uint8_t *val, uint8_t nybtes)
	Read nybtes of data from FRAM memory starting at memory address memaddr and
	put it in *val.

	Inputs:
		addr: FRAM TWI hardware address
		memaddr: 15-bit memory address to read
		nbytes: Number of bytes to read.

	Outputs:
		val: Array with data

	Returns:
		ERROR on start_TWI error (NACK)
		NOERROR otherwise
------------------------------------------------------------------------------*/
uint8_t read_FRAM(uint8_t addr, uint16_t memaddr, uint8_t *val, uint8_t nbytes)
{

	uint8_t i, memhigh, memlow;

	strcpy((char*) val, "SavedTime?");

	memlow = memaddr & 0xFF;						// high byte
	memhigh = (memaddr >> 8);						// low byte

	if (start_TWI(addr, TWIWRITE) == ERROR) {		// TWI start
		stop_TWI();
		return(ERROR);
	}

	if (write_TWI(memhigh) == ERROR) {			// Send memory address
		return(ERROR);
	}
	if (write_TWI(memlow) == ERROR) {
		return(ERROR);
	}

	if (start_TWI(addr, TWIREAD) == ERROR) {		// Repeated start
		return(ERROR);
	}

	for (i = 0; i < (nbytes-1); i++) {
		val[i] = read_TWI();						// Read memory
	}
	val[nbytes-1] = readlast_TWI();

	stop_TWI();

	return(NOERROR);

}

/*------------------------------------------------------------------------------
uint8_t write_FRAM(uint8_t addr, uint16_t memaddr, uint8_t *val, uint8_t nybtes)
	Write nybtes to FRAM memory starting at address memaddr.

	Inputs:
		addr: FRAM hardware address.
		memaddr: 15-bit memory address at which to write
		val: Data to write
		nbytes: Number of bytes to write.

	Returns:
		ERROR if start_TWI fails (NACK)
		NOERROR otherwise
------------------------------------------------------------------------------*/
uint8_t write_FRAMx(uint8_t addr, uint16_t memaddr, uint8_t *val, uint8_t nbytes)
{

	uint8_t i, memhigh, memlow;

	memlow = memaddr & 0xFF;
	memhigh = (memaddr >> 8);

	if (start_TWI(addr, TWIWRITE) == ERROR) {	// TWI START
		return(ERROR);
	}

	if (write_TWI(memhigh) == ERROR) {			// FRAM address to write
		return(ERROR);
	}
	if (write_TWI(memlow) == ERROR) {			// FRAM address to write
		return(ERROR);
	}

	for (i = 0; i < nbytes; i++) {
		if (write_TWI(*val++) == ERROR) {		// Write to FRAM
			return(ERROR);
		}
	}

	stop_TWI();

	return(NOERROR);

}

uint8_t write_FRAM(uint8_t addr, uint8_t item, uint8_t *val)
{

	uint8_t i, memhigh, memlow, nbytes;
	uint16_t memaddr;

	switch (item) {
		case (SETTIMEFRAM):
			memaddr = SETTIMEADDR;
			nbytes = ISOTIMELEN;
			break;

		default:
			return(ERROR);
			break;

	}

	memlow = memaddr & 0xFF;
	memhigh = (memaddr >> 8);

	if (start_TWI(addr, TWIWRITE) == ERROR) {	// TWI START
		return(ERROR);
	}

	if (write_TWI(memhigh) == ERROR) {			// FRAM address to write
		return(ERROR);
	}
	if (write_TWI(memlow) == ERROR) {			// FRAM address to write
		return(ERROR);
	}

	for (i = 0; i < nbytes; i++) {
		if (write_TWI(*val++) == ERROR) {		// Write to FRAM
			return(ERROR);
		}
	}

	stop_TWI();

	return(NOERROR);

}
