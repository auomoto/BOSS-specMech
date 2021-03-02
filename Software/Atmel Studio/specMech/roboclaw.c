/*------------------------------------------------------------------------------
roboclaw.c
	Collimator motor controls
------------------------------------------------------------------------------*/

#include "roboclaw.h"

float get_ROBOVoltage(uint8_t controller)
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
	//START WORK HERE
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