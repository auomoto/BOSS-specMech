/*------------------------------------------------------------------------------
Curiosity Nano ATMega4809 Fujitsu FRAM TWI/I2C interface routines

------------------------------------------------------------------------------*/

#ifndef FRAMC
#define FRAMC
#endif

#define FRAMADDR	(0xA0)	// A0, A1, A2 grounded

// Function Prototypes
uint8_t read_FRAM(uint8_t, uint16_t, uint8_t *, uint8_t);
uint8_t write_FRAM(uint8_t, uint16_t, uint8_t *, uint8_t);

/*------------------------------------------------------------------------------
uint8_t read_FRAM(uint8_t addr, uint16_t memaddr, uint8_t *val, uint8_t nybtes)
	Read nybtes of data from FRAM memory starting at address memaddr and puts
	the result in *val.

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

	uint8_t retval, membytes[2];

	membytes[1] = memaddr & 0xFF;
	membytes[0] = (memaddr >> 8);

	if ((retval = start_TWI(addr, TWIWRITE))) {		// TWI START
		return(retval);
	}

	if ((retval = write_TWI(membytes, 2))) {		// Mem address to read
		return(retval);
	}

	if ((retval = start_TWI(addr, TWIREAD))) {		// Restart
		return(retval);
	}

	read_TWI(val, nbytes);							// Read memory
	stop_TWI();
	return(0);

}

uint8_t write_FRAM(uint8_t addr, uint16_t memaddr, uint8_t *val, uint8_t nbytes)
{

	uint8_t retval, membytes[2];

	membytes[1] = memaddr & 0xFF;
	membytes[0] = (memaddr >> 8);

	if ((retval = start_TWI(addr, TWIWRITE))) {		// TWI START
		return(retval);
	}

	if ((retval = write_TWI(membytes, 2))) {		// Mem address to write
		return(retval);
	}

	if ((retval = write_TWI(val, nbytes))) {		// Write to memory
		return(retval);
	}

	stop_TWI();

	return(0);

}