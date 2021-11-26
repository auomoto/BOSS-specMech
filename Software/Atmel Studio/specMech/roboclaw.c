#include "globals.h"
#include "usart.h"
#include "timers.h"
#include "commands.h"
#include "fram.h"
#include "errors.h"
#include "roboclaw.h"

uint8_t timerSAVEENCODER, timeoutSAVEENCODER;
PID pid;

/*------------------------------------------------------------------------------
uint16_t crc16(uint8_t *packet, uint16_t nbytes)

	RoboClaw expects a CRC16 word at the end of command data packets. See
	page 59 of the RoboClaw user manual Rev 5.7 (2015) for this routine.

	Inputs:
		packet - array to compute
		nBytes - length of the array

	Returns:
		The CRC16 value, an unsigned 16-bit word
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
int16_t enc2microns(uint32_t encoderValue)
	Converts a motor encoder value to microns. The encoder value is an unsigned
		long integer (uint32) but the position in microns can be negative. The
		encoder value is offset by ENC_ZEROPOINT to keep encoder values positive
		and scaled by ENC_COUNTS_PER_MICRON.

	Input:
		encoderValue - a 32-bit unsigned integer.

	Returns:
		The equivalent motor position in microns.
------------------------------------------------------------------------------*/
/*
int16_t enc2microns(uint32_t encoderValue)
{

	int32_t temp;

	temp = (int32_t) encoderValue - ENC_ZEROPOINT;
	return((int16_t) (temp / ENC_COUNTS_PER_MICRON));	// add 1/2 the remainder?

}
*/

/*------------------------------------------------------------------------------
uint8_t getFRAM_MOTOREncoder(uint8_t controller, uint32_t *encoderValue)
	Retrieves the encoder value stored in FRAM

	Inputs:
		controller: The controller address: MOTOR_A, MOTOR_B, or MOTOR_C

	Outputs:
		encoderValue: The stored encoder value in FRAM

	Returns:
		ERROR on FRAM read error
		NOERROR otherwise
------------------------------------------------------------------------------*/
//uint8_t getFRAM_MOTOREncoder(uint8_t controller, uint32_t *encoderValue)
uint8_t get_FRAM_MOTOR_ENCODER(uint8_t controller, int32_t *encoderValue)
{

	uint8_t tbuf[4];
	uint16_t framaddr;
	int32_t tempVal;

	switch (controller) {
		case MOTOR_A:
			framaddr = ENCAFRAMADDR;
			break;

		case MOTOR_B:
			framaddr = ENCBFRAMADDR;
			break;

		case MOTOR_C:
			framaddr = ENCCFRAMADDR;
			break;

		default:
			return(ERROR);
	}

	if (read_FRAM(FRAMTWIADDR, framaddr, tbuf, 4) == ERROR) {
		*encoderValue = 0xFFFFFFFF;
		return(ERROR);
	}

	tempVal =  (uint32_t) tbuf[0] << 24;
	tempVal |= (uint32_t) tbuf[1] << 16;
	tempVal |= (uint32_t) tbuf[2] << 8;
	tempVal |= (uint32_t) tbuf[3];
	*encoderValue = tempVal;
	return(NOERROR);

}

/*------------------------------------------------------------------------------
uint8_t get_MOTOR(uint8_t mtraddr, uint8_t cmd, uint8_t* data, uint8_t nbytes)

	Requests data from the motor controller "mtraddr" and presents the raw data.

	Inputs:
		mtraddr:	MOTOR_A, MOTOR_B, or MOTOR_C (128, 129, or 130)
		command:	The motor controller packet serial command
		nbytes:		Number of bytes expected from the motor controller
					not including the two CRC bytes.

	Output:
		data:		Array to receive the data. Undefined if ERROR returned.

	Returns
		ERROR on USART timeout or CRC mismatch
		NOERROR otherwise
------------------------------------------------------------------------------*/
uint8_t get_MOTOR(uint8_t mtraddr, uint8_t cmd, uint8_t* data, uint8_t nbytes)
{

	uint8_t i, tbuf[nbytes+2];
	uint16_t crcReceived, crcExpected;

	recv1_buf.nbytes = nbytes+2;	// Set up receive buffer (+2 for crc bytes)
	recv1_buf.nxfrd = 0;
	recv1_buf.done = NO;

	tbuf[0] = mtraddr;				// Motor controller packet serial address
	tbuf[1] = cmd;					// Motor controller command
	send_USART(1, tbuf, 2);

	USART1_ticks = 0;
	start_TCB0(1);					// 1 ms tisk period for USART1_ticks
	while (recv1_buf.done == NO) {	// Wait for the reply
		if (USART1_ticks > 50) {	// Timeout about 4 ticks at 38400 baud
			stop_TCB0();
			printError(ERR_MTRREADENC, "get_MOTOR: serial timeout");
			return(ERROR);
		}
	}
	stop_TCB0();

	crcReceived = (recv1_buf.data[nbytes] << 8) | recv1_buf.data[nbytes+1];

	for (i = 2; i < nbytes+2; i++) {		// Compute expected crc value
		tbuf[i] = recv1_buf.data[i-2];
	}
	crcExpected = crc16(tbuf, nbytes+2);

	if (crcReceived != crcExpected) {
		printError(ERR_MTRENCCRC, "get_MOTOR: CRC mismatch");
		return(ERROR);
	}

	for (i = 0; i < nbytes; i++) {
		data[i] = recv1_buf.data[i];
	}

	return(NOERROR);	
	
}

