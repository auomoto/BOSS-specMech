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
uint8_t ROBOGoTo(uint8_t controller, int32_t newPosition)
	Move the motor at the indicated controller to a new absolute position.

	Inputs:
		controller - the Roboclaw controller address (128, 129, or 130)
		newPosition - the new encoder position in native encoder units

	Returns:
	
------------------------------------------------------------------------------*/
uint8_t ROBOGoTo(uint8_t controller, int32_t newPosition)
{

	uint8_t tbuf[21], buffer;
	uint16_t crc;
	uint32_t acceleration, deceleration, speed;

	acceleration = ACCELERATION;
	deceleration = DECELERATION;
	speed = SPEED;
	buffer = 0;							// 0 -> command is buffered

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
	send_USART(1, tbuf, 21);

	recv1_buf.data[0] = 0x00;
	recv1_buf.nbytes = 1;
	recv1_buf.nxfrd = 0;
	recv1_buf.done = NO;

	start_TCB0(1);				// Start 1 ms ticks timer
	for (;;) {
		if (recv1_buf.done == YES) {
			break;
		}
		if (ticks > 50) {		// 4 ms just barely works at 38400 baud
			stop_TCB0();
			printError(ERR_MTRTIMEOUT, "Motor GoTo timeout");
			return(ERROR);
			break;
		}
	}
	stop_TCB0();

//	if (recv1_buf.data[0] != 0xFF) {
//		printError(ERR_MTRTIMEOUT, "Motor timeout");
//		return(ERROR);
//	}

	return(NOERROR);
}

/*
uint8_t ROBOMoveAbsolute(uint8_t controller, int32_t position)
{

	uint8_t tbuf[9];
	uint16_t crc;

	tbuf[0] = controller;
	tbuf[1] = ROBOMOVETO;			// Command 119
	tbuf[2] = (position >> 24) & 0xFF;
	tbuf[3] = (position >> 16) & 0xFF;
	tbuf[4] = (position >> 8) & 0xFF;
	tbuf[5] = (position) & 0xFF;
	tbuf[6] = 1;				// Allow interrupting an ongoing motion
	crc = crc16(tbuf, 7);
	tbuf[7] = (crc >> 8) & 0xFF;
	tbuf[8] = crc & 0xFF;
	send_USART(1, tbuf, 9);

	recv1_buf.data[0] = 0x00;
	recv1_buf.nbytes = 1;
	recv1_buf.nxfrd = 0;
	recv1_buf.done = NO;

	stop_TCB0();
	start_TCB0(1);
	for (;;) {
		if (ticks > 10) {		// 4 ms just works at 38400 baud
			break;
		}
		if (recv1_buf.done == YES) {
			break;
		}
	}

	if (recv1_buf.data[0] != 0xFF) {
		return(ERRORPROMPT);
	}
	return(GREATERPROMPT);

}
*/


/*------------------------------------------------------------------------------
uint8_t ROBOMove(uint8_t cstack)
	Move a motor to a new relative or absolute postion. Called by the m
	command.

	Input:
		cstack - the command stack position

	Output:
		ERROR if an unknown motor designator (A, B, C, or a, b, c) is read.
------------------------------------------------------------------------------*/
uint8_t ROBOMove(uint8_t cstack)
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
			retval = get_ROBOEncoder(controller, ROBOREADENCODERCOUNT, &currentPosition);
			if (retval == ERROR) {
				return(ERROR);
			}
			break;

		default:
			printError(ERR_UNKNOWNMTR, "Invalid motor designator");
			return(ERROR);
			break;	
	}

	newPosition = currentPosition + (atol(pcmd[cstack].cvalue) * ROBOCOUNTSPERMICRON);

	return(ROBOGoTo(controller, newPosition));

}

