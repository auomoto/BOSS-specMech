#include "globals.h"
#include "report.h"

/*------------------------------------------------------------------------------
uint8_t report(char *ptr)
	Report status, including reading sensors

	Input:
		*ptr - Command line pointer. Incremented to find object to report

	Output:
		Prints NMEA formatted output to the serial port.

	Returns:
		ERROR or NOERROR
------------------------------------------------------------------------------*/
uint8_t report(uint8_t cstack)
{

	char outbuf[BUFSIZE], version[11];
	char currenttime[20], lastsettime[20], boottime[20], encsavetime[20];
	char shutter, left, right, air, mlimit, mdir;
	const char format_ENV[] = "ENV,%s,%3.1f,C,%1.0f,%%,%3.1f,C,%1.0f,%%,%3.1f,C,%1.0f,%%,%3.1f,C,%s";
	const char format_LN2[] = "LN2,%s,%s";
	const char format_MTR[] = "MTR,%s,%c,%ld,um,%ld,um/s,%d,mA,%c,dir,%c,lim,%s";
	const char format_MT0[] = "MET,%s,%c,%3.1f,V,%3.1f,C,%s,encSaveTime,%s";	// Motor, Voltage, Save-time
	const char format_MT1[] = "PID,%s,%c,%.2f,P,%.3f,I,%.2f,D,%ld,maxInt,%s";
	const char format_MT2[] = "DMM,%s,%c,%ld,dead,%ld,minP,%ld,maxP,%ld,qpps,%s";	// Dead, min, max, qpps
	const char format_MT3[] = "MIL,%s,%c,%ld,mA,0x%02x,S4,%s";	// Motor, current, limit function
	const char format_ORI[] = "ORI,%s,%3.1f,%3.1f,%3.1f,%s";	// Orientation
	const char dformat_ORI[] = "%2.0f %2.0f %2.0f";				// For display
	const char format_PNU[] = "PNU,%s,%c,shutter,%c,left,%c,right,%c,air,%s";	// Pneumatics
	const char dformat_PN1[] = "Left:%c   Right:%c";
	const char dformat_PN2[] = "Shutter:%c  Air:%c";
	const char format_SPM[] = "SPM,%s,%d,fan,%5.2f,volts";
	const char format_TIM[] = "TIM,%s,%s,set,%s,boot,%s";	// Time
	const char format_VAC[] = "VAC,%s,%5.2f,redvac,%5.2f,bluevac,%s";	// Vacuum
	const char dformat_VAC[] = "%2.2f  %2.2f";
	const char format_VER[] = "VER,%s,%s,%s";	// Version
	uint8_t i, controller, fanstate, s4mode, ln2status[80];
	int32_t encoderValue;
	int32_t encoderSpeed;
	int32_t micronValue, micronSpeed;
	int32_t maxCurrent;
	float t0, t1, t2, t3, h0, h1, h2;		// temperature and humidity
	float voltage;							// voltage
	uint16_t motorCurrent;
	float x, y, z;							// accelerometer
	float redvac, bluvac;					// red and blue vacuum
	PID pid;

	switch(pcmd[cstack].cobject) {

		case 'A':
		case 'B':
		case 'C':
			get_time(currenttime);
			controller = pcmd[cstack].cobject + 63;

			if (get_MOTOR_FLOAT(controller, READMAINVOLTAGE, &voltage) == ERROR) {
				printError(ERR_MTR, "report: get_MOTOR_FLOAT volts error");
				return(ERROR);
			}

			if (get_FRAM_ENCSAVETIME(encsavetime) == ERROR) {
				sprintf(outbuf, "report: get_FRAM_ENCSAVEIME error");
				printError(ERR_MTR, outbuf);
				return(ERROR);
			}

			if (get_MOTOR_FLOAT(controller, READTEMPERATURE, &t0) == ERROR) {
				printError(ERR_MTR, "report: get_MOTOR_FLOAT temperature error");
				return(ERROR);
			}

			if (get_MOTOR_MAXCURRENT(controller, &maxCurrent) == ERROR) {
				printError(ERR_MTR, "report: get_MOTOR_MAXCURRENT error");
				return(ERROR);
			}

			if (get_MOTOR_S4MODE(controller, &s4mode) == ERROR) {
				printError(ERR_MTR, "report: get_MOTOR_S4MODE error");
				return(ERROR);
			}

			if (get_MOTOR_PID(controller, &pid) == ERROR) {
				printError(ERR_MTR, "report: get_MOTOR_PID error");
				return(ERROR);
			}

			sprintf(outbuf, format_MT0, currenttime, (char) (controller-63),
			voltage, t0, encsavetime, pcmd[cstack].cid);
			printLine(outbuf);

			sprintf(outbuf, format_MT3, currenttime, (char) (controller-63),
			maxCurrent, s4mode, pcmd[cstack].cid);
			printLine(outbuf);

			sprintf(outbuf, format_MT1, currenttime, (char) (controller-63),
			pid.p, pid.i, pid.d, pid.maxI, pcmd[cstack].cid);
			printLine(outbuf);

			sprintf(outbuf, format_MT2, currenttime, (char) (controller-63),
				pid.deadZone, pid.minPos, pid.maxPos, pid.qpps, pcmd[cstack].cid);
			printLine(outbuf);

			break;

		case 'a':
		case 'b':
		case 'c':
			get_time(currenttime);
			controller = pcmd[cstack].cobject + 31;

			if (get_MOTOR_ENCODER(controller, &encoderValue) == ERROR) {
				printError(ERR_MTR, "report: get_MOTOR_ENCODER error");
				return(ERROR);
			}

			micronValue = encoderValue/ENC_COUNTS_PER_MICRON;

			if (get_MOTOR_SPEED(controller, &encoderSpeed) == ERROR) {
				encoderSpeed = 0x7FFFFFFF;
			}

			micronSpeed = encoderSpeed/ENC_COUNTS_PER_MICRON;

			if (get_MOTOR_CURRENT(controller, &motorCurrent) == ERROR) {
				printError(ERR_MTR, "report: get_MOTOR_CURRENT error");
				motorCurrent = 0xFFFF;
			}

			mdir = '?';
			if (motorDir[controller - MOTOR_A] == MTRDIRPOSITIVE) {
				mdir = 'F';
			} else if (motorDir[controller - MOTOR_A] == MTRDIRNEGATIVE) {
				mdir = 'R';
			}

			mlimit = '?';
			if (get_MOTOR_LIMITS() == ERROR) {
				printError(ERR_MTR, "report: get_MOTOR_LIMITS error");
			} else {
				if (motorLim[controller - MOTOR_A] == YES) {
					mlimit = 'Y';
				} else if (motorLim[controller - MOTOR_A] == MTRLIMUNKNOWN) {
					mlimit = '?';
				}
			}
			sprintf(outbuf, format_MTR, currenttime, pcmd[cstack].cobject,
				micronValue, micronSpeed, motorCurrent, mdir, mlimit, pcmd[cstack].cid);
			printLine(outbuf);
			break;

		case 'd':			// copy of the above code
			for (i = 0; i < NMOTORS; i++) {
				get_time(currenttime);
				controller = i + MOTOR_A;

				if (get_MOTOR_ENCODER(controller, &encoderValue) == ERROR) {
					printError(ERR_MTR, "report: get_MOTOR_ENCODER error");
					return(ERROR);
				}

				micronValue = encoderValue/ENC_COUNTS_PER_MICRON;

				if (get_MOTOR_SPEED(controller, &encoderSpeed) == ERROR) {
					encoderSpeed = 0x7FFFFFFF;
				}

				micronSpeed = encoderSpeed/ENC_COUNTS_PER_MICRON;

				if (get_MOTOR_CURRENT(controller, &motorCurrent) == ERROR) {
					printError(ERR_MTR, "report: get_MOTOR_CURRENT error");
					motorCurrent = 0xFFFF;
				}

				mdir = '?';
				if (motorDir[controller - MOTOR_A] == MTRDIRPOSITIVE) {
						mdir = 'F';
				} else if (motorDir[controller - MOTOR_A] == MTRDIRNEGATIVE) {
						mdir = 'R';
				}

				mlimit = '?';
				if (get_MOTOR_LIMITS() == ERROR) {
						printError(ERR_MTR, "report: get_MOTOR_LIMITS error");
				}
				if (motorLim[controller - MOTOR_A] == YES) {
						mlimit = 'Y';
				}

				sprintf(outbuf, format_MTR, currenttime, controller-31, micronValue,
					micronSpeed, motorCurrent, mdir, mlimit, pcmd[cstack].cid);
				printLine(outbuf);
			}
			break;

		case 'e':					// Environment (temperature & humidity)
			t0 = get_temperature(0);
			h0 = get_humidity(0, t0);
			t1 = get_temperature(1);
			h1 = get_humidity(1, t1);
			t2 = get_temperature(2);
			h2 = get_humidity(2, t2);
			t3 = get_temperature(3);
			get_time(currenttime);
			sprintf(outbuf, format_ENV, currenttime, t0, h0, t1, h1, t2, h2, t3, pcmd[cstack].cid);
			printLine(outbuf);
			writestr_OLED(1, "Temp & Humidity", 1);
			sprintf(outbuf, "%1.1fC %1.0fF %1.0f%%", t0, ((t0*1.8)+32), h0);
			writestr_OLED(1, outbuf, 2);
			break;

		case 'n':					// LN2 controller status
			if (get_ln2(ln2status) != ERROR) {
				get_time(currenttime);
				sprintf(outbuf, format_LN2, currenttime, ln2status);
				printLine(outbuf);
			}
			break;

		case 'o':					// Orientation
			get_orientation(&x, &y, &z);
			get_time(currenttime);
			sprintf(outbuf, format_ORI, currenttime, x, y, z, pcmd[cstack].cid);
			printLine(outbuf);
			writestr_OLED(1, "Orientation", 1);
			sprintf(outbuf, dformat_ORI, x, y, z);
			writestr_OLED(1, outbuf, 2);
			break;

		case 'p':					// Pneumatics
			get_time(currenttime);
			read_PNEUSensors(&shutter, &left, &right, &air);
			sprintf(outbuf, format_PNU, currenttime, shutter, left, right, air, pcmd[cstack].cid);
			printLine(outbuf);
			sprintf(outbuf, dformat_PN1, left, right);
			writestr_OLED(1, outbuf, 1);
			sprintf(outbuf, dformat_PN2, shutter, air);
			writestr_OLED(1, outbuf, 2);
			break;

		case 's':					// specMech voltage and fan state
			get_time(currenttime);
			fanstate = (PORTB.IN & PIN3_bm) ? 1 : 0;
			voltage = read_PSVOLTAGE();
			sprintf(outbuf, format_SPM, currenttime, fanstate, voltage);
			printLine(outbuf);
			break;

		case 't':					// Report current time on specMech clock
			get_time(currenttime);
			get_SETTIME(lastsettime);
			get_BOOTTIME(boottime);
			sprintf(outbuf, format_TIM, currenttime, lastsettime,
				boottime, pcmd[cstack].cid);
			printLine(outbuf);
			writestr_OLED(1, "Time", 1);
			writestr_OLED(1, &currenttime[11], 2);			
			break;

		case 'v':					// Vacuum
			redvac = read_ionpump(REDPUMP);
			bluvac = read_ionpump(BLUEPUMP);
			get_time(currenttime);
			sprintf(outbuf, format_VAC, currenttime, redvac, bluvac, pcmd[cstack].cid);
			printLine(outbuf);
			writestr_OLED(1, "RedVac  BlueVac", 1);
			sprintf(outbuf, dformat_VAC, redvac, bluvac);
			writestr_OLED(1, outbuf, 2);
			break;

		case 'V':					// Version
			get_VERSION(version);
			get_time(currenttime);
			sprintf(outbuf, format_VER, currenttime, version, pcmd[cstack].cid);
			printLine(outbuf);
			writestr_OLED(1, "specMech Version", 1);
			get_VERSION(outbuf);
			writestr_OLED(1, outbuf, 2);
			break;

		default:
			printError(ERR_BADOBJECT, "report: unknown object");
			return(ERROR);
			break;
	}

	return(NOERROR);

}