/*------------------------------------------------------------------------------
uint8_t get_MOTOR_Encoder(uint8_t mtraddr, int32_t *value)

	Gets the 32-bit encoder value from mtraddr.

	Inputs:
		mtraddr: MOTOR_A, MOTOR_B, or MOTOR_C (128, 129, or 130)

	Output:
		encoderValue: The encoder count

	Returns
		ERROR on USART timeout
		NOERROR otherwise

	Also available with this command but not output here:
		status - from page 86 of version 5.7 of the manual:
			Bit0: Counter underflow (1=underflow occurred, clear after reading)
			Bit1: Direction (0=forward, 1-backwards)
			Bit2: Counter overflow (1=overflow occurred, clear after reading)
			Bits 3-7 are "reserved." Bit7 is 1.
------------------------------------------------------------------------------*/
uint8_t get_MOTOR_ENCODER(uint8_t controller, int32_t *encoderValue)
{

	uint8_t data[5];	// 5 bytes includes status, which is ignored here

	if (get_MOTOR(controller, ENCODERCOUNT, data, 5) == ERROR) {
		printError(ERR_MTR, "get_MOTOR_ENCODER: get_MOTOR error");
		return(ERROR);
	}

	*encoderValue =  (uint32_t) data[0] << 24;
	*encoderValue |= (uint32_t) data[1] << 16;
	*encoderValue |= (uint32_t) data[2] << 8;
	*encoderValue |= (uint32_t) data[3];

	return(NOERROR);

}

uint8_t get_MOTOR_SPEED(uint8_t mtraddr, int32_t *speed)
{
	
	uint8_t data[5];

	if (get_MOTOR(mtraddr, ENCODERSPEED, data, 5) == ERROR) {
		printError(ERR_MTR, "get_MOTOR_SPEED: get_MOTOR call error");
		return(ERROR);
	}

	*speed =  (uint32_t) data[0] << 24;
	*speed |= (uint32_t) data[1] << 16;
	*speed |= (uint32_t) data[2] << 8;
	*speed |= (uint32_t) data[3];

	return(NOERROR);

}