/*------------------------------------------------------------------------------
uint8_t get_ROBOEncoder(uint8_t controller, uint8_t command, uint32_t *value)
	Read the 32-bit encoder value and speed

	Inputs:
		controller - the controller address
		command - either ROBOREADENCODERCOUNT (command 16) or
			ROBOREADENCODERSPEED (command 18)
	Returns
		value - a 32-bit integer, the encoder value or speed.
		status - from page 86 of version 5.7 of the manual:
			Bit0 - Counter underflow (1=underflow occurred, clear after reading)
			Bit1 - Direction (0=forward, 1-backwards)
			Bit2 - Counter overflow (1=overflow occurred, clear after reading)
			Bits 3-7 are "reserved" and masked out here since Bit7 returns 1
------------------------------------------------------------------------------*/
uint8_t get_ROBOEncoder(uint8_t controller, uint8_t command, int32_t *value)
{
	uint8_t i, tbuf[7];
	uint16_t crcReceived, crcExpected;

	recv1_buf.nbytes = 7;
	recv1_buf.nxfrd = 0;
	recv1_buf.done = NO;

	tbuf[0] = controller;
	tbuf[1] = command;
	send_USART(1, tbuf, 2);
	start_TCB0(1);
	while (recv1_buf.done == NO) {
		asm("nop");
		if (ticks > 4) {
			stop_TCB0();
			printError(ERR_MTRENCREAD, "Encoder read timeout");
			return(ERROR);
		}
	}
	stop_TCB0();

	crcReceived = (recv1_buf.data[5] << 8) | recv1_buf.data[6];

	for (i = 2; i < 7; i++) {
		tbuf[i] = recv1_buf.data[i-2];
	}
	crcExpected = crc16(tbuf, 7);

	if (crcReceived != crcExpected) {
		*value = 0;
		printError(ERR_MTRENCCRC, "Encoder CRC error");
		return(ERROR);
	}

	*value =  (uint32_t) recv1_buf.data[0] << 24;
	*value |= (uint32_t) recv1_buf.data[1] << 16;
	*value |= (uint32_t) recv1_buf.data[2] << 8;
	*value |= (uint32_t) recv1_buf.data[3];

	return(NOERROR);

}

/*------------------------------------------------------------------------------
uint8_t get_ROBOEncoderValue(uint8_t controller, uint32_t *encoderValue)
	Read the 32-bit encoder value
------------------------------------------------------------------------------*/
/*
uint8_t get_ROBOEncoderValue(uint8_t controller, uint32_t *encoderValue)
{
	uint8_t i, cmd, status, tbuf[7];
	uint16_t crcReceived, crcExpected;
	uint32_t value;

	recv1_buf.nbytes = 7;
	recv1_buf.nxfrd = 0;
	recv1_buf.done = NO;

	cmd = ROBOREADENCODERCOUNT;
	tbuf[0] = controller;
	tbuf[1] = cmd;
	send_USART(1, tbuf, 2);
	while (recv1_buf.done == NO) {
		asm("nop");
	}

	crcReceived = (recv1_buf.data[5] << 8) | recv1_buf.data[6];

	for (i = 2; i < 7; i++) {
		tbuf[i] = recv1_buf.data[i-2];
	}
	crcExpected = crc16(tbuf, 7);

	if (crcReceived != crcExpected) {
		*encoderValue = 0;
		return(0xF0);
	}

	value =  (uint32_t) recv1_buf.data[0] << 24;
	value |= (uint32_t) recv1_buf.data[1] << 16;
	value |= (uint32_t) recv1_buf.data[2] << 8;
	value |= (uint32_t) recv1_buf.data[3];
	*encoderValue = value;
	status = recv1_buf.data[4] & 0x07;		// Blank reserved bits
	return(status);

}
*/

float get_ROBOFloat(uint8_t controller, uint8_t command)
{
	uint8_t tbuf[4];
	uint16_t value, crcReceived, crcExpected;

	recv1_buf.nbytes = 4;
	recv1_buf.nxfrd = 0;
	recv1_buf.done = NO;

	tbuf[0] = controller;
	tbuf[1] = command;
	send_USART(1, tbuf, 2);

	for (;;) {
		if (recv1_buf.done == YES) {
			break;
		}
	}
	value = (recv1_buf.data[0] << 8) | recv1_buf.data[1];
	crcReceived = (recv1_buf.data[2] << 8) | recv1_buf.data[3];
	recv1_buf.data[2] = recv1_buf.data[0];
	recv1_buf.data[3] = recv1_buf.data[1];
	recv1_buf.data[0] = controller;
	recv1_buf.data[1] = command;
	crcExpected = crc16(recv1_buf.data, 4);
	if (crcExpected != crcReceived) {
		return(-666.0);
	} else {
		return((float) value / 10.0);
	}
}

