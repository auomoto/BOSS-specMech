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

	RoboClaw sends a CRC16 word at the end of data packets and requires a CRC16
	word at the end of commands that send data (not just a command). See page 59
	of the RoboClaw user manual Rev 5.7 (2015) for this routine.

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

	Retrieves the encoder value in FRAM.

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

	const char fmt1[] ="get_FRAM_MOTOR_ENCODER: read_FRAM error for %c";
	char strbuf[80];
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
		sprintf(strbuf, fmt1, (char) (controller-63));
		printError(ERR_FRAM, strbuf);
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

	char strbuf[80];
	uint8_t i, nbytesp2, tbuf[4];
	uint16_t crc, crcReceived, crcExpected;
	const char fmt1[] = "get_MOTOR: serial timeout on %c";
	const char fmt2[] = "get_MOTOR: CRC mismatch on %c";

	nbytesp2 = nbytes + 2;
	ser_recv1.nxfrd = 0;			// Set up receive buffer
	ser_recv1.n2xfr = nbytesp2;		// Data plus CRC

	tbuf[0] = mtraddr;				// Motor controller serial address
	tbuf[1] = cmd;					// Motor controller command
	crc = crc16(tbuf, 2);
	tbuf[2] = (crc >> 8) & 0xFF;
	tbuf[3] = crc & 0xFF;

#ifdef DEBUG
	sprintf(strbuf, " addr=%d cmd=%d crc=0x%04X n2xfr=%d", mtraddr, cmd, crc, nbytesp2);
	printLine(strbuf);
#endif

	send_USART1(tbuf, 2);

//_delay_ms(20); // This was needed for the Unix version

	USART1_ticks = 0;
	while (ser_recv1.nxfrd < ser_recv1.n2xfr) {
		if (USART1_ticks > 100) {							// Check the number thoroughly
			sprintf(strbuf, fmt1, (char) (mtraddr-63));
			printError(ERR_MTRREADENC, strbuf);
			return(ERROR);
		}
	}

#ifdef DEBUG
	sprintf(strbuf, " get_MOTOR USART1_ticks=%d nxfrd=%d", USART1_ticks, ser_recv1.nxfrd);
	printLine(strbuf);
#endif

	crcReceived = (ser_recv1.data[nbytes] << 8) | ser_recv1.data[nbytes+1];

	for (i = 2; i < nbytesp2; i++) {		// Compute expected crc value
		tbuf[i] = ser_recv1.data[i-2];
	}
	crcExpected = crc16(tbuf, nbytesp2);

	if (crcReceived != crcExpected) {
		sprintf(strbuf, fmt2, (char) (mtraddr-63));
		printError(ERR_MTRENCCRC, strbuf);
		return(ERROR);
	}

	for (i = 0; i < nbytes; i++) {
		data[i] = ser_recv1.data[i];
	}

	return(NOERROR);	

}

