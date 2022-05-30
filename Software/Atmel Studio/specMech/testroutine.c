#include "globals.h"
#include "timers.h"
#include "usart.h"
#include "roboclaw.h"
#include "fram.h"
#include "testroutine.h"
#include "initialize.h"
#include "commands.h"
#include "errors.h"
#include "beeper.h"

void testroutine(void)
{

	char strbuf[80];
//	uint8_t i;

	strcpy(strbuf, "TESTING\r");
	send_USART(3, (uint8_t*) strbuf, strlen(strbuf));



/*
	beeper(".... ..");
	beeper("  --- -.-");
*/

/*
	if (PORTB.IN & PIN3_bm) {
		fan(OFF);
	} else {
		fan(ON);
	}
*/
/*
	float temperature;

	for (i = 0; i < 3; i++) {
		read_AD590(i, &temperature);
		sprintf(strbuf, "sensor=%d, temp=%f", i, temperature);
		printLine(strbuf);
	}
*/
/*
	i = unstick_MOTOR_LIMIT('c');
	sprintf(strbuf, "unstick_MOTOR_LIMIT returned %d", i);
	printLine(strbuf);
*/
/*
	for (i = 0; i < 3; i++) {
		get_MOTOR_LIMITS();
		sprintf(strbuf, "motor %d direction is %d", i, motorDir[i]);
		printLine(strbuf);
		sprintf(strbuf, "motor %d limit state is %d", i, motorLim[i]);
		printLine(strbuf);
	}
*/
/*
	uint32_t robostatus;

	if (get_MOTOR_STATUS(130, &robostatus) == NOERROR) {
		sprintf(strbuf, "status = %08lx", robostatus);
		printLine(strbuf);
		return;
	} else {
		printLine("error");
		return;
	}
*/
/*
//	write_MCP23008(HIGHCURRENT, IODIR, 0x00);
	for (i = 0; i < 5; i++) {
		if (write_MCP23008(HIGHCURRENT, OLAT, 0xFF) == ERROR) {
			sprintf(strbuf, "test routine error");
			printLine(strbuf);
		}
		_delay_ms(1000);
		write_MCP23008(HIGHCURRENT, OLAT, 0x00);
		_delay_ms(1000);
	}
	return;
*/
/*
	char strbuf[80], isotime[20];
	if (get_FRAM_ENCSAVETIME(isotime) == NOERROR) {
		printLine(isotime);
	} else {
		sprintf(strbuf, "get_FRAM_ENCSAVETIME error");
		printLine(strbuf);
	}
	return;
*/

/*
	move_MOTORS_PISTON(28300);
	return;
*/

/*
	char strbuf[80];
	if (motorsMoving()) {
		strcpy(strbuf, "YES");
	} else {
		strcpy(strbuf, "NO");
	}
	printLine(strbuf);
	return;
*/
/*
	set_MOTOR_PARAMS();
*/
/*
	char strbuf[80];
	if (motorsMoving()) {
		sprintf(strbuf, "yes");
		printLine(strbuf);
	} else {
		sprintf(strbuf, "no");
		printLine(strbuf);
	}

	stop_MOTORS();
	return;
*/

/*
	char strbuf[80];
	uint8_t i;
	uint16_t current;
	for (i = 0; i < 50; i++) {
		get_MOTOR_CURRENT(128, &current);
		sprintf(strbuf, "current = %dmA", current);
		printLine(strbuf);
		_delay_ms(100);	
	}
---*/

/*
	char strbuf[80];
	int32_t encoderValue;

	if (get_MOTOR_ENCODER(128, &encoderValue) == ERROR) {
		sprintf(strbuf, " testroutine error get_MOTOR_ENCODER");
		printLine(strbuf);
		return;
	}
	sprintf(strbuf, " testroutine encoder=%ld", encoderValue);
	printLine(strbuf);
	return;
*/

/*
	char strbuf[80];
	int32_t encoderValue, newPosition;

	put_MOTOR_ENCODER(128, 10000UL);
	get_MOTOR_ENCODER(128, &encoderValue);
	sprintf(strbuf, "encoderValue=%lu", encoderValue);
	printLine(strbuf);

	newPosition = encoderValue+101;
	sprintf(strbuf, "newPosition=%lu", newPosition);
	printLine(strbuf);

	move_MOTORAbsolute(128, newPosition);

	sprintf(strbuf, "done");
	printLine(strbuf);
---*/

/*
	char strbuf[80];
	int32_t encoderValue, newPosition;

	set_MOTOREncoder(128, 10000UL);
	get_MOTOR_ENCODER(128, &encoderValue);

	sprintf(strbuf, "encoderValue=%lu", encoderValue);
	printLine(strbuf);

	newPosition = encoderValue+101;

	sprintf(strbuf, "newPosition=%lu", newPosition);
	printLine(strbuf);

	move_MOTORAbsolute(128, newPosition);

	sprintf(strbuf, "done");
	printLine(strbuf);
---*/

/*
	char strbuf[80];
	PID pid;

	pid.p = pid.i = pid.d = 0.0;
	pid.maxI = pid.deadZone = pid.minPos = pid.maxPos = 0;

	get_MOTOR_PID(128, &pid);

	sprintf(strbuf, " p=%10.5f", pid.p);
	printLine(strbuf);

	sprintf(strbuf, " i=%10.5f", pid.i);
	printLine(strbuf);

	sprintf(strbuf, " d=%10.5f", pid.d);
	printLine(strbuf);

	sprintf(strbuf, " maxI=%ld", pid.maxI);
	printLine(strbuf);

	sprintf(strbuf, " deadZone=%ld", pid.deadZone);
	printLine(strbuf);

	sprintf(strbuf, " minPos=%ld", pid.minPos);
	printLine(strbuf);

	sprintf(strbuf, " maxPos=%ld", pid.maxPos);
	printLine(strbuf);

	// Values for the test motors, not PI motors
	pid.p = 65.0;
	pid.i = 1.0;
	pid.d = 350.0;
	pid.maxI = 150;
	pid.deadZone = 0;
	pid.minPos = -100000;
	pid.maxPos = 900000;

	sprintf(strbuf, " setting pid");
	printLine(strbuf);

	put_MOTOR_PID(128, pid);

	pid.p = pid.i = pid.d = 0.0;
	pid.maxI = pid.deadZone = pid.minPos = pid.maxPos = 0;

	get_MOTOR_PID(128, &pid);

	sprintf(strbuf, " getting pid");
	printLine(strbuf);

	sprintf(strbuf, " p=%10.5f", pid.p);
	printLine(strbuf);

	sprintf(strbuf, " i=%10.5f", pid.i);
	printLine(strbuf);

	sprintf(strbuf, " d=%10.5f", pid.d);
	printLine(strbuf);

	sprintf(strbuf, " maxI=%ld", pid.maxI);
	printLine(strbuf);

	sprintf(strbuf, " deadZone=%ld", pid.deadZone);
	printLine(strbuf);

	sprintf(strbuf, " minPos=%ld", pid.minPos);
	printLine(strbuf);

	sprintf(strbuf, " maxPos=%ld", pid.maxPos);
	printLine(strbuf);
---*/

/*
	char strbuf[80], istr[10], jstr[10], kstr[10], fstr[10];
	int i, j, k, nitems;
	float f;
	strcpy(strbuf," 400.5 200 300 400");
	nitems = sscanf(strbuf, "%s%s%s%s", fstr, istr, jstr, kstr);
	i = atoi(istr);
	j = atoi(jstr);
	k = atoi(kstr);
	f = atof(fstr);
	sprintf(strbuf, "nitems=%d i=%d j=%d k=%d f=%f", nitems, i, j, k, f);
	printLine(strbuf);
---*/

/*
	char strbuf[80];
	int32_t maxCurrent;
	put_MOTOR_MAXCURRENT(MOTOR_A, 300);
	get_MOTOR_MAXCURRENT(MOTOR_A, &maxCurrent);
	sprintf(strbuf, "maxcurrent=%ld mA", maxCurrent);
	printLine(strbuf);
---*/

/*
	char strbuf[80];
	uint8_t mode;

	if (get_MOTOR_S4MODE(128, &mode) == ERROR) {
		printError(ERR_MTR, "testroutine: get_MOTOR_S4MODE error");
		return;
	}
	sprintf(strbuf, "S4 mode=0x%02x", mode);
	printLine(strbuf);

	while (put_MOTOR_S4MODE(128) == ERROR) {
		printError(ERR_MTR, "testroutine: put_MOTOR_S4MODE error");
		_delay_ms(12);
//		return;
	}
	if (get_MOTOR_S4MODE(128, &mode) == ERROR) {
		printError(ERR_MTR, "testroutine: get_MOTOR_S4MODE error");
		return;
	}
	sprintf(strbuf, "S4 mode=0x%02x after write", mode);
	printLine(strbuf);
---*/

/*
	char strbuf[80];
	uint8_t i, data[4];
	uint32_t status;

	for (i = 0; i < 100; i++) {
		if (get_MOTOR(128, 90, data, 4) == ERROR) {
			printError(ERR_MTR, "testroutine: get_MOTOR error");
			return;
		}
		status = (((uint32_t)(data[0])) << 24);
		status |= (((uint32_t)(data[1])) << 16);
		status = (((uint32_t)(data[2])) << 8);
		status |= data[3];
		sprintf(strbuf, " status=0x%08lx", status);
		printLine(strbuf);		
		_delay_ms(100);
	}
---*/

/*
	char strbuf[80];
	PID pid;

	if (get_MOTOR_PID(128, &pid) == ERROR) {
		printError(ERR_MTR, "testroutine: get_MOTOR_PID error");
		return;
	}

	sprintf(strbuf, " p=%10.5f", pid.p);
	printLine(strbuf);

	sprintf(strbuf, " i=%10.5f", pid.i);
	printLine(strbuf);

	sprintf(strbuf, " d=%10.5f", pid.d);
	printLine(strbuf);

	sprintf(strbuf, " maxI=%ld", pid.maxI);
	printLine(strbuf);

	sprintf(strbuf, " deadZone=%ld", pid.deadZone);
	printLine(strbuf);

	sprintf(strbuf, " minPos=%ld", pid.minPos);
	printLine(strbuf);

	sprintf(strbuf, " maxPos=%ld", pid.maxPos);
	printLine(strbuf);

	sprintf(strbuf, " qpps=%ld", pid.qpps);
	printLine(strbuf);


	pid.qpps = 23200;
	if (put_MOTOR_PID(128, pid) == ERROR) {
		printError(ERR_MTR, "testroutine: put_MOTOR_PID error");
		return;
	}
	if (get_MOTOR_PID(128, &pid) == ERROR) {
		printError(ERR_MTR, "testroutine: get_MOTOR_PID error");
		return;
	}
	sprintf(strbuf, " qpps=%ld confirmed written", pid.qpps);
	printLine(strbuf);
---*/
/*
	int32_t value;
	get_MOTOR_MAXCURRENT(128, &value);
	get_MOTOR_MAXCURRENT(129, &value);
	get_MOTOR_MAXCURRENT(130, &value);
---*/

//	stop_MOTOR(128);

}