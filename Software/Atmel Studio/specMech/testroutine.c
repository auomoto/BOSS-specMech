#include "globals.h"
#include "timers.h"
#include "usart.h"
#include "roboclaw.h"
#include "fram.h"
#include "testroutine.h"

#include "commands.h"
#include "errors.h"

void testroutine(void)
{
/*
	char strbuf[80];
	int32_t encoderValue;

	if (get_MOTOR_ENCODER(128, &encoderValue) == ERROR) {
		sprintf(strbuf, "testroutine error");
		printLine(strbuf);
		return;
	}
	sprintf(strbuf, "testroutine encoder value = %ld", encoderValue);
	printLine(strbuf);
*/

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
*/
/*
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
	pid.p = 100.0;
	pid.i = 1.4;
	pid.d = 430.0;
	pid.maxI = 217;
	pid.deadZone = 0;
	pid.minPos = -1000000000;
	pid.maxPos = 1000000000;

	sprintf(strbuf, " setting pid");
	printLine(strbuf);

	set_MOTOR_PID(128, pid);

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
*/
	return;
}