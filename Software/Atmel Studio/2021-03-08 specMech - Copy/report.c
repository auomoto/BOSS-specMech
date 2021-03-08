#include "globals.h"
#include "wdt.h"
#include "usart.h"
#include "set.h"
#include "specID.h"
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

	Input
		*ptr - Command line pointer. Incremented to find object to report

	Output
		Prints NMEA formatted output to the serial port.

	Returns
		0 - GREATERPROMPT on success
		1 - ERRORPROMPT on error (invalid command noun)
------------------------------------------------------------------------------*/
uint8_t report(uint8_t cstack)
{

	char outbuf[BUFSIZE+10], version[11];
	char currenttime[20], lastsettime[20], boottime[20];
	char shutter, left, right, air;
	const char format_ENV[] = "$S%dENV,%s,%3.1fC,%1.0f%%,%3.1fC,%1.0f%%,%3.1fC,%1.0f%%,%3.1fC,%s";
	const char format_MTR[] = "$S%dMTR,%s,%c,%ld,microns,%ld,microns/sec,%d,mA,%s";
	const char format_ORI[] = "$S%dORI,%s,%3.1f,%3.1f,%3.1f,%s";
	const char dformat_ORI[] = "%2.0f %2.0f %2.0f";
	const char format_PNU[] = "$S%dPNU,%s,%c,shutter,%c,left,%c,right,%c,air,%s";
	const char dformat_PN1[] = "left:%c   right:%c";
	const char dformat_PN2[] = "shutter:%c  air:%c";
	const char format_TIM[] = "$S%dTIM,%s,%s,set,%s,boot,%s";
	const char format_VAC[] = "$S%dVAC,%s,%5.2f,redvac,%5.2f,bluevac,%s";
	const char dformat_VAC[] = "%2.2f  %2.2f";
	const char format_VER[] = "$S%dVER,%s,%s,%s";
	uint8_t status, controller;
	int32_t encoderValue, encoderSpeed, micronValue, micronSpeed;
	uint32_t icurrents;
	float t0, t1, t2, t3, h0, h1, h2;		// temperature and humidity
//	float voltage;							// voltage
	uint16_t current;
	float x, y, z;							// accelerometer
	float redvac, bluvac;					// red and blue vacuum

	switch(pcmd[cstack].cobject) {
/*
		case 'A':
			voltage = get_ROBOFloat(MOTORAADDR, ROBOREADMAINVOLTAGE);
			t0 = get_ROBOFloat(MOTORAADDR, ROBOREADTEMPERATURE);
			get_time(currenttime);
			sprintf(outbuf, format_MTR, get_SPECID, currenttime, (char) (MOTORAADDR-63),
				voltage, t0, pcmd[cstack].cid);
			checksum_NMEA(outbuf);
			send_USART(0, (uint8_t*) outbuf, strlen(outbuf));
			break;
*/
		case 'a':
		case 'b':
		case 'c':
			get_time(currenttime);
			controller = pcmd[cstack].cobject + 31;
			status = get_ROBOEncoder(controller, ROBOREADENCODERCOUNT, &encoderValue);
			micronValue = encoderValue/ROBOCOUNTSPERMICRON;
			status = get_ROBOEncoder(controller, ROBOREADENCODERSPEED, &encoderSpeed);
			micronSpeed = encoderSpeed/ROBOCOUNTSPERMICRON;
			icurrents = get_ROBOInt32(controller, ROBOREADCURRENT);
			current = (uint16_t) ((icurrents >> 16) * 10);	// convert to mA
			sprintf(outbuf, format_MTR, get_SPECID, currenttime, pcmd[cstack].cobject,
				micronValue, micronSpeed, current, pcmd[cstack].cid);
			checksum_NMEA(outbuf);
			send_USART(0, (uint8_t*) outbuf, strlen(outbuf));
			break;

		case 'B':
			break;

		case 'C':
			break;
/*
		case 'B':					// Boot time
			get_BOOTTIME(boottime);
			sprintf(outbuf, get_SPECID, boottime, pcmd[cstack].cid);
			checksum_NMEA(outbuf);
			send_USART(0, (uint8_t*) outbuf, strlen(outbuf));
			writestr_OLED(1, "Boot time", 1);
			writestr_OLED(1, &boottime[5], 2);
			break;
*/
		case 'e':					// Environment (temperature & humidity)
			t0 = get_temperature(0);
			h0 = get_humidity(0);
			t1 = get_temperature(1);
			h1 = get_humidity(1);
			t2 = get_temperature(2);
			h2 = get_humidity(2);
			t3 = get_temperature(3);
			get_time(currenttime);
			sprintf(outbuf, format_ENV, get_SPECID, currenttime, t0, h0, t1, h1, t2, h2, t3, pcmd[cstack].cid);
			checksum_NMEA(outbuf);
			send_USART(0, (uint8_t*) outbuf, strlen(outbuf));
			writestr_OLED(1, "Temp & Humidity", 1);
			sprintf(outbuf, "%1.1fC %1.0fF %1.0f%%", t0, ((t0*1.8)+32), h0);
			writestr_OLED(1, outbuf, 2);
			break;

		case 'o':					// Orientation
			get_orientation(&x, &y, &z);
			get_time(currenttime);
			sprintf(outbuf, format_ORI, get_SPECID, currenttime, x, y, z, pcmd[cstack].cid);
			checksum_NMEA(outbuf);
			send_USART(0, (uint8_t*) outbuf, strlen(outbuf));
			writestr_OLED(1, "Orientation", 1);
			sprintf(outbuf, dformat_ORI, x, y, z);
			writestr_OLED(1, outbuf, 2);
			break;

		case 'p':
			get_time(currenttime);
			read_PNEUSENSORS(&shutter, &left, &right, &air);
			sprintf(outbuf, format_PNU, get_SPECID, currenttime, shutter, left, right, air, pcmd[cstack].cid);
			checksum_NMEA(outbuf);
			send_USART(0, (uint8_t*) outbuf, strlen(outbuf));
			sprintf(outbuf, dformat_PN1, left, right);
			writestr_OLED(1, outbuf, 1);
			sprintf(outbuf, dformat_PN2, shutter, air);
			writestr_OLED(1, outbuf, 2);
			break;

		case 't':					// Report current time on specMech clock
			get_time(currenttime);
			read_FRAM(FRAMADDR, SETTIMEADDR, (uint8_t*) lastsettime, 20);
			get_BOOTTIME(boottime);
			sprintf(outbuf, format_TIM, get_SPECID, currenttime, lastsettime,
				boottime, pcmd[cstack].cid);
			checksum_NMEA(outbuf);
			send_USART(0, (uint8_t*) outbuf, strlen(outbuf));
			writestr_OLED(1, "Time", 1);
			writestr_OLED(1, &currenttime[11], 2);			
			break;

		case 'v':
			redvac = read_ionpump(REDPUMP);
			bluvac = read_ionpump(BLUEPUMP);
			if (get_time(currenttime)) {
				printError(ERR_GETTIME, "DS3231");
			}
			sprintf(outbuf, format_VAC, get_SPECID, currenttime, redvac, bluvac, pcmd[cstack].cid);
			checksum_NMEA(outbuf);
			send_USART(0, (uint8_t*) outbuf, strlen(outbuf));
			writestr_OLED(1, "RedVac  BlueVac", 1);
			sprintf(outbuf, dformat_VAC, redvac, bluvac);
			writestr_OLED(1, outbuf, 2);
			break;

		case 'V':
			get_VERSION(version);	// Send the specMech version
			get_time(currenttime);
			sprintf(outbuf, format_VER, get_SPECID, currenttime, version, pcmd[cstack].cid);
			checksum_NMEA(outbuf);
			send_USART(0, (uint8_t*) outbuf, strlen(outbuf));
			writestr_OLED(1, "specMech Version", 1);
			get_VERSION(outbuf);
			writestr_OLED(1, outbuf, 2);
			break;

		default:
			return(GREATERPROMPT);
			break;
	}

	return(GREATERPROMPT);

}