/*------------------------------------------------------------------------------
uint8_t get_MOTORFloat(uint8_t controller, uint8_t command, float *value)
	Retrieves a floating point value (voltage or temperature) from a RoboClaw
	controller.

	Inputs:
		controller:	Controller address (128, 129, 130)
		command:	READMAINVOLTAGE (command 24) or
					READTEMPERATURE (command 82)

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

	USART1_ticks = 0;
	start_TCB0(1);
	for (;;) {
		if (recv1_buf.done == YES) {	// Receive reply
			break;
		}
		if (USART1_ticks > 50) {				// Timeout
			stop_TCB0();
			printError(ERR_MTRREADENC, "get_MOTORFloat timeout");
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
uint8_t get_MOTORCurrent(uint8_t controller, uint8_t command, uint32_t *value)
	Returns a 32-bit integer from a RoboClaw controller. This could be the
	motor current (ROBOREADCURRENT 49).

	Inputs:
		controller: MOTOR_A, MOTOR_B, or MOTOR_C
		command: ROBOREADCURRENT

	Outputs:
		value: the 32-bit value

	Returns:
		ERROR: USART timeout or CRC error
		NOERROR
------------------------------------------------------------------------------*/
uint8_t get_MOTORCurrent(uint8_t controller, uint8_t command, uint32_t *value)
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

	USART1_ticks = 0;
	start_TCB0(1);
	for (;;) {
		if (recv1_buf.done == YES) {	// Receive reply
			stop_TCB0();
			break;
		}
		if (USART1_ticks > 50) {		// Timeout is usally after 3 ms (38400 baud)
			stop_TCB0();
			printError(ERR_MTRTIMEOUT, "get_MOTORCurrent timeout");
			return(ERROR);
		}
	}

	crcReceived = (recv1_buf.data[4] << 8) | recv1_buf.data[5];

	for (i = 2; i < 6; i++) {			// Compute expected CRC
		tbuf[i] = recv1_buf.data[i-2];
	}
	crcExpected = crc16(tbuf, 6);

	if (crcReceived != crcExpected) {
		printError(ERR_MTRENCCRC, "get_MOTORCurrent CRC error");
		*value = 0xFFFFFFFF;
		return(ERROR);
	}

	tempval =  (uint32_t) recv1_buf.data[0] << 24;
	tempval |= (uint32_t) recv1_buf.data[1] << 16;
	tempval |= (uint32_t) recv1_buf.data[2] << 8;
	tempval |= (uint32_t) recv1_buf.data[3];
	*value = tempval;

	return(NOERROR);

}

uint8_t get_MOTOR_PID(uint8_t controller, PID *pid)
{

	uint8_t tbuf[30];
	uint16_t crcReceived, crcExpected;
	int32_t p, i, d, maxI, deadZone, minPos, maxPos;


	recv1_buf.nbytes = 30;				// Set up receive buffer
	recv1_buf.nxfrd = 0;
	recv1_buf.done = NO;

	tbuf[0] = controller;
	tbuf[1] = READPID;					// Command 63
	send_USART(1, tbuf, 2);				// Send command

	USART1_ticks = 0;
	start_TCB0(1);
	for (;;) {
		if (recv1_buf.done == YES) {	// Receive reply
			stop_TCB0();
			break;
		}
		if (USART1_ticks > 50) {		// Timeout is usally after 3 ms (38400 baud)
			stop_TCB0();
			printError(ERR_MTRTIMEOUT, "get_MOTOR_PID timeout");
			return(ERROR);
		}
	}

	crcReceived = (recv1_buf.data[28] << 8) | recv1_buf.data[29];

	for (i = 2; i < 30; i++) {			// Compute expected CRC
		tbuf[i] = recv1_buf.data[i-2];
	}
	crcExpected = crc16(tbuf, 30);

	if (crcReceived != crcExpected) {
		printError(ERR_MTRENCCRC, "get_MOTOR_PID CRC error");
		return(ERROR);
	}

	p =  (uint32_t) recv1_buf.data[0] << 24;
	p |= (uint32_t) recv1_buf.data[1] << 16;
	p |= (uint32_t) recv1_buf.data[2] << 8;
	p |= (uint32_t) recv1_buf.data[3];
	pid->p = (float) p / 1024.0;

	i =  (uint32_t) recv1_buf.data[4] << 24;
	i |= (uint32_t) recv1_buf.data[5] << 16;
	i |= (uint32_t) recv1_buf.data[6] << 8;
	i |= (uint32_t) recv1_buf.data[7];
	pid->i = (float) i / 1024.0;

	d =  (uint32_t) recv1_buf.data[8] << 24;
	d |= (uint32_t) recv1_buf.data[9] << 16;
	d |= (uint32_t) recv1_buf.data[10] << 8;
	d |= (uint32_t) recv1_buf.data[11];
	pid->d = (float) d / 1024.0;

	maxI =  (uint32_t) recv1_buf.data[12] << 24;
	maxI |= (uint32_t) recv1_buf.data[13] << 16;
	maxI |= (uint32_t) recv1_buf.data[14] << 8;
	maxI |= (uint32_t) recv1_buf.data[15];
	pid->maxI = maxI;

	deadZone =  (uint32_t) recv1_buf.data[16] << 24;
	deadZone |= (uint32_t) recv1_buf.data[17] << 16;
	deadZone |= (uint32_t) recv1_buf.data[18] << 8;
	deadZone |= (uint32_t) recv1_buf.data[19];
	pid->deadZone = deadZone;

	minPos =  (uint32_t) recv1_buf.data[20] << 24;
	minPos |= (uint32_t) recv1_buf.data[21] << 16;
	minPos |= (uint32_t) recv1_buf.data[22] << 8;
	minPos |= (uint32_t) recv1_buf.data[23];
	pid->minPos = minPos;

	maxPos =  (uint32_t) recv1_buf.data[24] << 24;
	maxPos |= (uint32_t) recv1_buf.data[25] << 16;
	maxPos |= (uint32_t) recv1_buf.data[26] << 8;
	maxPos |= (uint32_t) recv1_buf.data[27];
	pid->maxPos = maxPos;

	return(NOERROR);
}

