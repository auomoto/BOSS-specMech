/*------------------------------------------------------------------------------
fram.c
	Fujitsu MB85RC256V FRAM TWI interface. 32K x 8bit memory.
------------------------------------------------------------------------------*/

#ifndef FRAMC
#define FRAMC

#define FRAMADDR	(0xA0)	// A0, A1, A2 grounded

// Function Prototypes
uint8_t read_FRAM(uint8_t, uint16_t, uint8_t *, uint8_t);
uint8_t write_FRAM(uint8_t, uint16_t, uint8_t *, uint8_t);

/*------------------------------------------------------------------------------
uint8_t read_FRAM(uint8_t addr, uint16_t memaddr, uint8_t *val, uint8_t nybtes)
	Read nybtes of data from FRAM memory starting at memory address memaddr and
	put it in *val.

	Inputs:
		addr - FRAM hardware address, left shifted to make room for the R/W
			bit. The address is 0b1010AAAX where AAA are the three hardware
			address pins and X is the R/W bit.

		memaddr - 15-bit memory address.

		nbytes - Number of bytes to read.

	Returns:
		val - Array with data
		Function returns 0 if OK, TWI-error if not (see twi.c for these)

------------------------------------------------------------------------------*/
uint8_t read_FRAM(uint8_t addr, uint16_t memaddr, uint8_t *val, uint8_t nbytes)
{

	uint8_t i, retval, memhigh, memlow;

	memlow = memaddr & 0xFF;						// high byte
	memhigh = (memaddr >> 8);						// low byte

	if ((retval = start_TWI(addr, TWIWRITE))) {		// TWI start
		return(retval);
	}

	if ((retval = write_TWI(memhigh))) {			// Send memory address
		return(retval);
	}
	if ((retval = write_TWI(memlow))) {
		return(retval);
	}

	if ((retval = start_TWI(addr, TWIREAD))) {		// Repeated start
		return(retval);
	}

	for (i = 0; i < (nbytes-1); i++) {
		val[i] = read_TWI();						// Read memory
	}
	val[nbytes-1] = readlast_TWI();

	stop_TWI();

	return(0);

}

/*------------------------------------------------------------------------------
uint8_t write_FRAM(uint8_t addr, uint16_t memaddr, uint8_t *val, uint8_t nybtes)
	Write nybtes to FRAM memory starting at address memaddr.

	Inputs:
		addr - FRAM hardware address, left shifted to make room for the R/W
			bit. The address is 0b1010AAAX where AAA are the three hardware
			address pins and X is the R/W bit.
		memaddr - 15-bit memory address.
		nbytes - Number of bytes to write.

	Returns:
		val - Array with data
		Function returns 0 if OK, TWI-error if not (see twi.c for these)
------------------------------------------------------------------------------*/
uint8_t write_FRAM(uint8_t addr, uint16_t memaddr, uint8_t *val, uint8_t nbytes)
{

	uint8_t i, retval, memhigh, memlow;

	memlow = memaddr & 0xFF;
	memhigh = (memaddr >> 8);

	if ((retval = start_TWI(addr, TWIWRITE))) {	// TWI START
		return(retval);
	}

	if ((retval = write_TWI(memhigh))) {		// FRAM address to write
		return(retval);
	}
	if ((retval = write_TWI(memlow))) {			// FRAM address to write
		return(retval);
	}

	for (i = 0; i < nbytes; i++) {
		if ((retval = write_TWI(*val++))) {		// Write to FRAM
			return(retval);
		}
	}

	stop_TWI();

	return(0);

}

#endif
