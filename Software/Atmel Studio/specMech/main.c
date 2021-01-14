/*------------------------------------------------------------------------------
specMech.c
	BOSS motion controller board based on an ATMega4809 implemented on a
	Microchip Curiosity Nano
------------------------------------------------------------------------------*/
#define F_CPU		3333333UL
#define VERSION		"2021-01-02"
#define	YES				1
#define	NO				0
#define GREATERPROMPT	0	// Standard return prompt >
#define EXCLAIMPROMPT	1	// No REBOOT ACK prompt !
#define ERRORPROMPT		2	// Generate error line, then >

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>		// sprintf
#include <string.h>		// for strcpy, strlen

#include "ports.c"			// ATMega4809 ports
#include "led.c"			// On-board LED
#include "specid.c"			// Spectrograph ID jumper
#include "twi.c"			// I2C
#include "mcp23008.c"		// MCP23008 port expander
#include "pneu.c"			// Pneumatic valves and sensors
#include "ads1115.c"		// ADS1115 ADC
#include "ad590.c"			// AD590 temperature sensors
#include "mcp9808.c"		// MCP9808 sensor
#include "temperature.c"	// AD590 and MCP9808 sensors
#include "humidity.c"		// Honeywell HiH-4031
#include "ionpump.c"		// Read the ion pump vacuum
#include "usart.c"			// Serial (RS232) communications
#include "fram.c"			// FRAM memory
#include "ds3231.c"			// Day-time clock
#include "nmea.c"			// GPS style output
#include "oled.c"			// Newhaven OLED display
#include "eeprom.c"			// ATMega4809 eeprom
#include "wdt.c"			// Watchdog timer used only for reboot function

// Function Prototypes
void commands(void);
uint8_t report(char*);
void send_prompt(uint8_t);
uint8_t set(char*);

extern USARTBuf		// These are declared in usart.c
	send0_buf, send1_buf, send3_buf,
	recv0_buf, recv1_buf, recv3_buf;

int main(void)
{

	init_PORTS();
	init_LED();
	init_SPECID();
	init_TWI();
	init_PNEU();
	init_USART();
	init_OLED(0);
	init_EEPROM();
	sei();

	for (;;) {
		if (recv0_buf.done) {
			recv0_buf.done = NO;
			commands();
		}
	}

}

