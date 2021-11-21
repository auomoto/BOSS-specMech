#include "globals.h"
#include "wdt.h"
#include "usart.h"
#include "set.h"
//#include "specID.h"
#include "commands.h"
#include "ds3231.h"
#include "temperature.h"
#include "humidity.h"
#include "roboclaw.h"
#include "oled.h"
#include "mma8451.h"
#include "pneu.h"
#include "fram.h"
#include "ionpump.h"
#include "nmea.h"
#include "eeprom.h"
#include "errors.h"
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
	char currenttime[20], lastsettime[20], boottime[20];
	char shutter, left, right, air;
	const char format_ENV[] = "ENV,%s,%3.1f,C,%1.0f,%%,%3.1f,C,%1.0f,%%,%3.1f,C,%1.0f,%%,%3.1f,C,%s";
	const char format_MTR[] = "MTR,%s,%c,%ld,microns,%ld,microns/sec,%d,mA,%s";
	const char format_MTV[] = "MTV,%s,%c,%3.1f,V,%3.1f,C,%s";
	const char format_ORI[] = "ORI,%s,%3.1f,%3.1f,%3.1f,%s";
	const char dformat_ORI[] = "%2.0f %2.0f %2.0f";
	const char format_PNU[] = "PNU,%s,%c,shutter,%c,left,%c,right,%c,air,%s";
	const char dformat_PN1[] = "Left:%c   Right:%c";
	const char dformat_PN2[] = "Shutter:%c  Air:%c";
	const char format_TIM[] = "TIM,%s,%s,set,%s,boot,%s";
	const char format_VAC[] = "VAC,%s,%5.2f,redvac,%5.2f,bluevac,%s";
	const char dformat_VAC[] = "%2.2f  %2.2f";
	const char format_VER[] = "VER,%s,%s,%s";
	uint8_t retval, controller;
	int32_t encoderValue, encoderSpeed, micronValue, micronSpeed;
	uint32_t icurrents;
	float t0, t1, t2, t3, h0, h1, h2;		// temperature and humidity
	float voltage;							// voltage
	uint16_t current;
	float x, y, z;							// accelerometer
	float redvac, bluvac;					// red and blue vacuum

	switch(pcmd[cstack].cobject) {

		case 'A':
		case 'B':
		case 'C':
			get_time(currenttime);
			controller = pcmd[cstack].cobject + 63;
			retval = get_MOTORFloat(controller, READMAINVOLTAGE, &voltage);
			if (retval == ERROR) {
				voltage = -666.0;
			}
			retval = get_MOTORFloat(controller, READTEMPERATURE, &t0);
			if (retval == ERROR) {
				t0 = -666.0;
			}
			sprintf(outbuf, format_MTV, currenttime, (char) (controller-63),
				voltage, t0, pcmd[cstack].cid);
			printLine(outbuf);
			break;

		case 'a':
		case 'b':
		case 'c':
			get_time(currenttime);
			controller = pcmd[cstack].cobject + 31;
			retval = get_MOTOREncoder(controller, ENCODERCOUNT, &encoderValue);
			if (retval == ERROR) {
				encoderValue = 0x7FFFFFFF;
			}
			micronValue = encoderValue/ROBOCOUNTSPERMICRON;
			retval = get_MOTOREncoder(controller, ENCODERSPEED, &encoderSpeed);
			if (retval == ERROR) {
				encoderSpeed = 0x7FFFFFFF;
			}
			micronSpeed = encoderSpeed/ROBOCOUNTSPERMICRON;
			retval = get_MOTORInt32(controller, ROBOREADCURRENT, &icurrents);
			if (retval == ERROR) {
				icurrents = 0x7FFFFFFF;
			}
			current = (uint16_t) ((icurrents >> 16) * 10);	// convert to mA
			sprintf(outbuf, format_MTR, currenttime, pcmd[cstack].cobject,
				micronValue, micronSpeed, current, pcmd[cstack].cid);
			printLine(outbuf);
			break;

		case 'e':					// Environment (temperature & humidity)
			t0 = get_temperature(0);
			h0 = get_humidity(0);
			t1 = get_temperature(1);
			h1 = get_humidity(1);
			t2 = get_temperature(2);
			h2 = get_humidity(2);
			t3 = get_temperature(3);
			get_time(currenttime);
			sprintf(outbuf, format_ENV, currenttime, t0, h0, t1, h1, t2, h2, t3, pcmd[cstack].cid);
			printLine(outbuf);
			writestr_OLED(1, "Temp & Humidity", 1);
			sprintf(outbuf, "%1.1fC %1.0fF %1.0f%%", t0, ((t0*1.8)+32), h0);
			writestr_OLED(1, outbuf, 2);
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

		case 't':					// Report current time on specMech clock
			get_time(currenttime);
			get_SETTIME(lastsettime);
//			read_FRAM(FRAMTWIADDR, SETTIMEADDR, (uint8_t*) lastsettime, 20);
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