/*------------------------------------------------------------------------------
uint8_t get_MOTORSpeed(uint8_t controller, uint32_t *speed, uint32_t *direction)
	Gets the motor speed in encoder pulses/sec

	Inputs:
		controller: MOTOR_A, MOTOR_B, or MOTOR_C

	Outputs:
		speed: Unsigned speed in encoder pulses/sec
		direction: 0 for forward, 1 for backward

	Returns:
		ERROR: USART timeout or CRC error
		NOERROR
------------------------------------------------------------------------------*/
uint8_t get_MOTORSpeed(uint8_t controller, int32_t* speedValue, uint8_t* direction)
{

	uint8_t i, tbuf[7];
	uint16_t crcReceived, crcExpected;

	recv1_buf.nbytes = 7;			// Set up receive buffer
	recv1_buf.nxfrd = 0;
	recv1_buf.done = NO;

	tbuf[0] = controller;			// Build command
	tbuf[1] = ENCODERSPEED;			// Command 18

	send_USART(1, tbuf, 2);			// Send the command

	USART1_ticks = 0;
	start_TCB0(1);
	while (recv1_buf.done == NO) {	// Wait for the reply
		if (USART1_ticks > 50) {	// Timeout about 4 ticks at 38400 baud
			stop_TCB0();
			printError(ERR_MTRREADENC, "get_MOTORSpeed timeout");
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
		*speedValue = 0xFFFFFFFF;
		printError(ERR_MTRENCCRC, "get_MOTORSpeed CRC error");
		return(ERROR);
	}

	*speedValue =  (uint32_t) recv1_buf.data[0] << 24;
	*speedValue |= (uint32_t) recv1_buf.data[1] << 16;
	*speedValue |= (uint32_t) recv1_buf.data[2] << 8;
	*speedValue |= (uint32_t) recv1_buf.data[3];
	*direction = tbuf[4];

	return(NOERROR);

}


/*------------------------------------------------------------------------------
uint8_t init_MOTORS(void)

	Reads the last-saved encoder values from FRAM and loads them into the three
	controllers. No error message is output on first call (reboot).

	Inputs: None

	Outputs: None

	Returns
		ERROR if set_MOTOREncoder fails
		NOERROR otherwise

NEED TO FIGURE OUT WHAT TO DO ABOUT ERRORS
------------------------------------------------------------------------------*/
uint8_t init_MOTORS(void)
{

	uint8_t controller;
	int32_t encoderValue;

	_delay_ms(100);	// 50 seems to work
	timerSAVEENCODER = 0;
	timeoutSAVEENCODER = SAVEENCODERFREQUENCY;
	for (controller = MOTOR_A; controller <= MOTOR_C; controller++) {
		get_FRAM_MOTOR_ENCODER(controller, &encoderValue);
		set_MOTOREncoder(controller, encoderValue);
	}
	return(NOERROR);
}

/*------------------------------------------------------------------------------
uint32_t microns2enc(int16_t micronValue)
	Converts a motor position in microns to encoder values. The encoder value
		is a uint32 but the position in microns can be negative, so we offset
		the true encoder value by ENC_ZEROPOINT to allow this.

	Input:
		micronValue - a 16-bit signed integer that the user sees when
			interrogating the motor controller.

	Returns:
		The actual target encoder value that corresponds to the position in
			microns.
------------------------------------------------------------------------------*/
/*
uint32_t microns2enc(int16_t micronValue)
{

	int32_t temp;

	temp = ((int32_t) micronValue * ENC_COUNTS_PER_MICRON) + (uint32_t) ENC_ZEROPOINT;
	return((uint32_t) temp);

}
*/

uint8_t motorsMoving(void)
{
	uint8_t i;
	int32_t encoderSpeed;

	for (i = MOTOR_A; i <= MOTOR_C; i++) {
		if (get_MOTOR_SPEED(i, &encoderSpeed) == ERROR) {
			printError(ERR_MTR, "motorsMoving: get_MOTOR_SPEED error");
			continue;
		}
		if (encoderSpeed != 0) {
			return(YES);
		}
	}

	return(NO);

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
uint8_t move_MOTOR_CMD(uint8_t cstack)
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
			retval = get_MOTOR_ENCODER(controller, &currentPosition);
			if (retval == ERROR) {
				printError(ERR_MTR_ENC_VAL, "move_MOTOR_CMD: get_MOTOR_ENCODER error");
				return(ERROR);
			}
			break;

		default:
			printError(ERR_MTR, "move_MOTOR_CMD: unknown motor");
			return(ERROR);
			break;	
	}

	if (pcmd[cstack].cvalue[0] == '\0') {	// Don't do anything on null distance
		return(NOERROR);
	}

	newPosition = currentPosition + (atol(pcmd[cstack].cvalue) * ENC_COUNTS_PER_MICRON);

//	return(move_MOTORAbsolute(controller, newPosition));

	if (move_MOTOR(controller, newPosition) == ERROR) {
		printError(ERR_MTR, "move_MOTOR_CMD: move_MOTOR call error");
		return(ERROR);
	}

	return(ERROR);

}