void commands(void)
{

	uint8_t i, prompt_flag, framdata[10], framtest[10];
	char cmdline[BUFSIZE];
	char *ptr, strbuf[40];
	uint16_t memaddress;
	static uint8_t rebootnack = 1;

	// Copy the command to cmdline[]
	for (i = 0; i < BUFSIZE; i++) {
		cmdline[i] = recv0_buf.data[recv0_buf.tail];
		recv0_buf.tail = (recv0_buf.tail + 1) % BUFSIZE;
		if (cmdline[i] == '\0') {
			break;
		}
	}

	if (rebootnack) {			// Rebooted, but not acknowledged
		if (cmdline[0] == '!') {
			rebootnack = 0;
			send_prompt(GREATERPROMPT);
			return;
		} else {
			send_prompt(EXCLAIMPROMPT);
			return;			
		}
	}

	prompt_flag = GREATERPROMPT;
	ptr = cmdline;

	switch (*ptr) {

		case 'c':
			prompt_flag = pneu_close(ptr);
			break;

// IGNORE---------------------------------------------
		case 'e':
			ptr++;
			writestr_OLED(0, ptr, 1);
			prompt_flag = GREATERPROMPT;
			break;

		case 'E':
			ptr++;
			writestr_OLED(0, ptr, 2);
			prompt_flag = GREATERPROMPT;
			break;

		case 'f':
			framdata[0] = 'Q';
			framdata[1] = 'B';
			framdata[2] = 'X';
			framdata[3] = 'D';
			framdata[4] = 'E';
			framdata[5] = 'F';
			framdata[6] = 'G';
			framdata[7] = 'H';
			framdata[8] = 'I';
			framdata[9] = '\0';

			memaddress = 0;
			write_FRAM(FRAMADDR, memaddress, framdata, 10);

			for (i = 0; i < 10; i++) {
				framtest[i] = 0;
			}
			memaddress = 0;
			read_FRAM(FRAMADDR, memaddress, framtest, 10);
			sprintf(strbuf, "%s read from fram\r\n", framtest);
			send_USART(0, (uint8_t*) strbuf, strlen(strbuf));
			prompt_flag = GREATERPROMPT;
			break;
// STOP IGNORE----------------------------------------

		case 'o':				// open
			prompt_flag = pneu_open(ptr);
			break;

		case 'r':				// report
			prompt_flag = report(ptr);
			break;

		case 'R':
			reboot();
			break;

		case 's':				// set
			prompt_flag = set(ptr);
			break;

		case '\0':
			prompt_flag = GREATERPROMPT;
			break;
	
		default:				// send an error
			prompt_flag = ERRORPROMPT;
			break;

	}

	send_prompt(prompt_flag);

}

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
uint8_t report(char *ptr)
{

	char outbuf[81], isotime[21], version[11];
	float t0, t1, t2, t3, h0, h1, h2;		// temperature and humidity
	float redvac, bluvac;						// red and blue vacuum
	const char format_BTM[]="$S%dBTM,%s";
	const char format_TIM[]="$S%dTIM,%s";
	const char format_VER[]="$S%dVER,%s";
	const char format_ENV[]="$S%dENV,%3.1fC,%1.0f%%,%3.1fC,%1.0f%%,%3.1fC,%1.0f%%,%3.1fC";
	const char format_VAC[]="$S%dVAC,%5.2f,red,%5.2f,blue";

	ptr++;

	t0 = t1 = t2 = t3 = -99.0;
	h0 = h1 = h2 = -99.0;

	switch(*ptr++) {

		case 'B':					// Boot time
			get_BOOTTIME(isotime);
			sprintf(outbuf, format_BTM, get_specID(), isotime);
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
			sprintf(outbuf, format_ENV, get_specID(), t0, h0, t1, h1, t2, h2, t3);
			checksum_NMEA(outbuf);
			send_USART(0, (uint8_t*) outbuf, strlen(outbuf));
			break;

		case 't':					// Report current time on specMech clock
			get_time(isotime);
			sprintf(outbuf, format_TIM, get_specID(), isotime);
			checksum_NMEA(outbuf);
			send_USART(0, (uint8_t*) outbuf, strlen(outbuf));
			break;

		case 'v':
			redvac = read_ionpump(REDPUMP);
			bluvac = read_ionpump(BLUEPUMP);
			sprintf(outbuf, format_VAC, get_specID(), redvac, bluvac);
			checksum_NMEA(outbuf);
			send_USART(0, (uint8_t*) outbuf, strlen(outbuf));
			break;

		case 'V':
			get_VERSION(version);	// Send the specMech version
			sprintf(outbuf, format_VER, get_specID(), version);
			checksum_NMEA(outbuf);
			send_USART(0, (uint8_t*) outbuf, strlen(outbuf));
			break;

		default:
			return(ERRORPROMPT);
			break;

	}

	return(GREATERPROMPT);

}

/*------------------------------------------------------------------------------
void send_prompt(uint8_t prompt_flag)
	Puts a command line prompt on the output line. These could be the normal
	greater than (>) for a successful transaction, or an NMEA error line
	followed by the greater than (>).

	If specMech is rebooted, it will only prompt with a single exclamation
	point (!) until it receives an acknowledgment in the form of a single !.
------------------------------------------------------------------------------*/
void send_prompt(uint8_t prompt_flag)
{

	const char str0[] = ">";
	const char str1[] = "!";

	char prompt_str[25];

	switch (prompt_flag) {
		case GREATERPROMPT:
			strcpy(prompt_str, str0);
			send_USART(0, (uint8_t*) prompt_str, strlen(prompt_str));
			break;

		case ERRORPROMPT:
			format_ERR(prompt_str);
			send_USART(0, (uint8_t*) prompt_str, strlen(prompt_str));
			strcpy(prompt_str, str0);
			send_USART(0, (uint8_t*) prompt_str, strlen(prompt_str));
			break;

		case EXCLAIMPROMPT:
			strcpy(prompt_str, str1);
			send_USART(0, (uint8_t*) prompt_str, strlen(prompt_str));
			break;

		default:
			strcpy(prompt_str, str1);
			send_USART(0, (uint8_t*) prompt_str, strlen(prompt_str));
			break;

	}

}

/*------------------------------------------------------------------------------
uint8_t set (char *ptr)
	Sets hardware parameters.

	Input:
		*ptr - Command line pointer that is incremented to find the item to set.

	Returns:
		0 - on success
		1 - invalid item to set (an unrecognized character on the command line).
------------------------------------------------------------------------------*/
uint8_t set(char *ptr)
{

	ptr++;
	switch(*ptr) {
		case 't':			// date/time
			ptr++;
			put_time(ptr);
			break;

		default:
			return(ERRORPROMPT);
			break;
	}
	return(GREATERPROMPT);
}