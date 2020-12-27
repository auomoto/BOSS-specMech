/*------------------------------------------------------------------------------
twi.c
	TWI/I2C routines
------------------------------------------------------------------------------*/

#ifndef TWIC
#define TWIC

#define TWIFREQ		100000UL		// Fastest allowed for F_CPU = 3.33 MHz
#define TWIBAUD		((uint8_t) (F_CPU/(2*TWIFREQ)) - 5)	// Ignore rise time
//#define TWISENDACK	(TWI0.MCTRLB = TWI_MCMD_RECVTRANS_gc)
//#define TWISENDNACK	(TWI0.MCTRLB = (TWI_ACKACT_bm | TWI_MCMD_RECVTRANS_gc))
//#define TWINACKNEXT	(TWI0.MCTRLB |= TWI_ACKACT_NACK_gc)
//#define TWIACKNEXT	(TWI0.MCTRLB |= TWI_ACKACT_ACK_gc)
#define TWIWRITE	0
#define TWIREAD		1
#define TWIBUSERR	1
#define TWIARBLOST	2
#define TWIACKERR	3
#define TWINOSLAVE	4

// Function prototypes
void init_TWI(void);
uint8_t read_TWI(void);
uint8_t readlast_TWI(void);
uint8_t start_TWI(uint8_t, uint8_t);
void stop_TWI(void);
uint8_t write_TWI(uint8_t);

/*------------------------------------------------------------------------------
void init_TWI(void)

	Initialize the TWI interface on a megaAVR 0-series processor.

	On the ATMega4809:
		SDA is on PA2
		SCL is on PA3

	In the data sheet, ATmega4808-4809-Data-Sheet-DS40002173A.pdf, the TWI
	interface information is in section 25, starting on page 326.

	Initialization instructions are in section 25.3.1 (p327):

	1.	Set SDASETUP and SDAHOLD in TWI.CTRLA. These are important only for
		SMB operation so will use the startup defaults (do nothing).

	2a.	Write Master Baud Rate Register TWIn.BAUD *before* enabling TWI master.
		The data sheet formula for F_SCL, the TWI frequency is:
		F_SCL = F_CPU/(10 + 2*BAUD + F_CPU*T_RISE) or
		BAUD = (F_CPU/2*F_SCL) - 5 - ((F_CPU*T_RISE)/2)
		These are defined as macros at the top of this file and changing TWIFREQ
		(defined in Hz) controls the baud rate.

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
		Experiments show that an ACK is sent automatically anyway without setting
		this bit, so we won't.

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
uint8_t read_TWI(void)

	Read one byte then send an ACK. Use readlast_TWI() to read the last byte
	and send a NACK.

old note:
	NB: This does not work if nbytes is 1. The ACKACT bit in TWI0.MCTRLB must
	be set to 1 BEFORE doing the start_TWI read operation. The start_TWI will
	immediately read the first data byte and send an ACK, which seems to cause
	(at least the MCP23008) to hold the clock line low, freezing the software
	(issuing a STOP condition doesn't get out of this; only a reset of the
	MCP23008 does).
uint8_t read_TWI(void)
{

	uint8_t data;
	int x;

	TWI0.MCTRLB |= TWI_ACKACT_ACK_gc;			// Set ACK after read

on_LED();
_delay_ms(1000);
x = 0;
	while (!(TWI0.MSTATUS & TWI_RIF_bm)) {		// Wait for data
		asm("nop");
		if (x++ > 1000) break;
	}
off_LED();
_delay_ms(1000);
on_LED();
_delay_ms(50);
	data = TWI0.MDATA;
off_LED();
_delay_ms(500);
//	TWI0.MCTRLB = (TWI_ACKACT_ACK_gc | TWI_MCMD_RECVTRANS_gc);			// Send ACK after read
	return(data);

}
------------------------------------------------------------------------------*/
uint8_t read_TWI(void)
{

	uint8_t data;

//	TWI0.MCTRLB |= TWI_ACKACT_ACK_gc;			// Set ACK after read
//	TWI0.MCTRLB |= TWI_ACKACT_bm;
	while (!(TWI0.MSTATUS & TWI_RIF_bm)) {		// Wait for data
		asm("nop");
	}
TWI0.MCTRLB &= ~(1<<TWI_ACKACT_bp);
	data = TWI0.MDATA;
//	while (!(TWI0.MSTATUS & TWI_RIF_bm)) {		// Wait for data
//		asm("nop");
//	}
//	TWI0.MCTRLB |= (TWI_ACKACT_bm | TWI_MCMD_RECVTRANS_gc);			// Send ACK after read
	TWI0.MCTRLB |= TWI_MCMD_RECVTRANS_gc;			// Send ACK after read

	return(data);
}