uint8_t move_MOTOR(uint8_t mtraddr, int32_t newPosition)
{

	uint8_t buffer, data[17], nbytes;
	uint32_t acceleration, deceleration, speed;

	nbytes = 17;
	buffer = 0;						// Operation is buffered
	acceleration = ACCELERATION;	// See roboclaw.h
	deceleration = DECELERATION;
	speed = SPEED;

	data[0] = (acceleration >> 24) & 0XFF;
	data[1] = (acceleration >> 16) & 0xFF;
	data[2] = (acceleration >> 8) & 0xFF;
	data[3] = (acceleration) & 0xFF;
	data[4] = (speed >> 24) & 0xFF;
	data[5] = (speed >> 16) & 0xFF;
	data[6] = (speed >> 8) & 0xFF;
	data[7] = (speed) & 0xFF;
	data[8] = (deceleration >> 24) & 0xFF;
	data[9] = (deceleration >> 16) & 0xFF;
	data[10] = (deceleration >> 8) & 0xFF;
	data[11] = (deceleration) & 0xFF;
	data[12] = (newPosition >> 24) & 0xFF;
	data[13] = (newPosition >> 16) & 0xFF;
	data[14] = (newPosition >> 8) & 0xFF;
	data[15] = (newPosition) & 0xFF;
	data[16] = buffer;

	if (put_MOTOR(mtraddr, DRIVETO, data, nbytes) == ERROR) {
		printError(ERR_MTR, "move_MOTOR: put_MOTOR call error");
		return(ERROR);
	}

	return(NOERROR);

}