uint32_t get_ROBOInt32(uint8_t controller, uint8_t command)
{
	uint8_t i, tbuf[6];
	uint16_t crcReceived, crcExpected;
	uint32_t value;

	recv1_buf.nbytes = 6;
	recv1_buf.nxfrd = 0;
	recv1_buf.done = NO;

	tbuf[0] = controller;
	tbuf[1] = command;
	send_USART(1, tbuf, 2);
	while (recv1_buf.done == NO) {
		asm("nop");
	}

	crcReceived = (recv1_buf.data[4] << 8) | recv1_buf.data[5];

	for (i = 2; i < 6; i++) {
		tbuf[i] = recv1_buf.data[i-2];
	}
	crcExpected = crc16(tbuf, 6);

	if (crcReceived != crcExpected) {
		value = 0;
		return(0xF0);
	}

	value =  (uint32_t) recv1_buf.data[0] << 24;
	value |= (uint32_t) recv1_buf.data[1] << 16;
	value |= (uint32_t) recv1_buf.data[2] << 8;
	value |= (uint32_t) recv1_buf.data[3];
	return(value);

}

/*
uint8_t ROBOMove(uint8_t cstack, uint8_t speed)
{
	uint8_t controller, command, tbuf[5];
	uint16_t crc;

	recv1_buf.nbytes = 1;
	recv1_buf.nxfrd = 0;
	recv1_buf.done = NO;

	command = 1;
	if ((pcmd[cstack].cobject) == 'a') {
		controller = MOTORAADDR;
	}
	tbuf[0] = controller;
	tbuf[1] = command;
	tbuf[2] = speed;
	crc = crc16(tbuf,3);
	tbuf[3] = (crc >> 8) & 0xFF;
	tbuf[4] = crc & 0xFF;
	send_USART(1, tbuf, 5);
	for (;;) {
		if (recv1_buf.done == YES) {
			break;
		}
	}

	if (recv1_buf.data[0] != 0xFF) {
		return(ERRORPROMPT);
	}
	return(GREATERPROMPT);
}
*/

/*
float get_ROBOTemperature(uint8_t controller)
{

	uint8_t cmd, tbuf[4];
	uint16_t value, crcReceived, crcExpected;

	cmd = ROBOREADTEMPERATURE;
	recv1_buf.nbytes = 4;
	recv1_buf.nxfrd = 0;
	recv1_buf.done = NO;

	tbuf[0] = controller;
	tbuf[1] = cmd;
	send_USART(1, tbuf, 2);

	for (;;) {
		if (recv1_buf.done == YES) {
			break;
		}
	}
	value = (recv1_buf.data[0] << 8) | recv1_buf.data[1];
	crcReceived = (recv1_buf.data[2] << 8) | recv1_buf.data[3];
	recv1_buf.data[2] = recv1_buf.data[0];
	recv1_buf.data[3] = recv1_buf.data[1];
	recv1_buf.data[0] = controller;
	recv1_buf.data[1] = cmd;
	crcExpected = crc16(recv1_buf.data, 4);
	if (crcExpected != crcReceived) {
		return(-666.0);
		} else {
		return((float) value / 10.0);
	}
}
*/
/*------------------------------------------------------------------------------
float get_ROBOVoltage(uint8_t controller)
	Returns the supply voltage

	Inputs:
		controller - The address of the RoboClaw controller

	Output:
		The voltage as measured by the RoboClaw
------------------------------------------------------------------------------*/
/*float get_ROBOVoltage(uint8_t controller)
{
	uint8_t cmd, tbuf[4];
	uint16_t value, crcReceived, crcExpected;

	cmd = ROBOREADMAINVOLTAGE;
	recv1_buf.nbytes = 4;
	recv1_buf.nxfrd = 0;
	recv1_buf.done = NO;

	tbuf[0] = controller;
	tbuf[1] = cmd;
	send_USART(1, tbuf, 2);

	for (;;) {
		if (recv1_buf.done == YES) {
			break;
		}
	}
	value = (recv1_buf.data[0] << 8) | recv1_buf.data[1];
	crcReceived = (recv1_buf.data[2] << 8) | recv1_buf.data[3];
	recv1_buf.data[2] = recv1_buf.data[0];
	recv1_buf.data[3] = recv1_buf.data[1];
	recv1_buf.data[0] = controller;
	recv1_buf.data[1] = cmd;
	crcExpected = crc16(recv1_buf.data, 4);
	if (crcExpected != crcReceived) {
		return(-666.0);
	} else {
		return((float) value / 10.0);
	}
}
*/
/*------------------------------------------------------------------------------
	crc16(packet, nbytes)

	RoboClaw expects a CRC16 word at the end of command data packets. See
	page 59 of the RoboClaw user manual Rev 5.7 (2015) for this routine.

	Inputs:
		packet - array with length nBytes
		nBytes - length of the character array

	Returns:
		The CRC16 value as an unsigned 16-bit word
------------------------------------------------------------------------------*/

uint16_t crc16(unsigned char *packet, uint16_t nBytes)
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