/*------------------------------------------------------------------------------
uint8_t get_MOTOR_CURRENT(uint8_t mtraddr, int32_t *current)

	Retrieves motor current from mtraddr

	Inputs:
		mtraddr: MOTOR_A, MOTOR_B, or MOTOR_C (128, 129, or 130)

	Output:
		current: The current in mA

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
uint8_t get_MOTOR_CURRENT(uint8_t mtraddr, uint16_t *current)
{

	const char fmt1[] = "get_MOTOR_CURRENT: get_MOTOR error on %c";
	char strbuf[80];
	uint8_t data[4];

	if (get_MOTOR(mtraddr, READCURRENT, data, 4) == ERROR) {
		sprintf(strbuf, fmt1, (char) (mtraddr-63));
		printError(ERR_MTR, strbuf);
		*current = 0xFFFF;
		return(ERROR);
	}

	*current = (((uint16_t) data[0] << 8) | (uint16_t) data[1]) * 10;

	return(NOERROR);

}

/*------------------------------------------------------------------------------
uint8_t get_MOTOR_ENCODER(uint8_t mtraddr, int32_t *value)

	Retrieves the 32-bit encoder value from the controller at mtraddr

	Inputs:
		mtraddr: MOTOR_A, MOTOR_B, or MOTOR_C (128, 129, or 130)

	Output:
		encoderValue: The raw encoder value

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

	const char fmt1[] = "get_MOTOR_ENCODER: get_MOTOR error on %c";
	char strbuf[80];
	uint8_t data[5];	// 5 bytes includes status, which is ignored here

	if (get_MOTOR(controller, ENCODERCOUNT, data, 5) == ERROR) {
		sprintf(strbuf, fmt1, (char) (controller-63));
		printError(ERR_MTR, strbuf);
		return(ERROR);
	}

	*encoderValue =  (uint32_t) data[0] << 24;
	*encoderValue |= (uint32_t) data[1] << 16;
	*encoderValue |= (uint32_t) data[2] << 8;
	*encoderValue |= (uint32_t) data[3];

#ifdef DEBUG
	sprintf(strbuf, " encval=%ld", *encoderValue);
	printLine(strbuf);
#endif

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

	const char fmt1[] = "get_MOTOR_FLOAT: get_MOTOR error on %c";
	char strbuf[80];
	uint8_t data[2];

	if (get_MOTOR(mtraddr, cmd, data, 2) == ERROR) {
		sprintf(strbuf, fmt1, (char) (mtraddr-63));
		printError(ERR_MTR, strbuf);
		*value = BADFLOAT;
		return(ERROR);
	}

	*value = ((float) ((data[0] << 8) | data[1])) / 10.0;

	return(NOERROR);

}

/*------------------------------------------------------------------------------
uint8_t get_MOTOR_MAXCURRENT(uint8_t mtraddr, int32_t *maxCurrent)
	Retrieve the maximum current allowed by the controller.

	Input:
		mtraddr - the controller address

	Output:
		maxCurrent - the maximum current allowed in mA

	Returns:
		ERROR on get_MOTOR error
		NOERROR otherwise
------------------------------------------------------------------------------*/
uint8_t get_MOTOR_MAXCURRENT(uint8_t mtraddr, int32_t *maxCurrent)
{

	const char fmt1[] = "get_MOTOR_MAXCURRENT: get_MOTOR error on %c";
	char strbuf[80];
	uint8_t data[8];

	if (get_MOTOR(mtraddr, GETMAXCURRENT, data, 8) == ERROR) {
		sprintf(strbuf, fmt1, (char) (mtraddr-63));
		printError(ERR_MTR, strbuf);
		return(ERROR);
	}

	*maxCurrent =  (uint32_t) data[0] << 24;
	*maxCurrent |= (uint32_t) data[1] << 16;
	*maxCurrent |= (uint32_t) data[2] << 8;
	*maxCurrent |= (uint32_t) data[3];
	*maxCurrent *= 10;	// Convert to mA

	return(NOERROR);

}