/*------------------------------------------------------------------------------
uint8_t readlast_TWI(void)

	Read the last byte in a sequence, or the first byte in a single-byte read.
	This routine sends a NACK.
------------------------------------------------------------------------------*/
uint8_t readlast_TWI(void)
{

	uint8_t data;

//	TWI0.MCTRLB |= TWI_ACKACT_NACK_gc;			// Set NACK after read
	while (!(TWI0.MSTATUS & TWI_RIF_bm)) {		// Wait for data
		asm("nop");
	}
	TWI0.MCTRLB |= TWI_ACKACT_NACK_gc;
	data = TWI0.MDATA;
//	TWI0.MCTRLB = (TWI_ACKACT_NACK_gc | TWI_MCMD_RECVTRANS_gc);			// Send NACK after read THIS IS BAD HERE

	return(data);

}

/*------------------------------------------------------------------------------
uint8_t start_TWI(uint8_t address, uint8_t rw)

	Puts a start condition on the bus and sends the slave address and R/W bit.
	The address has already been left-shifted to make space for the rw bit
	(bit 0), which will be added or cleared here depending on the value of rw.

	Input:	addr - slave address, left shifted one bit
			rw - either TWIWRITE (0) or TWIREAD (1)

	Return:	0 - All OK
			TWIARBLOST - bus arbitration lost
			TWIBUSERR - bus error
			TWINOSLAVE - no device sent an ACK

Notes:

	1.	init_TWI() must be called first.

	2.	The WIF or RIF in TWI0.MSTATUS is set after the address packet is sent
		(the RIF only when a read operation is requested).

	3.	Check for BUSERR, ARBLOST, and RXACK flags being set in TWI0.MSTATUS.
		These are errors. The MSTATUS register is cleared by accessing any
		of TWI0.MADDR, TWI0.MDATA, or the CMD bits in TWI-.MCTRLB.
------------------------------------------------------------------------------*/
uint8_t start_TWI(uint8_t addr, uint8_t rw)
{

	if (rw == TWIREAD) {
		addr |= 0x01;
	} else {
		addr &= ~0x01;
	}

	TWI0.MADDR = addr;							// Start condition

	while (!(TWI0.MSTATUS & (TWI_WIF_bm | TWI_RIF_bm))) {
		asm("nop");								// Wait for addr transmission
	}

	if ((TWI0.MSTATUS & TWI_BUSERR_bm)) {		// Bus error
		return(TWIBUSERR);
	} else if (TWI0.MSTATUS & TWI_ARBLOST_bm) {	// Arbitration lost
		return(TWIARBLOST);
	} else if (TWI0.MSTATUS & TWI_RXACK_bm) {	// No device responded
		return(TWINOSLAVE);
	} else {									// OK
		return(0);
	}
}

/*------------------------------------------------------------------------------
void stop_TWI(void)

	Puts a stop condition on the TWI bus. The TWI_MCMD_STOP_gc bit in MCTRLB
	is a strobe action.
------------------------------------------------------------------------------*/
void stop_TWI(void)
{

//	TWI0.MCTRLB |= TWI_MCMD_STOP_gc;
	TWI0.MCTRLB = (TWI_ACKACT_bm | TWI_MCMD_STOP_gc);	// NACK and STOP

}

/*------------------------------------------------------------------------------
uint8_t write_TWI(uint8_t data)

	Write data onto the TWI bus.\
	The test for RXACK being set is probably not interesting since an ACK
	is required to get out of the NOP loop at the top.

	Returns 0 on success.
------------------------------------------------------------------------------*/
uint8_t write_TWI(uint8_t data)
{

	while (!(TWI0.MSTATUS & TWI_WIF_bm)) {	// Wait for previous writes
		asm("nop");
	}

	TWI0.MDATA = data;

	while (!(TWI0.MSTATUS & TWI_WIF_bm)) {
		asm("nop");
	}

	if (TWI0.MSTATUS & TWI_RXACK_bm) {		// If device did not ACK
		return(TWIACKERR);
	} else {
		return(0);
	}

}

#endif
