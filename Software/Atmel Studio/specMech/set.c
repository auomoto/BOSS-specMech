#include "globals.h"
#include "set.h"

/*------------------------------------------------------------------------------
uint8_t set (char *ptr)

	Input:
		cstack - The array index in the ParsedCMD array (pcmd). The command
			must have been parsed already

	Returns:
		ERROR if an invalid object was requested.
		NOERROR otherwise
------------------------------------------------------------------------------*/
uint8_t set(uint8_t cstack)
{

	char strbuf[80];
	char object;
	uint8_t i, mtraddr;
//	int32_t newEncoderValue;
	PID motorPID;
//	const char fmt0[] = "set: put_MOTOR_ENCODER error on %c";
	const char fmt1[] = "set: bad time format";
	const char fmt2[] = "set: put_time error";
	const char fmt3[] = "set: write_FRAM error";
	const char fmt4[] = "set: what?";
//	const char fmt5[] = "set: encoder value out of range %ld";
	const char fmt6[] = "set: get_MOTOR_PID error on %c";
	const char fmt7[] = "set: put_MOTOR_PID error on %c";

	object = pcmd[cstack].cobject;
	switch(object) {
/*
		case 'a':
		case 'b':
		case 'c':
			if (pcmd[cstack].cvalue[0] == '\0') {	// Don't do anything on null distance
				return(NOERROR);
			}
			newEncoderValue = atol(pcmd[cstack].cvalue) * ENC_COUNTS_PER_MICRON;
			if (newEncoderValue > PID_MAXPOS || newEncoderValue < PID_MINPOS) {
				sprintf(strbuf, fmt5, newEncoderValue/ENC_COUNTS_PER_MICRON);
				printError(ERR_MTR, strbuf);
				return(ERROR);
			}
			mtraddr = object + 31;
			if (put_MOTOR_ENCODER(mtraddr, newEncoderValue) == ERROR) {
				sprintf(strbuf, fmt0, object);
				printError(ERR_MTR, strbuf);
				return(ERROR);
			}
			break;
*/

		case 't':		// Time
			if (strlen(pcmd[cstack].cvalue) != 19) {
				sprintf(strbuf, fmt1);
				printError(ERR_SETTIME, strbuf);
				return(ERROR);
			}
			if (put_time(pcmd[cstack].cvalue) == ERROR) {
				sprintf(strbuf, fmt2);
				printError(ERR_ISO, strbuf);
				return(ERROR);
			}
			if (write_FRAM(FRAMTWIADDR, SETTIMEADDR, (uint8_t*) pcmd[cstack].cvalue, 20) == ERROR) {
				sprintf(strbuf, fmt3);
				printError(ERR_ISO, strbuf);
				return(ERROR);
			}
			break;

		case 'u':		// Unsafe motor encoder limits
		case 'L':
			for (i = 0; i < NMOTORS; i++) {
				mtraddr = i + MOTOR_A;
				if (get_MOTOR_PID(mtraddr, &motorPID) == ERROR) {
					sprintf(strbuf, fmt6, mtraddr - 31);
					printError(ERR_MTR, strbuf);
					return(ERROR);
				}
				motorPID.minPos = PID_MINPOSUNSAFE;
				motorPID.maxPos = PID_MAXPOSUNSAFE;
				if (put_MOTOR_PID(mtraddr, motorPID) == ERROR) {
					sprintf(strbuf, fmt7, mtraddr - 31);
					printError(ERR_MTR, strbuf);
					return(ERROR);
				}
			}
			break;

		case 's':		// Safe motor encoder limits
		case 'l':
			for (i = 0; i < NMOTORS; i++) {
				mtraddr = i + MOTOR_A;
				if (get_MOTOR_PID(mtraddr, &motorPID) == ERROR) {
					sprintf(strbuf, fmt6, mtraddr - 31);
					printError(ERR_MTR, strbuf);
					return(ERROR);
				}
				motorPID.minPos = PID_MINPOS;
				motorPID.maxPos = PID_MAXPOS;
				if (put_MOTOR_PID(mtraddr, motorPID) == ERROR) {
					sprintf(strbuf, fmt7, mtraddr - 31);
					printError(ERR_MTR, strbuf);
					return(ERROR);
				}
			}
			break;

		default:
			sprintf(strbuf, fmt4);
			printError(ERR_SET, strbuf);
			return(ERROR);
	}

	return(NOERROR);

}