/*------------------------------------------------------------------------------
uint8_t move_MOTORAbsolute(uint8_t controller, uint32_t newPosition)
	Move the motor on the selected controller to a new absolute position.

	Inputs:
		controller: controller address (128, 129, or 130)
		newPosition: the new encoder position in native encoder units

	Returns:
		ERROR on USART timeout or bad (not 0xFF) ack
		NOERROR otherwise
------------------------------------------------------------------------------*/
//uint8_t move_MOTORAbsolute(uint8_t controller, uint32_t newPosition)
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

	USART1_ticks = 0;
	start_TCB0(1);						// Start 1 ms ticks timer
	for (;;) {
		if (recv1_buf.done == YES) {	// Reply received
			stop_TCB0();
			break;
		}
		if (USART1_ticks > 50) {				// 4 ms just barely works at 38400 baud
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
uint8_t putFRAM_MOTOREncoder(uint8_t controller)

	Stores the encoder value in FRAM.

	Inputs:
		controller: The controller address (128, 129, or 130)

	Outputs:
		None

	Returns:
		ERROR on get_MOTOREncoder or write_FRAM failure
		NOERROR otherwise
------------------------------------------------------------------------------*/
uint8_t putFRAM_MOTOREncoder(uint8_t controller)
{

	uint8_t tbuf[4];
	uint16_t memaddr;
//	uint32_t encoderValue;
	int32_t encoderValue;

	switch (controller) {
		case MOTOR_A:
			memaddr = ENCAFRAMADDR;
			break;

		case MOTOR_B:
			memaddr = ENCBFRAMADDR;
			break;

		case MOTOR_C:
			memaddr = ENCCFRAMADDR;
			break;

		default:
			return(ERROR);
	}

	get_MOTOR_ENCODER(controller, &encoderValue);

	tbuf[0] = (encoderValue >> 24) & 0xFF;
	tbuf[1] = (encoderValue >> 16) & 0xFF;
	tbuf[2] = (encoderValue >> 8) & 0xFF;
	tbuf[3] = encoderValue & 0xFF;
	return(write_FRAM(FRAMTWIADDR, memaddr, tbuf, 4));

}

uint8_t put_MOTOR(uint8_t mtraddr, uint8_t cmd, uint8_t* data, uint8_t nbytes)
{
	uint8_t i, tbuf[nbytes+4];
	uint16_t crc;

	recv1_buf.data[0] = 0x00;			// Set up receiving buffer
	recv1_buf.nbytes = 1;
	recv1_buf.nxfrd = 0;
	recv1_buf.done = NO;

	tbuf[0] = mtraddr;
	tbuf[1] = cmd;
	for (i = 0; i < nbytes; i++) {
		tbuf[i+2] = data[i];
	}
	crc = crc16(tbuf, nbytes+2);
	tbuf[nbytes+2] = (crc >> 8) & 0xFF;
	tbuf[nbytes+3] = crc & 0xFF;

	send_USART(1, tbuf, nbytes+4);		// Send the command

	USART1_ticks = 0;
	start_TCB0(1);						// Start 1 ms USART1_ticks timer
	for (;;) {
		if (recv1_buf.done == YES) {	// Reply received
			stop_TCB0();
			break;
		}
		if (USART1_ticks > 50) {
			stop_TCB0();
			printError(ERR_MTR, "put_MOTOR: serial timeout");
			return(ERROR);
		}
	}
	if (recv1_buf.data[0] != 0xFF) {	// Bad ack
		printError(ERR_MTR, "put_MOTOR: bad ACK");
		return(ERROR);
	}
	return(NOERROR);
}

uint8_t put_MOTOR_ENCODER(uint8_t mtraddr, int32_t encoderValue)
{
	uint8_t tbuf[8];
	uint16_t crc;

	recv1_buf.data[0] = 0x00;			// Set up receiving buffer
	recv1_buf.nbytes = 1;
	recv1_buf.nxfrd = 0;
	recv1_buf.done = NO;

	tbuf[0] = mtraddr;
	tbuf[1] = PUTENCODER;
	tbuf[2] = (encoderValue >> 24) & 0xFF;
	tbuf[3] = (encoderValue >> 16) & 0xFF;
	tbuf[4] = (encoderValue >> 8) & 0xFF;
	tbuf[5] = encoderValue & 0xFF;
	crc = crc16(tbuf, 6);
	tbuf[6] = (crc >> 8) & 0xFF;
	tbuf[7] = crc & 0xFF;
	
	send_USART(1, tbuf, 8);				// Send the command

	USART1_ticks = 0;
	start_TCB0(1);						// Start 1 ms ticks timer

	for (;;) {
		if (recv1_buf.done == YES) {	// Reply received
			stop_TCB0();
			break;
		}
		if (USART1_ticks > 50) {				// 4 ms barely works at 38400 baud
			stop_TCB0();
			printError(ERR_MTR, "put_MOTOR_ENCODER: serial timeout");
			return(ERROR);
		}
	}

	if (recv1_buf.data[0] != 0xFF) {	// Bad ack
		printError(ERR_MTRTIMEOUT, "put_MOTOREncoder bad ack");
		return(ERROR);
	}

	return(NOERROR);

}

uint8_t saveFRAM_MOTOREncoders(void)
{
	uint8_t i, error = 0, retval;

	for (i = MOTOR_A; i <= MOTOR_C; i++) {
		retval = putFRAM_MOTOREncoder(i);
		error += retval;
	}
	if (error) {
		return(ERROR);
	} else {
		return(NOERROR);
	}
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
//uint8_t set_MOTOREncoder(uint8_t controller, uint32_t value)
uint8_t set_MOTOREncoder(uint8_t controller, int32_t value)
{

	uint8_t tbuf[8];
	uint16_t crc = 0;
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
	crc = crc16(tbuf, 6);
	tbuf[6] = (crc >> 8) & 0xFF;
	tbuf[7] = crc & 0xFF;

	send_USART(1, tbuf, 8);				// Send the command

	USART1_ticks = 0;
	start_TCB0(1);						// Start 1 ms ticks timer

	for (;;) {
		if (recv1_buf.done == YES) {	// Reply received
			stop_TCB0();
			break;
		}
		if (USART1_ticks > 50) {				// 4 ms barely works at 38400 baud
			stop_TCB0();
			printError(ERR_MTRTIMEOUT, "set_MOTOREncoder timeout");
			return(ERROR);
		}

	}

	if (recv1_buf.data[0] != 0xFF) {	// Bad ack
		printError(ERR_MTRTIMEOUT, "set_MOTOREncoder bad ack");
		return(ERROR);
	}

	return(NOERROR);

}

uint8_t set_MOTOR_PID(uint8_t controller, PID pid)
{

	uint8_t tbuf[32];
	uint16_t crc = 0;
	int32_t p, i, d;

	p = (int32_t) (pid.p * 1024.0);
	i = (int32_t) (pid.i * 1024.0);
	d = (int32_t) (pid.d * 1024.0);

	recv1_buf.data[0] = 0x00;			// Set up receiving buffer
	recv1_buf.nbytes = 1;
	recv1_buf.nxfrd = 0;
	recv1_buf.done = NO;

	tbuf[0] = controller;
	tbuf[1] = SETPID;
	tbuf[2] = (d >> 24) & 0xFF;
	tbuf[3] = (d >> 16) & 0xFF;
	tbuf[4] = (d >> 8) & 0xFF;
	tbuf[5] = d & 0xFF;
	tbuf[6] = (p >> 24) & 0xFF;
	tbuf[7] = (p >> 16) & 0xFF;
	tbuf[8] = (p >> 8) & 0xFF;
	tbuf[9] = p & 0xFF;
	tbuf[10] = (i >> 24) & 0xFF;
	tbuf[11] = (i >> 16) & 0xFF;
	tbuf[12] = (i >> 8) & 0xFF;
	tbuf[13] = i & 0xFF;
	tbuf[14] = (pid.maxI >> 24) & 0xFF;
	tbuf[15] = (pid.maxI >> 16) & 0xFF;
	tbuf[16] = (pid.maxI >> 8) & 0xFF;
	tbuf[17] = pid.maxI & 0xFF;
	tbuf[18] = (pid.deadZone >> 24) & 0xFF;
	tbuf[19] = (pid.deadZone >> 16) & 0xFF;
	tbuf[20] = (pid.deadZone >> 8) & 0xFF;
	tbuf[21] = pid.deadZone & 0xFF;
	tbuf[22] = (pid.minPos >> 24) & 0xFF;
	tbuf[23] = (pid.minPos >> 16) & 0xFF;
	tbuf[24] = (pid.minPos >> 8) & 0xFF;
	tbuf[25] = pid.minPos & 0xFF;
	tbuf[26] = (pid.maxPos >> 24) & 0xFF;
	tbuf[27] = (pid.maxPos >> 16) & 0xFF;
	tbuf[28] = (pid.maxPos >> 8) & 0xFF;
	tbuf[29] = pid.maxPos & 0xFF;
	crc = crc16(tbuf, 30);
	tbuf[30] = (crc >> 8) & 0xFF;
	tbuf[31] = crc & 0xFF;

	send_USART(1, tbuf, 32);				// Send the command

	USART1_ticks = 0;
	start_TCB0(1);						// Start 1 ms ticks timer

	for (;;) {
		if (recv1_buf.done == YES) {	// Reply received
			stop_TCB0();
			break;
		}
		if (USART1_ticks > 50) {				// 4 ms barely works at 38400 baud
			stop_TCB0();
			printError(ERR_MTRTIMEOUT, "set_MOTOR_PID timeout");
			return(ERROR);
		}
	}

	if (recv1_buf.data[0] != 0xFF) {	// Bad ack
		printError(ERR_MTRTIMEOUT, "set_MOTOR_PID bad ack");
		return(ERROR);
	}

	return(NOERROR);

}