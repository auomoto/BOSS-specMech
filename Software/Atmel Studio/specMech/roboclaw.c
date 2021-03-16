/*------------------------------------------------------------------------------
roboclaw.c
	Collimator motor controls
------------------------------------------------------------------------------*/

#include "globals.h"
#include "usart.h"
#include "timers.h"
#include "commands.h"
#include "errors.h"
#include "roboclaw.h"

/*------------------------------------------------------------------------------
uint16_t crc16(uint8_t *packet, uint16_t nbytes)

	RoboClaw expects a CRC16 word at the end of command data packets. See
	page 59 of the RoboClaw user manual Rev 5.7 (2015) for this routine.

	Inputs:
		packet - array to compute
		nBytes - length of the array

	Returns:
		The CRC16 value as an unsigned 16-bit word
------------------------------------------------------------------------------*/
uint16_t crc16(uint8_t *packet, uint16_t nBytes)
{

	uint8_t bit;
	uint16_t crc = 0;
	int16_t byte;

	for (byte = 0; byte < nBytes; byte++) {
		crc = crc ^ ((uint16_t) packet[byte] << 8);
		for (bit = 0; bit < 8; bit++) {
			if (crc & 0x8000) {
				crc = (crc << 1) ^ 0x1021;
			} else {
				crc = crc << 1;
			}
		}
	}
	return (crc);
}

/*------------------------------------------------------------------------------
uint8_t get_MOTOREncoder(uint8_t controller, uint8_t command, uint32_t *value)
	Gets the 32-bit encoder value and speed

	Inputs:
		controller: the controller address (128, 129, or 130)
		command:	ROBOREADENCODERCOUNT (command 16) or
					ROBOREADENCODERSPEED (command 18)

	Output:
		value: the encoder value or speed. It's declared as an unsigned
			integer but sprintf interprets it correctly if it's negative.

	Returns
		ERROR on USART timeout or 0xFF reply error
		NOERROR otherwise

	Also available but not output here:
		status - from page 86 of version 5.7 of the manual:
			Bit0: Counter underflow (1=underflow occurred, clear after reading)
			Bit1: Direction (0=forward, 1-backwards)
			Bit2: Counter overflow (1=overflow occurred, clear after reading)
			Bits 3-7 are "reserved." Bit7 is 1.
------------------------------------------------------------------------------*/
uint8_t get_MOTOREncoder(uint8_t controller, uint8_t command, int32_t *value)
{
	uint8_t i, tbuf[7];
	uint16_t crcReceived, crcExpected;

	recv1_buf.nbytes = 7;			// Set up reply buffer
	recv1_buf.nxfrd = 0;
	recv1_buf.done = NO;

	tbuf[0] = controller;
	tbuf[1] = command;
	send_USART(1, tbuf, 2);			// Send the command

	start_TCB0(1);
	while (recv1_buf.done == NO) {	// Wait for the reply
		asm("nop");
		if (ticks > 50) {			// Timeout
			stop_TCB0();
			printError(ERR_MTRREADENC, "get_MOTOREncoder timeout");
			return(ERROR);
		}
	}
	stop_TCB0();

	crcReceived = (recv1_buf.data[5] << 8) | recv1_buf.data[6];

	for (i = 2; i < 7; i++) {		// Compute expected crc value
		tbuf[i] = recv1_buf.data[i-2];
	}
	crcExpected = crc16(tbuf, 7);

	if (crcReceived != crcExpected) {
		*value = 0;
		printError(ERR_MTRENCCRC, "get_MOTOREncoder CRC");
		return(ERROR);
	}

	*value =  (uint32_t) recv1_buf.data[0] << 24;
	*value |= (uint32_t) recv1_buf.data[1] << 16;
	*value |= (uint32_t) recv1_buf.data[2] << 8;
	*value |= (uint32_t) recv1_buf.data[3];

//	status = recv1_buf.data[4];

	return(NOERROR);

}

