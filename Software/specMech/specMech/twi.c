/*------------------------------------------------------------------------------
Curiosity Nano ATMega4809 Two Wire Interface (TWI or I2C) routines

------------------------------------------------------------------------------*/ 

#ifndef TWIC
#define TWIC
#endif

// TWI defines
#define TWIFREQ		100000UL		// This is fastest for F_CPU = 3.33 MHz
#define TWIBAUD		((uint8_t) (F_CPU/(2*TWIFREQ)) - 5)	// Ignore rise time
#define TWIWRITE	0
#define TWIREAD		1
#define TWIBUSERR	2	// These flags are unlikely given our single-master
#define TWIARBLOST	3	// configuration and soldered-in hardware. They may
#define TWIACKERR	4	// be useful for troubleshooting hardware failures.

// Function Prototypes
void init_TWI(void);
void read_TWI(uint8_t*, uint8_t);
uint8_t start_TWI(uint8_t, uint8_t);
void stop_TWI(void);
uint8_t write_TWI(uint8_t*, uint8_t);

/*------------------------------------------------------------------------------
void init_TWI(void)

	Initialize the TWI interface on a megaAVR 0-series processor.
	
	On the ATMega4809:
		SDA is on PA2
		SCL is on PA3
	
	In the data sheet, ATmega4808-4809-Data-Sheet-DS40002173A.pdf, the TWI
	interface information is in section 25, starting on page 326.

	Initialization instructions are in section 25.3.1 (p327):

	1.	Set SDASETUP and SDAHOLD in TWI.CTRLA. These are for SMB operation
		so we will use the startup defaults (do nothing).

	2a.	Write Master Baud Rate Register TWIn.BAUD *before* enabling TWI master.
		The data sheet formula for F_SCL, the TWI frequency is:
			F_SCL = F_CPU/(10 + 2*BAUD + F_CPU*T_RISE) or
			BAUD = (F_CPU/2*F_SCL) - 5 - ((F_CPU*T_RISE)/2)
		These are defined as macros at the top of this file and changing TWIFREQ
		(defined in Hz) controls the baud rate.

		Note that the baud rate for the default 3.33 MHz processor clock on the
		ATMega4809 limits you to 100000Hz.

		The I/O pin rise time (T_RISE) is 1.5 ns with 20 pF loading at 5V (p470),
		so we will ignore the rise time:
			BAUD = ((F_CPU/2*F_SCL) - 5)
			BAUD = 12 gives F_SCL =  98015 for F_CPU=3333333
			BAUD = 11 gives F_SCL = 104140 for F_CPU=3333333

	2b.	Smart mode (25.3.4.4 p337) is enabled by setting the SMEN bit in
		TWI.MCTRLA. Smart mode causes the TWI interface to send automatically
		an ACK when the master data register TWI.MDATA is read. The ACKACT bit
		must also be set to ACK (cleared bit) in TWI.MCTRLB (this is the default;
		the data sheet seems to have an error in saying this bit is in TWI.MCTRLA).
		There seems to be little gain to using this and the code becomes a little
		more obscure, so we won't use this feature.

	3.	Write a '1' to the ENABLE bit in TWIn.MCTRLA.

	4.	Set bus state to IDLE by writing 0x01 to BUSSTATE in TWIn.MSTATUS.

------------------------------------------------------------------------------*/
void init_TWI(void)
{

	TWI0.MBAUD = TWIBAUD;
	TWI0.MCTRLA |= TWI_ENABLE_bm;			// Enable TWI
	TWI0.MSTATUS |= TWI_BUSSTATE_IDLE_gc;	// Set bus state to IDLE

}

/*------------------------------------------------------------------------------
void read_TWI(uint8_t *result, uint8_t nbytes)

	Read nbytes from the TWI bus into *result.

	A start condition and a high R/W bit must preceed this call.
	
	The 0-series AVR processors handle this somewhat differently than previous
	versions. Writing to TWI0.MADDR puts a start condition on the bus, transmits
	the slave address, and if the R/W bit is set, immediately reads a byte from
	the slave into TWI0.MDATA. After reading this register, you'll need to send
	an ACK, after which the TWI device reads another byte. Send a NACK to end
	the transaction.

	Input:
		nbytes - number of bytes to read

	Output:
		result - array of 8-bit data values

------------------------------------------------------------------------------*/
void read_TWI(uint8_t *result, uint8_t nbytes)
{

	uint8_t icnt;	// # of bytes already read

	while (!(TWI0.MSTATUS & TWI_RIF_bm)) {		// Wait
		asm("nop");
	}

	*result++ = TWI0.MDATA;

	icnt = 1;
	while (icnt++ < nbytes) {
		TWI0.MCTRLB = TWI_MCMD_RECVTRANS_gc;	// ACK forces next read
		while (!(TWI0.MSTATUS & TWI_RIF_bm)) {
			asm("nop");
		}
		*result++ = TWI0.MDATA;
	}
	TWI0.MCTRLB = TWI_ACKACT_bm | TWI_MCMD_RECVTRANS_gc; // NACK

}

/*------------------------------------------------------------------------------
uint8_t start_TWI(uint8_t address, uint8_t rw)

	Puts a start condition on the bus and sends the slave address and R/W bit.
	The address has already been left-shifted to make space for the rw bit
	(bit 0), which will be added here.

	Input:	addr - slave address, left shifted one bit
			rw - either TWIWRITE or TWIREAD

	Return:	0 - All OK
			TWIARBLOST - bus arbitration lost
			TWIBUSERR - bus error

	Notes:

		1.	init_TWI() must be called first.
	
		2.	When reading, the start operation automatically reads the first
			byte of data from the slave.

------------------------------------------------------------------------------*/
uint8_t start_TWI(uint8_t addr, uint8_t rw)
{

	if (rw == TWIREAD) {
		addr |= 0x01;
		TWI0.MADDR = addr;
		while (!(TWI0.MSTATUS & TWI_RIF_bm)) {
			asm("nop");
		}
	} else if (rw == TWIWRITE) {
		addr &= ~0x01;
		TWI0.MADDR = addr;
		while(!(TWI0.MSTATUS & TWI_WIF_bm)) {
			asm("nop");
		}
	}

	if (TWI0.MSTATUS & TWI_ARBLOST_bm) {		// Errors?
		return (TWIARBLOST);					// Arbitration lost
	} else if (TWI0.MSTATUS & TWI_BUSERR_bm) {	// Bus error
		return (TWIBUSERR);
	}

	return(0);
}

/*------------------------------------------------------------------------------
void stop_TWI(void)

	Puts a stop condition on the TWI bus. The TWI_MCMD_STOP_gc bit in MCTRLB
	is a strobe action.

------------------------------------------------------------------------------*/
void stop_TWI(void)
{

	TWI0.MCTRLB = TWI_ACKACT_bm | TWI_MCMD_STOP_gc;

}

/*------------------------------------------------------------------------------
uint8_t write_TWI(uint8_t *data, uint8_t nbytes)

	Write nbytes onto the TWI bus starting with *data.

	Returns 0 on success, TWIACKERR if slave does not acknowledge.

------------------------------------------------------------------------------*/
uint8_t write_TWI(uint8_t *data, uint8_t nbytes)
{

	uint8_t i;

	for (i = 0; i < nbytes; i++) {
		TWI0.MDATA = *data++;
		while (!(TWI0.MSTATUS & TWI_WIF_bm)) {
			asm("nop");
		}
	}
	if (TWI0.MSTATUS & TWI_RXACK_bm) {		// ACK error
		stop_TWI();
		return(TWIACKERR);
	} else {
		return(0);
	}

}
