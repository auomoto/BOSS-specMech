/* Add OLED Display
 * 2020-10-11
 * And eeprom
 * 2020-10-12
 * And WDT for reboots
*/

#define F_CPU		3333333UL
#define VERSION		"2020-12-17"

#define	YES			1
#define	NO			0
#define REBOOTNACK	2

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>		// sprintf
#include <string.h>		// for strcpy, strlen

#include "ports.c"
#include "led.c"
#include "twi.c"
#include "mcp23008.c"
#include "pneu.c"
#include "usart.c"
//#include "fram.c"
#include "ds3231.c"
#include "nmea.c"
#include "oled.c"
#include "eeprom.c"
#include "wdt.c"

// Function Prototypes
uint8_t close(char*);
void checksum_NMEA(char*);
void commands(void);
uint8_t get_specID(void);
uint8_t open(char*);
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

uint8_t close(char *ptr)
{

	ptr++;

	switch (*ptr) {

		case 'b':
			set_valves(LEFTBM, LEFTCLOSE);
			set_valves(RIGHTBM, RIGHTCLOSE);
			break;

		case 'l':
			set_valves(LEFTBM, LEFTCLOSE);
			break;
			
		case 'r':
			set_valves(RIGHTBM, RIGHTCLOSE);
			break;

		case 's':										// Close shutter
			set_valves(SHUTTERBM, SHUTTERCLOSE);
			break;

		default:
			return(1);
			break;

	}

	return(0);

}

void commands(void)
{

	uint8_t i, prompt_flag; //, framdata[10];
	char cmdline[BUFSIZE];
	char *ptr, strbuf[20];
//	uint16_t memaddress;
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
			send_prompt(0);
			return;
		} else {
			send_prompt(REBOOTNACK);
			return;			
		}
	}

	prompt_flag = 0;
	ptr = cmdline;

	switch (*ptr) {

		case 'c':
			prompt_flag = close(ptr);
			break;

// IGNORE---------------------------------------------
		case 'e':
			ptr++;
			writestr_OLED(0, ptr, 1);
			prompt_flag = 0;
			break;

		case 'E':
			ptr++;
			writestr_OLED(0, ptr, 2);
			prompt_flag = 0;
			break;

		case 'f':
//			memaddress = 0;
//			framdata[0] = 0xAA;
//			write_FRAM(FRAMADDR, memaddress, framdata, 1);
//			read_FRAM(FRAMADDR, memaddress, framdata, 2);
			strcpy(strbuf, "read FRAM\r\n");
			send_USART(0, (uint8_t*) strbuf, strlen(strbuf));
			prompt_flag = 0;
			break;
// STOP IGNORE----------------------------------------

		case 'o':				// open
			prompt_flag = open(ptr);
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
			prompt_flag = 0;
			break;
	
		default:				// send a ?
			prompt_flag = 1;
			break;

	}

	send_prompt(prompt_flag);

}

uint8_t open(char *ptr)
{

	ptr++;
	switch (*ptr) {

		case 'b':
			set_valves(LEFTBM, LEFTOPEN);
			set_valves(RIGHTBM, RIGHTOPEN);
			break;

		case 'l':
			set_valves(LEFTBM, LEFTOPEN);
			break;
		
		case 'r':
			set_valves(RIGHTBM, RIGHTOPEN);
			break;

		case 's':
			set_valves(SHUTTERBM, SHUTTEROPEN);
			break;

		default:
			return(1);

	}

	return(0);

}

uint8_t report(char *ptr)
{

	char outbuf[81], isotime[21], version[11];
	const char format_BTM[]="$S%dBTM,%s";
	const char format_TIM[]="$S%dTIM,%s";
	const char format_VER[]="$S%dVER,%s";

	ptr++;

	switch(*ptr++) {

		case 'b':					// Boot time
			get_BOOTTIME(isotime);
			sprintf(outbuf, format_BTM, get_specID(), isotime);
			checksum_NMEA(outbuf);
			send_USART(0, (uint8_t*) outbuf, strlen(outbuf));

			get_EOD(outbuf);
			send_USART(0, (uint8_t*) outbuf, strlen(outbuf));
			break;

		case 't':					// Send current time on specMech clock
			get_time(isotime);
			sprintf(outbuf, format_TIM, get_specID(), isotime);
			checksum_NMEA(outbuf);
			send_USART(0, (uint8_t*) outbuf, strlen(outbuf));

			get_EOD(outbuf);
			send_USART(0, (uint8_t*) outbuf, strlen(outbuf));
			break;

		case 'v':
			get_VERSION(version);	// Send the specMech version
			sprintf(outbuf, format_VER, get_specID(), version);
			checksum_NMEA(outbuf);
			send_USART(0, (uint8_t*) outbuf, strlen(outbuf));

			get_EOD(outbuf);
			send_USART(0, (uint8_t*) outbuf, strlen(outbuf));
			break;

		default:
			return(1);
			break;

	}

	return(0);

}

void send_prompt(uint8_t prompt_flag)
{

	const char str0[] = ">";
	const char str1[] = "!";

	char prompt_str[25];

	switch (prompt_flag) {
		case 0:
			strcpy(prompt_str, str0);
			send_USART(0, (uint8_t*) prompt_str, strlen(prompt_str));
			break;

		case 1:
			get_ERR(prompt_str);
			send_USART(0, (uint8_t*) prompt_str, strlen(prompt_str));
			get_EOD(prompt_str);
			send_USART(0, (uint8_t*) prompt_str, strlen(prompt_str));
			strcpy(prompt_str, str0);
			send_USART(0, (uint8_t*) prompt_str, strlen(prompt_str));
			break;

		case REBOOTNACK:
			strcpy(prompt_str, str1);
			send_USART(0, (uint8_t*) prompt_str, strlen(prompt_str));
			break;

		default:
			strcpy(prompt_str, str1);
			send_USART(0, (uint8_t*) prompt_str, strlen(prompt_str));
			break;
			
	}


}

uint8_t set(char *ptr)
{

	ptr++;
	switch(*ptr) {
		case 't':			// date/time
			ptr++;
//			put_time(ptr);
			break;

		default:
			return(1);
			break;
	}
	return(0);
}