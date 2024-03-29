/*------------------------------------------------------------------------------
mma8451.c
	MMA8451 accelerometer on an Adafruit breakout board. This is set up to run
	at +/-2g range, 14 bit resolution, 1.56 Hz sample rate.
------------------------------------------------------------------------------*/

#include "globals.h"
#include "errors.h"
#include "twi.h"
#include "mma8451.h"

/*------------------------------------------------------------------------------
uint8_t get_orientation(float *x, float *y, float *z)
	Put the accelerometer readout into x, y, and z after scaling to 980.6
	cm/s/s gravity.
------------------------------------------------------------------------------*/
uint8_t get_orientation(float *x, float *y, float *z)
{

	uint8_t datain[6];
	int16_t ix, iy, iz;

	*x = *y = *z = BADFLOAT;

	if (read_MMA8451(MMA8451ADDR, MMA8451OUTXMSB, datain, 6) == ERROR) {
		return(ERROR);
	}

	ix = datain[0];
	ix <<= 8;
	ix |= datain[1];
	ix >>= 2;
	*x = 980.6 * ((float) ix/4096.0);

	iy = datain[2];
	iy <<= 8;
	iy |= datain[3];
	iy >>= 2;
	*y = 980.6 * ((float) iy/4096.0);

	iz = datain[4];
	iz <<= 8;
	iz |= datain[5];
	iz >>= 2;
	*z = 980.6 * ((float) iz/4096.0);

	return(0);

}

/*------------------------------------------------------------------------------
uint8_t init_MMA8451(void)
	Initialize accelerometer.

	Since we need information only while tracking, we set up for a slow
	sampling frequency (1.56 Hz), a low cutoff frequency on the high pass
	filter (2.0 Hz), and oversampling and resolution. This gives us good
	consistency (better than 0.5 cm/s/s) at the cost of longer sampling times.
------------------------------------------------------------------------------*/
uint8_t init_MMA8451(void)
{

	uint8_t addr, retval;

	addr = MMA8451ADDR;
	write_MMA8451(addr, MMA8451CTRLREG2, 0x40);			// reset
	read_MMA8451(addr, MMA8451CTRLREG2, &retval, 1);	// Wait to finish
	while (retval & 0x40) {
		read_MMA8451(addr, MMA8451CTRLREG2, &retval, 1);
	}

		// 2.0 Hz high frequency filter cutoff
	write_MMA8451(addr, MMA8451HFCUTOFF, 0b11000000);

		// High resolution mode
	write_MMA8451(addr, MMA8451CTRLREG2, 0b00000010);

		// 1.56 Hz sampling, low noise, set active
	write_MMA8451(addr, MMA8451CTRLREG1, 0b00111101);

	return(retval);

}

/*------------------------------------------------------------------------------
uint8_t read_MMA8451(uint8_t addr, uint8_t reg, uint8_t *val, uint8_t nbyte)

	Reads bytes starting at the MMA8451 "reg," and puts the result into val.

	Input:
		addr - MMA8451 hardware address, left shifted to make room for R/W bit.
		reg - MMA8451 register to read
		nbytes - number of sequential bytes to read

	Returns:
		val - data starting in the MMA8451 register reg
		Function returns 0 if OK, TWI-error if not (see twi.c)
------------------------------------------------------------------------------*/
uint8_t read_MMA8451(uint8_t addr, uint8_t reg, uint8_t *val, uint8_t nbytes)
{

	uint8_t i;

	if (start_TWI(addr, TWIWRITE) == ERROR) {
		stop_TWI();
		return(ERROR);
	}
	if (write_TWI(reg) == ERROR) {
		return(ERROR);
	}
	if (start_TWI(addr, TWIREAD) == ERROR) {
		stop_TWI();
		return(ERROR);
	}
	for (i = 1; i < nbytes; i++) {
		*val++ = read_TWI();
	}
	*val = readlast_TWI();
	stop_TWI();
	return(NOERROR);

}

/*------------------------------------------------------------------------------
uint8_t write_MMA8451(uint8_t addr, uint8_t reg, uint8_t val)

	Writes to an MMA8451 register.

	Input:
		addr - MMA8451 hardware address, left shifted to make room for R/W bit
		reg - The register to select (see #defines above)
		val - The value to put into that register

	Returns:
		0 if OK, TWI error if not (see twi.c)
------------------------------------------------------------------------------*/
uint8_t write_MMA8451(uint8_t addr, uint8_t reg, uint8_t val)
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
