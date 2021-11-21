#include "globals.h"
#include "timers.h"
#include "errors.h"
#include "twi.h"

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

	2.	Write Master Baud Rate Register TWIn.BAUD *before* enabling TWI master.
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
	Read one byte then send an ACK.
	Use readlast_TWI() to read the last byte and send a NACK.
------------------------------------------------------------------------------*/
uint8_t read_TWI(void)
{

	uint8_t data;

	while (!(TWI0.MSTATUS & TWI_RIF_bm)) {		// Wait xfer to complete
		asm("nop");								// Should set timer here
	}

	TWI0.MCTRLB &= ~(1<<TWI_ACKACT_bp);			// Send ACK, next read
	data = TWI0.MDATA;
	TWI0.MCTRLB |= TWI_MCMD_RECVTRANS_gc;		// Send ACK after read

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

	while (!(TWI0.MSTATUS & TWI_RIF_bm)) {		// Wait for xfer to complete
		asm("nop");
	}

	TWI0.MCTRLB |= TWI_ACKACT_NACK_gc;
	data = TWI0.MDATA;

	return(data);

}

/*------------------------------------------------------------------------------
uint8_t start_TWI(uint8_t address, uint8_t rw)
	Puts a start condition on the bus and sends the device address and R/W bit.
	The address has already been left-shifted to make space for the rw bit
	(bit 0), which will be added or cleared here depending on the value of rw.

	Input:	addr - device address, left shifted one bit
			rw - either TWIWRITE (0) or TWIREAD (1)

	Return:	0 - All OK
			TWIARBLOST - bus arbitration lost
			TWIBUSERR - bus error
			TWINODEVICE - no TWI device sent an ACK

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
//		addr = ((addr << 1) | 0x01);
		TWI0.MADDR = ((addr << 1) | 0x01);
	} else {
//		addr = ((addr << 1) & ~0x01);
//		addr = (addr << 1);
		TWI0.MADDR = (addr << 1);
	}

//	TWI0.MADDR = addr;							// Start condition

	start_TCB0(1);
	while (!(TWI0.MSTATUS & (TWI_WIF_bm | TWI_RIF_bm))) {
		if (ticks > 10) {
			stop_TCB0();
			return(ERROR);
		}
		asm("nop");								// Wait for addr transmission
	}
	stop_TCB0();
	if ((TWI0.MSTATUS & TWI_BUSERR_bm)) {		// Bus error
		printError(ERR_TWI, "TWI bus");
		return(ERROR);
	} else if (TWI0.MSTATUS & TWI_ARBLOST_bm) {	// Arbitration lost
		printError(ERR_TWI, "TWI arbitration");
		return(ERROR);
	} else if (TWI0.MSTATUS & TWI_RXACK_bm) {	// No device responded
		return(ERROR);
	}

	return(NOERROR);

}

/*------------------------------------------------------------------------------
void stop_TWI(void)
	Puts a stop condition on the TWI bus. The TWI_MCMD_STOP_gc bit in MCTRLB
	is a strobe action.
------------------------------------------------------------------------------*/
void stop_TWI(void)
{

	TWI0.MCTRLB = (TWI_ACKACT_bm | TWI_MCMD_STOP_gc);	// NACK and STOP

}

/*------------------------------------------------------------------------------
uint8_t write_TWI(uint8_t data)
	Write data onto the TWI bus.
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

	start_TCB0(1);			// Maybe only check on start_TWI?
	while (!(TWI0.MSTATUS & TWI_WIF_bm)) {
		asm("nop");
		if (ticks > 50) {
			stop_TCB0();
			return(ERROR);
			break;
		}
	}
	stop_TCB0();
	if (TWI0.MSTATUS & TWI_RXACK_bm) {		// If device did not ACK
		return(ERROR);
	} else {
		return(NOERROR);
	}

}