/*------------------------------------------------------------------------------
uint8_t get_MOTORFloat(uint8_t controller, uint8_t command, float *value)
	Retrieves a floating point value (voltage or temperature) from a RoboClaw
	controller.

	Inputs:
		controller:	Controller address (128, 129, 130)
		command:	ROBOREADMAINVOLTAGE (command 24) or
					ROBOREADTEMPERATURE (command 82)

	Outputs:
		value: The integer value from the command in increments of tenths of
			a volt or tenths of a degree. The value returned is divided by 10
			to present units of degrees C and volts.

	Returns:
		ERROR: USART timeout or CRC check error
		NOERROR
------------------------------------------------------------------------------*/
uint8_t get_MOTORFloat(uint8_t controller, uint8_t command, float *value)
{
	uint8_t tbuf[4];
	uint16_t tempval, crcReceived, crcExpected;

	recv1_buf.nbytes = 4;				// Set up receive buffer
	recv1_buf.nxfrd = 0;
	recv1_buf.done = NO;

	tbuf[0] = controller;
	tbuf[1] = command;
	send_USART(1, tbuf, 2);				// Send command

	start_TCB0(1);
	for (;;) {
		if (recv1_buf.done == YES) {	// Receive reply
			break;
		}
		if (ticks > 50) {				// Timeout
			stop_TCB0();
			printError(ERR_MTRREADENC, "ROBOFloat timeout");
			return(ERROR);
		}
	}
	stop_TCB0();

	tempval = (recv1_buf.data[0] << 8) | recv1_buf.data[1];
	crcReceived = (recv1_buf.data[2] << 8) | recv1_buf.data[3];

	tbuf[0] = controller;
	tbuf[1] = command;
	tbuf[2] = recv1_buf.data[0];
	tbuf[3] = recv1_buf.data[1];
	crcExpected = crc16(tbuf, 4);

	if (crcExpected != crcReceived) {
		printError(ERR_MTRENCCRC, "get_MOTORFloat CRC");
		return(ERROR);
	} else {
		*value = ((float) tempval / 10.0);
		return(NOERROR);
	}
}

/*------------------------------------------------------------------------------
uint8_t get_MOTORInt32(uint8_t controller, uint8_t command, uint32_t *value)
	Returns a 32-bit integer from a RoboClaw controller. This could
	be the temperature (ROBOREADTEMPERATURE 82), main battery voltage
	(ROBOREADMAINVOLTAGE 24), or motor current (ROBOREADCURRENT 49).

	Inputs:
		controller: controller address (128, 129, or 130)
		command: RoboClaw command number

	Outputs:
		value: the 32-bit value

	Returns:
		ERROR: USART timeout or CRC error
		NOERROR
------------------------------------------------------------------------------*/
uint8_t get_MOTORInt32(uint8_t controller, uint8_t command, uint32_t *value)
{
	uint8_t i, tbuf[6];
	uint16_t crcReceived, crcExpected;
	uint32_t tempval;

	recv1_buf.nbytes = 6;				// Set up receive buffer
	recv1_buf.nxfrd = 0;
	recv1_buf.done = NO;

	tbuf[0] = controller;
	tbuf[1] = command;
	send_USART(1, tbuf, 2);				// Send command

	start_TCB0(1);
	for (;;) {
		if (recv1_buf.done == YES) {	// Receive reply
			stop_TCB0();
			break;
		}
		if (ticks > 50) {				// Timeout
			stop_TCB0();
			printError(ERR_MTRTIMEOUT, "get_MOTORInt32 timeout");
			return(ERROR);
		}
	}

	crcReceived = (recv1_buf.data[4] << 8) | recv1_buf.data[5];

	for (i = 2; i < 6; i++) {			// Compute expected CRC
		tbuf[i] = recv1_buf.data[i-2];
	}
	crcExpected = crc16(tbuf, 6);

	if (crcReceived != crcExpected) {
		printError(ERR_MTRENCCRC, "get_MOTORInt32 CRC error");
		*value = 0x7FFFFFFF;
		return(ERROR);
	}

	tempval =  (uint32_t) recv1_buf.data[0] << 24;
	tempval |= (uint32_t) recv1_buf.data[1] << 16;
	tempval |= (uint32_t) recv1_buf.data[2] << 8;
	tempval |= (uint32_t) recv1_buf.data[3];
	*value = tempval;

	return(NOERROR);

}

