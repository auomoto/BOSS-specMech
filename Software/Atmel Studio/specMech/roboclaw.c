/*------------------------------------------------------------------------------
roboclaw.c
	Collimator motor controls
------------------------------------------------------------------------------*/

#include "globals.h"
#include "roboclaw.h"

/*------------------------------------------------------------------------------
uint8_t get_ROBOEncoder(uint8_t controller, uint8_t command, uint32_t *value)
	Read the 32-bit encoder value
------------------------------------------------------------------------------*/
uint8_t get_ROBOEncoder(uint8_t controller, uint8_t command, uint32_t *value)
{
	uint8_t i, status, tbuf[7];
	uint16_t crcReceived, crcExpected;

	recv1_buf.nbytes = 7;
	recv1_buf.nxfrd = 0;
	recv1_buf.done = NO;

	tbuf[0] = controller;
	tbuf[1] = command;
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
		*value = 0;
		return(0xF0);
	}

	*value =  (uint32_t) recv1_buf.data[0] << 24;
	*value |= (uint32_t) recv1_buf.data[1] << 16;
	*value |= (uint32_t) recv1_buf.data[2] << 8;
	*value |= (uint32_t) recv1_buf.data[3];
	status = recv1_buf.data[4] & 0x07;		// Blank reserved bits
	return(status);

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
	uint8_t i, status, tbuf[6];
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