/*------------------------------------------------------------------------------
uint8_t get_MOTOR_PID(uint8_t mtraddr, PID *pid)
	Retrieves the PID, maxI, deadZone, minPos, and maxPos from mtraddr

	Inputs:
		mtraddr: The motor controller address(MOTOR_A, MOTOR_B, or MOTOR_C)

	Outputs:
		pid: The stored parameters in the controller

	Returns:
		ERROR: If get_MOTOR routine fails
------------------------------------------------------------------------------*/
uint8_t get_MOTOR_PID(uint8_t mtraddr, PID *pid)
{

	const char fmt1[] = "get_MOTOR_PID: get_MOTOR error on %c";
	char strbuf[80];
	uint8_t data[28];
	int32_t p, i, d, maxI, deadZone, minPos, maxPos, qpps;

	if (get_MOTOR(mtraddr, READPID, data, 28) == ERROR) {
		sprintf(strbuf, fmt1, (char) (mtraddr-63));
		printError(ERR_MTR, strbuf);
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

	if (get_MOTOR(mtraddr, READQPPS, data, 16) == ERROR) {
		sprintf(strbuf, fmt1, (char) (mtraddr-63));
		printError(ERR_MTR, strbuf);
		return(ERROR);
	}

	qpps = (uint32_t) data[12] << 24;
	qpps |= (uint32_t) data[13] << 16;
	qpps |= (uint32_t) data[14] << 8;
	qpps |= (uint32_t) data[15];
	pid->qpps = qpps;

	return(NOERROR);

}

/*------------------------------------------------------------------------------
uint8_t get_MOTOR_S4MODE(uint8_t mtraddr, uint8_t *mode)
	Retrieve the S4 input pin behavior. The S4 pin is connected to both the
	forward and rearward limit switches. We want to see 0x72 mode, which means
	the S4 pin treats one switch as the home position and the other as a limit.

	Inputs:
		mtraddr:	The controller serial address

	Outputs:
		*mode:		The retrieved mode for the S4 pin (should be 0x72)

	Returns:
		ERROR if the get_MOTOR routine fails
		NOERROR otherwise

	Note: One of the modes, Motor 1 Limit(Both) returns 0x02 when it should
		return 0x32
------------------------------------------------------------------------------*/
uint8_t get_MOTOR_S4MODE(uint8_t mtraddr, uint8_t *mode)
{

	const char fmt1[] = "get_MOTOR_S4: get_MOTOR error on %c";
	char strbuf[80];
	uint8_t data[3];

	if (get_MOTOR(mtraddr, GETS4MODE, data, 3) == ERROR) {
		sprintf(strbuf, fmt1, (char) (mtraddr-63));
		printError(ERR_MTR, strbuf);
		*mode = 0xFF; 
		return(ERROR);
	}

	*mode = data[1];

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

	const char fmt1[] = "get_MOTOR_SPEED: get_MOTOR call error on %c";
	char strbuf[80];
	uint8_t data[5];

	if (get_MOTOR(mtraddr, ENCODERSPEED, data, 5) == ERROR) {
		sprintf(strbuf, fmt1, (char) (mtraddr-63));
		printError(ERR_MTR, strbuf);
		return(ERROR);
	}

	*speed =  (uint32_t) data[0] << 24;
	*speed |= (uint32_t) data[1] << 16;
	*speed |= (uint32_t) data[2] << 8;
	*speed |= (uint32_t) data[3];

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
	PID pid;

	_delay_ms(100);

	pid.p = PID_P;
	pid.i = PID_I;
	pid.d = PID_D;
	pid.maxI = PID_MAXI;
	pid.deadZone = PID_DEADZONE;
	pid.minPos = PID_MINPOS;
	pid.maxPos = PID_MAXPOS;
	pid.qpps = PID_QPPS;

	timerSAVEENCODER = 0;
	timeoutSAVEENCODER = SAVEENCODERFREQUENCY;

	for (controller = MOTOR_A; controller <= MOTOR_C; controller++) {
		get_FRAM_MOTOR_ENCODER(controller, &encoderValue);
		put_MOTOR_ENCODER(controller, encoderValue);

/*
		put_MOTOR_MAXCURRENT(controller, MAXCURRENT);
		put_MOTOR_PID(controller, pid);
		put_MOTOR_S4MODE(controller);
*/

	}

	return(NOERROR);

}

/*------------------------------------------------------------------------------
uint8_t motorsMoving(uint8_t mtraddr)

	Checks the encoderspeed

	Input:
		mtraddr: The controller address

	Output:
		None

	Returns:
		YES if the motor is moving
		NO if it isn't
------------------------------------------------------------------------------*/
uint8_t motorMoving(uint8_t mtraddr)
{
	const char fmt0[] = "motorMoving: get_MOTOR_SPEED error on %c";
	char strbuf[80];
	uint32_t encoderSpeed;

	encoderSpeed = 0;
	if (get_MOTOR_SPEED(mtraddr, &encoderSpeed) == ERROR) {
		sprintf(strbuf, fmt0, (char) (mtraddr-63));
		printError(ERR_MTR, strbuf);
		return(ERROR);
	}
	if (encoderSpeed != 0) {
		return(YES);
	} else {
		return(NO);
	}

}


/*------------------------------------------------------------------------------
uint8_t motorsMoving(void)
	Returns YES if any motors are moving. Emits an error message if a
	controller does not respond, but does not return "ERROR"
------------------------------------------------------------------------------*/
uint8_t motorsMoving(void)
{

	const char fmt1[] = "motorsMoving: get_MOTOR_SPEED error on %c";
	char strbuf[80];
	uint8_t i, nmoving;
	int32_t encoderSpeed;

	encoderSpeed = 0;
	nmoving = 0;

	for (i = MOTOR_A; i <= MOTOR_A; i++) {
//	for (i = MOTOR_A; i <= MOTOR_C; i++) {
		if (get_MOTOR_SPEED(i, &encoderSpeed) == ERROR) {
			sprintf(strbuf, fmt1, (char) (i-63));
			printError(ERR_MTR, strbuf);
			continue;
		}
		if (encoderSpeed != 0) {
			nmoving++;
		}
	}

	return(nmoving);

}

/*------------------------------------------------------------------------------
uint8_t move_MOTOR(uint8_t mtraddr, int32_t newPosition)
	Move to newPosition (encoder units).

	Input:
		mtraddr:		MOTOR_A, MOTOR_B, or MOTOR_C
		newPosition:	The target encoder value

	Returns:
		ERROR if an unknown motor designator
		NOERROR otherwise
------------------------------------------------------------------------------*/
uint8_t move_MOTOR(uint8_t mtraddr, int32_t newPosition)
{

	const char fmt1[] = "move_MOTOR: put_MOTOR error on %c";
	char strbuf[80];
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

sprintf(strbuf, "newpos=%ld", newPosition);
printLine(strbuf);

	if (put_MOTOR(mtraddr, DRIVETO, data, nbytes) == ERROR) {
		sprintf(strbuf, fmt1, (char) (mtraddr-63));
		printError(ERR_MTR, strbuf);
		return(ERROR);
	}

	return(NOERROR);

}

/*------------------------------------------------------------------------------
uint8_t move_MOTOR_CMD(uint8_t cstack)
	Interprets the m command and calls for the move

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
		case 'x':
		case 'X':
			stop_MOTOR(128);
//			stop_MOTOR(129);		// NEED TO AVOID BUFFER OVERWRITE?
//			stop_MOTOR(130);
			return(NOERROR);

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
		printError(ERR_MTR, "move_MOTOR_CMD: move_MOTOR error");
		return(ERROR);
	}

	return(ERROR);

}

uint8_t move_MOTOR_HOME(void)
{

	const char fmt1[] = "moveMOTOR_HOME: get_MOTOR_ENCODER error on %c";
	const char fmt2[] = "moveMOTOR_HOME: move_MOTOR error on %c";
	char strbuf[80];
	uint8_t i, mtraddr;
	int32_t curPos[3], avgPos;

	avgPos = 0;

	for (i = 0; i < 1; i++) {
//	for (i = 0; i < 4; i++) {
		mtraddr = i+128;
		if (get_MOTOR_ENCODER(mtraddr, &curPos[i]) == ERROR) {
			sprintf(strbuf, fmt1, mtraddr-63);
			printError(ERR_MTR, strbuf);
			return(ERROR);
		}
		avgPos += curPos[i];
	}
	avgPos /= 3;

sprintf(strbuf, " avgPos = %ld", avgPos/ENC_COUNTS_PER_MICRON);
printLine(strbuf);

	for (i = 0; i < 1; i++) {
//	for (i = 0; i < 4; i++) {
		mtraddr = i+128;
		if (move_MOTOR(mtraddr, avgPos) == ERROR) {
			sprintf(strbuf, fmt2, mtraddr-63);
			printError(ERR_MTR, strbuf);
			return(ERROR);
		}
	}

i = 0;
	_delay_ms(500);		// give the motor a chance to start up
	while (motorsMoving()) {
sprintf(strbuf, " moving %d", i);
printLine(strbuf);
		_delay_ms(1000);
		i++;
	}

sprintf(strbuf, " stopped after %d", i);
printLine(strbuf);

	for (i = 0; i < 1; i++) {
//	for (i = 0; i < 4; i++) {
		mtraddr = i+128;
		if (get_MOTOR_ENCODER(mtraddr, &curPos[i]) == ERROR) {
			sprintf(strbuf, fmt1, mtraddr-63);
			printError(ERR_MTR, strbuf);
			return(ERROR);
		}
	}

	// TBD: Check that all motors are at the same position

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

	const char fmt1[] = "put_FRAM_ENCODERS: get_MOTOR_ENCODER error on %c";
	const char fmt2[] = "put_FRAM_ENCODERS: write_FRAM error for %c";
	const char fmt3[] = "put_FRAM_ENCODERS: encoder value read out of range for %c";
	char strbuf[80];
	uint8_t i, tbuf[4], mtraddr[3] = {MOTOR_A, MOTOR_B, MOTOR_C};
	uint16_t memaddr[3] = {ENCA_FRAMADDR, ENCB_FRAMADDR, ENCC_FRAMADDR};
	int32_t encoderValue;

	for (i = 0; i < 1; i++) {
//	for (i = 0; i < 3; i++) {
		if (get_MOTOR_ENCODER(mtraddr[i], &encoderValue) == ERROR) {
			sprintf(strbuf, fmt1, (char) (mtraddr[i]-63));
			printError(ERR_MTR, strbuf);
			return(ERROR);
		}
		if ((encoderValue > PID_MAXPOS) || (encoderValue < PID_MINPOS)) {
			sprintf(strbuf, fmt3, (char) (mtraddr[i]-63));
			printError(ERR_MTR, strbuf);
			return(ERROR);
		}
		tbuf[0] = (encoderValue >> 24) & 0xFF;
		tbuf[1] = (encoderValue >> 16) & 0xFF;
		tbuf[2] = (encoderValue >> 8) & 0xFF;
		tbuf[3] = encoderValue & 0xFF;
		if (write_FRAM(FRAMTWIADDR, memaddr[i], tbuf, 4) == ERROR) {
			sprintf(strbuf, fmt2, (char) (memaddr[i]-63));
			printError(ERR_FRAM, strbuf);
			return(ERROR);
		}
	}
	return(NOERROR);

}

/*------------------------------------------------------------------------------
uint8_t put_MOTOR(uint8_t mtraddr, uint8_t cmd, uint8_t* data, uint8_t nbytes)
	Sends a command to a motor controller. This routine copies data to temporary
	array and adds the two CRC bytes before sending the temporary array to the
	controller.

	Inputs:
		mtraddr:	Motor controller serial address
		cmd:		Motor controller command
		data:		Array containing data to send.
		nbytes:		Number of bytes to send (not including the 2 CRC bytes)

	Outputs:
		Nothing

	Returns:
		ERROR:		If a serial timeout or bad acknowledge is detected
		NOERROR:	Otherwise
------------------------------------------------------------------------------*/
uint8_t put_MOTOR(uint8_t mtraddr, uint8_t cmd, uint8_t* data, uint8_t nbytes)
{

	const char fmt1[] = "put_MOTOR: serial timeout on %c";
	const char fmt2[] = "put_MOTOR: bad ACK on %c, ack=0x%02x";
	char strbuf[80];
	uint8_t i, tbuf[nbytes+4];
	uint16_t crc;

	ser_recv1.nxfrd = 0;				// Set up reply
	ser_recv1.n2xfr = 1;				// Receive only the 0xFF ACK

	tbuf[0] = mtraddr;
	tbuf[1] = cmd;
	for (i = 0; i < nbytes; i++) {
		tbuf[i+2] = data[i];
	}
	crc = crc16(tbuf, nbytes+2);
	tbuf[nbytes+2] = (crc >> 8) & 0xFF;
	tbuf[nbytes+3] = crc & 0xFF;

	send_USART1(tbuf, nbytes+4);		// Send the command

	USART1_ticks = 0;
	while (ser_recv1.nxfrd < ser_recv1.n2xfr) {
		if (USART1_ticks > 100) {
			sprintf(strbuf, fmt1, (char) (mtraddr-63));
			printError(ERR_MTR, strbuf);
			return(ERROR);
		} else {
			asm("nop");
		}
	}

sprintf(strbuf, " put_MOTOR USART1_ticks=%d", USART1_ticks);
printLine(strbuf);

	if (ser_recv1.data[0] != 0xFF) {	// Bad ack
		sprintf(strbuf, fmt2, (char) (mtraddr-63), ser_recv1.data[0]);
		printError(ERR_MTR, strbuf);
		return(ERROR);
	}

	return(NOERROR);
	
}

/*------------------------------------------------------------------------------
uint8_t put_MOTOR_ENCODER(uint8_t mtraddr, int32_t encoderValue)
	Writes a new encoder value to the selected controller.

	Inputs:
		mtraddr:		Motor controller serial address
		encoderValue:	The new encoder value

	Outputs:
		Nothing

	Returns:
		ERROR:		If put_MOTOR returns an error
		NOERROR:	Otherwise
------------------------------------------------------------------------------*/
uint8_t put_MOTOR_ENCODER(uint8_t mtraddr, int32_t encoderValue)
{

	const char fmt1[] = "put_MOTOR_ENCODER: put_MOTOR error on %c";
	char strbuf[80];
	uint8_t data[4];

	data[0] = (encoderValue >> 24) & 0xFF;
	data[1] = (encoderValue >> 16) & 0xFF;
	data[2] = (encoderValue >> 8) & 0xFF;
	data[3] = encoderValue & 0xFF;

	if (put_MOTOR(mtraddr, PUTENCODER, data, 4) == ERROR) {
		sprintf(strbuf, fmt1, (char) (mtraddr-63));
		printError(ERR_MTR, strbuf);
		return(ERROR);
	}

	return(NOERROR);

}

/*------------------------------------------------------------------------------
uint8_t put_MOTOR_MAXCURRENT(uint8_t mtraddr, int32_t maxCurrent)
	Writes the maximum current value to the selected controller. This works even
	though the Basic Micro GUI doesn't reflect the change. Reading the device on
	the GUI also doesn't update the number. Writing the settings from the GUI
	*will* write the values to the controller EPROM.

	Inputs:
		mtraddr:		Motor controller serial address
		maxCurrent:		The maximum current value im mA

	Outputs:
		Nothing

	Returns:
		ERROR:			If put_MOTOR returns an error
		NOERROR:		Otherwise
------------------------------------------------------------------------------*/
uint8_t put_MOTOR_MAXCURRENT(uint8_t mtraddr, int32_t maxCurrent)
{

	const char fmt1[] = "put_MOTOR_MAXCURRENT: put_MOTOR error on %c";
	char strbuf[80];
	uint8_t data[8];

	maxCurrent /= 10;			// convert to 10 mA units
	data[0] = (maxCurrent >> 24) & 0xFF;
	data[1] = (maxCurrent >> 16) & 0xFF;
	data[2] = (maxCurrent >> 8) & 0xFF;
	data[3] = maxCurrent & 0xFF;
	data[4] = data[5] = data[6] = data[7] = 0;
	if (put_MOTOR(mtraddr, PUTMAXCURRENT, data, 8) == ERROR) {
		sprintf(strbuf, fmt1, (char) (mtraddr-63));
		printError(ERR_MTR, strbuf);
		return(ERROR);
	}
	return(NOERROR);
}

/*------------------------------------------------------------------------------
uint8_t put_MOTOR_PID(uint8_t mtraddr, PID pid)
	Writes new PID parameters to the selected controller. Besides P, I, and D,
	the PID structure also contains the maximum integral windup (maxI), deadZone,
	and the minimum and maximum allowable encoder values (minPos and maxPos).

	Inputs:
		mtraddr:	Motor controller serial address
		PID:		The new PID values

	Outputs:
		Nothing

	Returns:
		ERROR:		If put_MOTOR returns an error
		NOERROR:	Otherwise
------------------------------------------------------------------------------*/
uint8_t put_MOTOR_PID(uint8_t mtraddr, PID pid)
{

	const char fmt1[] = "put_MOTOR_PID: put_MOTOR error pid on %c";
	const char fmt2[] = "put_MOTOR_PID: put_MOTOR error qpps on %c";
	char strbuf[80];
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
		sprintf(strbuf, fmt1, (char) (mtraddr-63));
		printError(ERR_MTR, strbuf);
		return(ERROR);
	}

	data[0] = data[1] = data[2] = data[3] = 0;
	data[4] = data[5] = data[6] = data[7] = 0;
	data[8] = data[9] = data[10] = data[11] = 0;
	data[12] = (pid.qpps >> 24) & 0xFF;
	data[13] = (pid.qpps >> 16) & 0xFF;
	data[14] = (pid.qpps >> 8) & 0xFF;
	data[15] = pid.qpps & 0xFF;

	if (put_MOTOR(mtraddr, SETQPPS, data, 16) == ERROR) {
		sprintf(strbuf, fmt2, (char) (mtraddr-63));
		printError(ERR_MTR, strbuf);
		return(ERROR);
	}

	return(NOERROR);
}

