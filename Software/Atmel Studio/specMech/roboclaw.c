#include "globals.h"
#include "usart.h"
#include "timers.h"
#include "commands.h"
#include "fram.h"
#include "errors.h"
#include "roboclaw.h"

volatile uint8_t timerSAVEENCODER;
uint8_t timeoutSAVEENCODER;
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
uint8_t getFRAM_MOTOR_ENCODER(uint8_t controller, uint32_t *encoderValue)
	Retrieves the encoder value in FRAM

	Inputs:
		controller: The controller address: MOTOR_A, MOTOR_B, or MOTOR_C

	Outputs:
		encoderValue: The stored encoder value

	Returns:
		ERROR on FRAM read error
		NOERROR otherwise
------------------------------------------------------------------------------*/
uint8_t get_FRAM_MOTOR_ENCODER(uint8_t controller, int32_t *encoderValue)
{

	uint8_t tbuf[4];
	uint16_t framaddr;
	int32_t tempVal;

	switch (controller) {
		case MOTOR_A:
			framaddr = ENCA_FRAMADDR;
			break;

		case MOTOR_B:
			framaddr = ENCB_FRAMADDR;
			break;

		case MOTOR_C:
			framaddr = ENCC_FRAMADDR;
			break;

		default:
			return(ERROR);
	}

	if (read_FRAM(FRAMTWIADDR, framaddr, tbuf, 4) == ERROR) {
		printError(ERR_FRAM, "get_FRAM_MOTOR_ENCODER: read_FRAM error");
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

	Retrieves raw data from the motor controller "mtraddr"

	Inputs:
		mtraddr:	MOTOR_A, MOTOR_B, or MOTOR_C (128, 129, or 130)
		command:	The motor controller packet serial command
		nbytes:		Number of bytes expected from the motor controller,
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
uint8_t get_MOTOR_ENCODER(uint8_t mtraddr, int32_t *value)

	Retrieves the 32-bit encoder value from the controller at mtraddr

	Inputs:
		mtraddr: MOTOR_A, MOTOR_B, or MOTOR_C (128, 129, or 130)

	Output:
		encoderValue: The encoder value

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
/*------------------------------------------------------------------------------
uint8_t get_MOTOR_FLOAT(uint8_t controller, uint8_t command, float *value)
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
uint8_t get_MOTOR_FLOAT(uint8_t mtraddr, uint8_t cmd, float* value)
{

	uint8_t data[2];

	if (get_MOTOR(mtraddr, cmd, data, 2) == ERROR) {
		printError(ERR_MTR, "get_MOTOR_FLOAT: get_MOTOR error");
		*value = BADFLOAT;
		return(ERROR);
	}
	*value = ((float) ((data[0] << 8) | data[1])) / 10.0;
	return(NOERROR);
}


/*------------------------------------------------------------------------------
uint8_t get_MOTOR_SPEED(uint8_t mtraddr, int32_t *speed)

	Retrieves the motor speed from the controller at mtraddr

	Inputs:
		mtraddr: MOTOR_A, MOTOR_B, or MOTOR_C (128, 129, or 130)

	Output:
		speed: The speed in encoder counts/sec. 2-s complement value.

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

uint8_t get_MOTOR_CURRENT(uint8_t mtraddr, uint16_t *current)
{
	uint8_t data[4];

	if (get_MOTOR(mtraddr, ROBOREADCURRENT, data, 4) == ERROR) {
		printError(ERR_MTR, "get_MOTOR_CURRENT: get_MOTOR error");
		*current = 0xFFFF;
		return(ERROR);
	}
	*current = (((uint16_t) data[0] << 8) | (uint16_t) data[1]) * 10;
	return(NOERROR);	
}

uint8_t get_MOTOR_PID(uint8_t mtraddr, PID *pid)
{
	uint8_t data[28];
	int32_t p, i, d, maxI, deadZone, minPos, maxPos;

	if (get_MOTOR(mtraddr, READPID, data, 28) == ERROR) {
		printError(ERR_MTR, "get_MOTOR_PID: get_MOTOR error");
		return(ERROR);
	}

	p =  (uint32_t) data[0] << 24;
	p |= (uint32_t) data[1] << 16;
	p |= (uint32_t) data[2] << 8;
	p |= (uint32_t) data[3];
	pid->p = (float) p / 1024.0;

	i =  (uint32_t) data[4] << 24;
	i |= (uint32_t) data[5] << 16;
	i |= (uint32_t) data[6] << 8;
	i |= (uint32_t) data[7];
	pid->i = (float) i / 1024.0;

	d =  (uint32_t) data[8] << 24;
	d |= (uint32_t) data[9] << 16;
	d |= (uint32_t) data[10] << 8;
	d |= (uint32_t) data[11];
	pid->d = (float) d / 1024.0;

	maxI =  (uint32_t) data[12] << 24;
	maxI |= (uint32_t) data[13] << 16;
	maxI |= (uint32_t) data[14] << 8;
	maxI |= (uint32_t) data[15];
	pid->maxI = maxI;

	deadZone =  (uint32_t) data[16] << 24;
	deadZone |= (uint32_t) data[17] << 16;
	deadZone |= (uint32_t) data[18] << 8;
	deadZone |= (uint32_t) data[19];
	pid->deadZone = deadZone;

	minPos =  (uint32_t) data[20] << 24;
	minPos |= (uint32_t) data[21] << 16;
	minPos |= (uint32_t) data[22] << 8;
	minPos |= (uint32_t) data[23];
	pid->minPos = minPos;

	maxPos =  (uint32_t) data[24] << 24;
	maxPos |= (uint32_t) data[25] << 16;
	maxPos |= (uint32_t) data[26] << 8;
	maxPos |= (uint32_t) data[27];
	pid->maxPos = maxPos;

	return(NOERROR);

}

/*------------------------------------------------------------------------------
uint8_t init_MOTORS(void)

	Reads the last-saved encoder values from FRAM and loads them into the three
	controllers. No error message is output on first call (reboot).

	Inputs: None

	Outputs: None

	Returns
		ERROR if put_MOTOR_ENCODER fails
		NOERROR otherwise

ADD ERROR CHECKS?
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
		put_MOTOR_ENCODER(controller, encoderValue);
	}
	return(NOERROR);
}

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
uint8_t put_FRAM_ENCODERS(void)

	Store the encoder values in FRAM.

	Inputs:
		None

	Outputs:
		None

	Returns:
		ERROR on get_MOTOREncoder or write_FRAM failure
		NOERROR otherwise
------------------------------------------------------------------------------*/
uint8_t put_FRAM_ENCODERS(void)
{

	uint8_t i, tbuf[4], errorFlag, mtraddr[3] = {MOTOR_A, MOTOR_B, MOTOR_C};
	uint16_t memaddr[3] = {ENCA_FRAMADDR, ENCB_FRAMADDR, ENCC_FRAMADDR};
	int32_t encoderValue;

	errorFlag = 0;
	for (i = 0; i < 3; i++) {
		if (get_MOTOR_ENCODER(mtraddr[i], &encoderValue) == ERROR) {
			printError(ERR_MTR, "put_FRAM_ENCODERS: get_MOTOR_ENCODER error");
			errorFlag = 1;
		}
		tbuf[0] = (encoderValue >> 24) & 0xFF;
		tbuf[1] = (encoderValue >> 16) & 0xFF;
		tbuf[2] = (encoderValue >> 8) & 0xFF;
		tbuf[3] = encoderValue & 0xFF;
		if (write_FRAM(FRAMTWIADDR, memaddr[i], tbuf, 4) == ERROR) {
			printError(ERR_FRAM, "put_FRAM_ENCODERS: write_FRAM error");
			errorFlag = 1;
		}
	}
	if (errorFlag) {
		return(ERROR);
	} else {
		return(NOERROR);
	}
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

	uint8_t data[4];
	
	data[0] = (encoderValue >> 24) & 0xFF;
	data[1] = (encoderValue >> 16) & 0xFF;
	data[2] = (encoderValue >> 8) & 0xFF;
	data[3] = encoderValue & 0xFF;
	if (put_MOTOR(mtraddr, PUTENCODER, data, 4) == ERROR) {
		printError(ERR_MTR, "put_MOTOR_ENCODER: put_MOTOR error");
		return(ERROR);
	}
	return(NOERROR);

}

uint8_t put_MOTOR_PID(uint8_t mtraddr, PID pid)
{
	uint8_t data[28];
	int32_t p, i, d;

	p = (int32_t) (pid.p * 1024.0);
	i = (int32_t) (pid.i * 1024.0);
	d = (int32_t) (pid.d * 1024.0);

	data[0] = (d >> 24) & 0xFF;
	data[1] = (d >> 16) & 0xFF;
	data[2] = (d >> 8) & 0xFF;
	data[3] = d & 0xFF;
	data[4] = (p >> 24) & 0xFF;
	data[5] = (p >> 16) & 0xFF;
	data[6] = (p >> 8) & 0xFF;
	data[7] = p & 0xFF;
	data[8] = (i >> 24) & 0xFF;
	data[9] = (i >> 16) & 0xFF;
	data[10] = (i >> 8) & 0xFF;
	data[11] = i & 0xFF;
	data[12] = (pid.maxI >> 24) & 0xFF;
	data[13] = (pid.maxI >> 16) & 0xFF;
	data[14] = (pid.maxI >> 8) & 0xFF;
	data[15] = pid.maxI & 0xFF;
	data[16] = (pid.deadZone >> 24) & 0xFF;
	data[17] = (pid.deadZone >> 16) & 0xFF;
	data[18] = (pid.deadZone >> 8) & 0xFF;
	data[19] = pid.deadZone & 0xFF;
	data[20] = (pid.minPos >> 24) & 0xFF;
	data[21] = (pid.minPos >> 16) & 0xFF;
	data[22] = (pid.minPos >> 8) & 0xFF;
	data[23] = pid.minPos & 0xFF;
	data[24] = (pid.maxPos >> 24) & 0xFF;
	data[25] = (pid.maxPos >> 16) & 0xFF;
	data[26] = (pid.maxPos >> 8) & 0xFF;
	data[27] = pid.maxPos & 0xFF;

	if (put_MOTOR(mtraddr, SETPID, data, 28) == ERROR) {
		printError(ERR_MTR, "put_MOTOR_PID: put_MOTOR error");
		return(ERROR);
	}
	return(NOERROR);
}