uint8_t get_MOTORSpeed(uint8_t controller, uint32_t *speed)
{
	if (get_MOTORInt32(controller, ROBOREADENCODERSPEED, speed) == ERROR) {
		return(ERROR);
	}
	return(NOERROR);
}


/*------------------------------------------------------------------------------
uint8_t init_MOTORS(void)
	Reads the last-saved encoder value from FRAM and loads it into the three
	controllers. No error message is output on first call (reboot).

	Inputs: None
	Outputs: None
	Returns
		ERROR if set_MOTOREncoder fails
		NOERROR otherwise
------------------------------------------------------------------------------*/
uint8_t init_MOTORS(void)
{

	static uint8_t firstTime[3] = {YES, YES, YES};
	uint8_t controller, error = 0;
	uint32_t encoderValue;

	for (controller = 128; controller < 131; controller++) {
		// get saved encoder value from FRAM
		encoderValue = 22 * ROBOCOUNTSPERMICRON;	// Proxy for now
		if (set_MOTOREncoder(controller, encoderValue) == ERROR) {
			if (!firstTime[controller - 128]) {
				printError(ERR_MTRSETENC, "init_MOTORS");
				firstTime[controller - 128] = NO;
			}
			error++;
		}
	}
	if (error) {
		return(ERROR);
	} else {
		return(NOERROR);
	}
}

/*------------------------------------------------------------------------------
uint8_t move_MOTOR(uint8_t cstack)
	Move to a new relative or absolute position. Called by the m command.

	Input:
		cstack: command stack position

	Returns:
		ERROR if an unknown motor designator (not A, B, C, or a, b, c) is read
		NOERROR otherwise
------------------------------------------------------------------------------*/
uint8_t move_MOTOR(uint8_t cstack)
{

	uint8_t motor, controller, retval;
	int32_t newPosition, currentPosition;

	motor = pcmd[cstack].cobject;
	switch(motor) {
		case 'A':
		case 'B':
		case 'C':
			controller = motor + 63;
			currentPosition = 0;
			break;

		case 'a':
		case 'b':
		case 'c':
			controller = motor + 31;
			retval = get_MOTOREncoder(controller, ROBOREADENCODERCOUNT, &currentPosition);
			if (retval == ERROR) {
				return(ERROR);
			}
			break;

		default:
			printError(ERR_UNKNOWNMTR, "move_MOTOR unknown motor");
			return(ERROR);
			break;	
	}

	if (pcmd[cstack].cvalue[0] == '\0') {	// Don't do anything on null distance
		printError(ERR_MTRNULLMOVE, "move_MOTOR no position or increment");
		return(ERROR);
	}

	newPosition = currentPosition + (atol(pcmd[cstack].cvalue) * ROBOCOUNTSPERMICRON);
	return(move_MOTORAbsolute(controller, newPosition));

}

