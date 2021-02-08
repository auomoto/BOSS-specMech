#include "globals.h"

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
	const char format_BTM[] = "$S%dBTM,%s,%s";
	const char format_ENV[] = "$S%dENV,%s,%3.1fC,%1.0f%%,%3.1fC,%1.0f%%,%3.1fC,%1.0f%%,%3.1fC,%s";
	const char format_ORI[] = "$S%dORI,%s,%3.1f,%3.1f,%3.1f,%s";
	const char format_PNU[] = "$S%dPNU,%s,%c,shutter,%c,left,%c,right,%c,air,%s";
	const char format_TIM[] = "$S%dTIM,%s,%s,set,%s,boot,%s";
	const char format_VAC[] = "$S%dVAC,%s,%5.2f,redvac,%5.2f,bluevac,%s";
	const char format_VER[] = "$S%dVER,%s,%s,%s";
	float t0, t1, t2, t3, h0, h1, h2;		// temperature and humidity
	float x, y, z;							// accelerometer
	float redvac, bluvac;					// red and blue vacuum
	char shutter, left, right, air;

	switch(pcmd[cstack].cobject) {

		case 'B':					// Boot time
			get_BOOTTIME(boottime);
			sprintf(outbuf, format_BTM, get_SPECID, boottime, pcmd[cstack].cid);
			checksum_NMEA(outbuf);
			send_USART(0, (uint8_t*) outbuf, strlen(outbuf));
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
			sprintf(outbuf, format_ENV, get_SPECID, currenttime, t0, h0, t1, h1, t2, h2, t3, pcmd[cstack].cid);
			checksum_NMEA(outbuf);
			send_USART(0, (uint8_t*) outbuf, strlen(outbuf));
			break;

		case 'o':					// Orientation
			get_orientation(MMA8451ADDR, &x, &y, &z);
			get_time(currenttime);
x=y=z=23.4;
			sprintf(outbuf, format_ORI, get_SPECID, currenttime, x, y, z, pcmd[cstack].cid);
			checksum_NMEA(outbuf);
			send_USART(0, (uint8_t*) outbuf, strlen(outbuf));
			break;

		case 'p':
			get_time(currenttime);
			read_PNEUSENSORS(&shutter, &left, &right, &air);
			sprintf(outbuf, format_PNU, get_SPECID, currenttime, shutter, left, right, air, pcmd[cstack].cid);
			checksum_NMEA(outbuf);
			send_USART(0, (uint8_t*) outbuf, strlen(outbuf));
			break;

		case 't':					// Report current time on specMech clock
			get_time(currenttime);
			read_FRAM(FRAMADDR, SETTIMEADDR, (uint8_t*) lastsettime, 20);
			get_BOOTTIME(boottime);
			sprintf(outbuf, format_TIM, get_SPECID, currenttime, lastsettime,
				boottime, pcmd[cstack].cid);
			checksum_NMEA(outbuf);
			send_USART(0, (uint8_t*) outbuf, strlen(outbuf));
			break;

		case 'v':
			redvac = read_ionpump(REDPUMP);
			bluvac = read_ionpump(BLUEPUMP);
			get_time(currenttime);
			sprintf(outbuf, format_VAC, get_SPECID, currenttime, redvac, bluvac, pcmd[cstack].cid);
			checksum_NMEA(outbuf);
			send_USART(0, (uint8_t*) outbuf, strlen(outbuf));
			break;

		case 'V':
			get_VERSION(version);	// Send the specMech version
			get_time(currenttime);
			sprintf(outbuf, format_VER, get_SPECID, currenttime, version, pcmd[cstack].cid);
			checksum_NMEA(outbuf);
			send_USART(0, (uint8_t*) outbuf, strlen(outbuf));
			break;

		default:
			return(ERRORPROMPT);
			break;
	}

	return(GREATERPROMPT);

}