/*------------------------------------------------------------------------------
uint8_t put_MOTOR_S4MODE(uint8_t mtraddr)
	Writes the S4 input pin behavior. The S4 pin is connected to both the
	forward and rearward limit switches. We write 0x72, which means the S4 pin
	treats one switch as the home position and the other as a limit.

	Inputs:
		mtraddr:	The controller serial address

	Outputs:
		None

	Returns:
		ERROR if the put_MOTOR routine fails
		NOERROR otherwise
------------------------------------------------------------------------------*/
uint8_t put_MOTOR_S4MODE(uint8_t mtraddr)
{

	const char fmt1[] = "put_MOTOR_S4MODE: put_MOTOR error on %c";
	char strbuf[80];
	uint8_t data[3];

	data[0] = 0x00;
	data[1] = S4MODE;	// 0x72, Home(User)/Limit(Fwd)
	data[2] = 0x00;

	if (put_MOTOR(mtraddr, SETS4MODE, data, 3) == ERROR) {
		sprintf(strbuf, fmt1, (char) (mtraddr-63));
		printError(ERR_MTR, strbuf);
		return(ERROR);
	}

	return(NOERROR);

}

uint8_t stop_MOTOR(uint8_t mtraddr)
{

	char strbuf[80];
	uint8_t tbuf[1];
	const char fmt[] = "stop_MOTOR: put_MOTOR error on %c";

	tbuf[0] = 0;
	if (put_MOTOR(mtraddr, STOP, tbuf, 1) == ERROR) {
		sprintf(strbuf, fmt, (char) (mtraddr-63));
		printError(ERR_MTR, strbuf);
		return(ERROR);
	}
	return(NOERROR);

}