/*------------------------------------------------------------------------------
uint8_t move_MOTORAbsolute(uint8_t controller, int32_t newPosition)
	Move the motor on the selected controller to a new absolute position.

	Inputs:
		controller: controller address (128, 129, or 130)
		newPosition: the new encoder position in native encoder units

	Returns:
		ERROR on USART timeout or bad (not 0xFF) ack
		NOERROR otherwise
------------------------------------------------------------------------------*/
uint8_t move_MOTORAbsolute(uint8_t controller, int32_t newPosition)
{

	uint8_t tbuf[21], buffer;
	uint16_t crc;
	uint32_t acceleration, deceleration, speed;

	acceleration = ACCELERATION;
	deceleration = DECELERATION;
	speed = SPEED;
	buffer = 0;							// 0 -> command is buffered

	recv1_buf.data[0] = 0x00;			// Set up receiving buffer
	recv1_buf.nbytes = 1;
	recv1_buf.nxfrd = 0;
	recv1_buf.done = NO;

	tbuf[0] = controller;
	tbuf[1] = ROBODRIVETO;				// Command 65
	tbuf[2] = (acceleration >> 24) & 0XFF;
	tbuf[3] = (acceleration >> 16) & 0xFF;
	tbuf[4] = (acceleration >> 8) & 0xFF;
	tbuf[5] = (acceleration) & 0xFF;
	tbuf[6] = (speed >> 24) & 0xFF;
	tbuf[7] = (speed >> 16) & 0xFF;
	tbuf[8] = (speed >> 8) & 0xFF;
	tbuf[9] = (speed) & 0xFF;
	tbuf[10] = (deceleration >> 24) & 0xFF;
	tbuf[11] = (deceleration >> 16) & 0xFF;
	tbuf[12] = (deceleration >> 8) & 0xFF;
	tbuf[13] = (deceleration) & 0xFF;
	tbuf[14] = (newPosition >> 24) & 0xFF;
	tbuf[15] = (newPosition >> 16) & 0xFF;
	tbuf[16] = (newPosition >> 8) & 0xFF;
	tbuf[17] = (newPosition) & 0xFF;
	tbuf[18] = buffer;
	crc = crc16(tbuf, 19);
	tbuf[19] = (crc >> 8) & 0xFF;
	tbuf[20] = crc & 0xFF;

	send_USART(1, tbuf, 21);			// Send command

	start_TCB0(1);						// Start 1 ms ticks timer
	for (;;) {
		if (recv1_buf.done == YES) {	// Reply received
			stop_TCB0();
			break;
		}
		if (ticks > 50) {				// 4 ms just barely works at 38400 baud
			stop_TCB0();
			printError(ERR_MTRTIMEOUT, "move_MOTORAbsolute timeout");
			return(ERROR);
		}
	}

	if (recv1_buf.data[0] != 0xFF) {
		printError(ERR_MTRTIMEOUT, "move_MOTORAbsolute ack");		
		return(ERROR);
	}

	return(NOERROR);
}

/*------------------------------------------------------------------------------
uint8_t set_MOTOREncoder(uint8_t controller, uint32_t value)
	Loads an encoder value into a controller

	Inputs:
		controller: the motor controller (128, 129, or 130)
		value: the encoder value to load

	Outputs:
		None

	Returns:
		ERROR on USART timeout or bad (not 0xFF) ack
		NOERROR otherwise

	Note:
		Does not print an error. It's only called at reboot. We should have a
		double-check at startup to make sure the encoder values agree before
		a move.
------------------------------------------------------------------------------*/
uint8_t set_MOTOREncoder(uint8_t controller, uint32_t value)
{

	uint8_t tbuf[6];

	recv1_buf.data[0] = 0x00;			// Set up receiving buffer
	recv1_buf.nbytes = 1;
	recv1_buf.nxfrd = 0;
	recv1_buf.done = NO;

	tbuf[0] = controller;
	tbuf[1] = ROBOSETENCODER;
	tbuf[2] = (value >> 24) & 0xFF;
	tbuf[3] = (value >> 16) & 0xFF;
	tbuf[4] = (value >> 8) & 0xFF;
	tbuf[5] = value & 0xFF;

	send_USART(1, tbuf, 6);				// Send the command

	start_TCB0(1);						// Start 1 ms ticks timer
	for (;;) {
		if (recv1_buf.done == YES) {	// Reply received
			stop_TCB0();
			break;
		}
		if (ticks > 50) {				// 4 ms barely works at 38400 baud
			stop_TCB0();
			return(ERROR);
		}
	}

	if (recv1_buf.data[0] != 0xFF) {	// Bad ack
		return(ERROR);
	}

	return(NOERROR);

}
