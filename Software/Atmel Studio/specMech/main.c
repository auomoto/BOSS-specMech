/*------------------------------------------------------------------------------
specMech.c
	BOSS motion controller board based on an ATMega4809 implemented on a
	Microchip Curiosity Nano
------------------------------------------------------------------------------*/

#define CVALUESIZE		41	// Maximum length of a command value string
#define CIDSIZE			9	// Maximum length of a command ID string
#define CSTACKSIZE		10	// Number of stacked up commands allowed

#include "globals.h"		// Global variables

#include <avr/io.h>
#include <util/delay.h>

//#include "ports.c"			// ATMega4809 port initializations
//#include "led.c"			// On-board LED
//#include "beeper.c"			// Beeper
//#include "specid.c"			// Spectrograph ID jumper
//#include "twi.c"			// I2C
//#include "mcp23008.c"		// MCP23008 port expander
//#include "pneu.c"			// Pneumatic valves and sensors
#include "ads1115.c"		// ADS1115 ADC
#include "ad590.c"			// AD590 temperature sensors
#include "mcp9808.c"		// MCP9808 sensor
#include "temperature.c"	// AD590 and MCP9808 sensors
#include "humidity.c"		// Honeywell HiH-4031
#include "ionpump.c"		// Read the ion pump vacuum
//#include "usart.c"			// Serial (RS232) communications
#include "fram.c"			// FRAM memory
//#include "ds3231.c"			// Day-time clock
//#include "mma8451.c"		// Accelerometer
//#include "nmea.c"			// GPS style output
//#include "oled.c"			// Newhaven OLED displays
//#include "eeprom.c"			// ATMega4809 eeprom
#include "wdt.c"			// Watchdog timer used only for reboot function
#include "interrupts.c"		// Interrupt service routines
<<<<<<< HEAD
//#include "rtc.c"			// Real time clock one second ticks
=======
>>>>>>> parent of b752f80... Set up RTC for flashing led

// Function Prototypes
void commands(void);
void echo_cmd(char *str);
uint8_t isaletter(char);
void parse_cmd(char*, uint8_t);
uint8_t report(uint8_t);
void send_prompt(uint8_t);
uint8_t set(uint8_t);
void initialize(void);

typedef struct {
	char cverb,			// Single character command
	cobject,			// Single character object
	cvalue[CVALUESIZE],	// Input value string for object
	cid[CIDSIZE];		// Command ID string
} ParsedCMD;

ParsedCMD pcmd[CSTACKSIZE];	// Split the command line into its parts

uint8_t specMechErrors;

int main(void)
{
<<<<<<< HEAD

//	char strbuf[11];
	initialize();
=======
	char strbuf[11];
	init_PORTS();
	init_LED();
	init_BEEPER();
	init_SPECID();
	init_TWI();
	init_PNEU();
	init_USART();
	init_OLED(0);
	init_OLED(1);
	init_EEPROM();
	init_MMA8451();
>>>>>>> parent of b752f80... Set up RTC for flashing led

/*
	writestr_OLED(1, "specMech Version", 1);
	get_VERSION(strbuf);
	writestr_OLED(1, strbuf, 2);
*/

	sei();

	specMechErrors = 0;
	for (;;) {
		if (recv0_buf.done) {
			recv0_buf.done = NO;
			commands();
		}
	}

}

/*------------------------------------------------------------------------------
void commands(void)
	Command loop
------------------------------------------------------------------------------*/
void commands(void)
{

	char cmdline[BUFSIZE];			// BUFSIZE is the size of the ring buffer
	char verb, object;
	uint8_t i, prompt_flag = GREATERPROMPT;
	static uint8_t rebootnack = 1;
	static uint8_t cstack = 0;		// Index for pcmd

on_BEEPER();
_delay_ms(50);
off_BEEPER();

	// Copy the command string to cmdline
	for (i = 0; i < BUFSIZE; i++) {
		cmdline[i] = recv0_buf.data[recv0_buf.tail];
		recv0_buf.tail = (recv0_buf.tail + 1) % BUFSIZE;
		if (cmdline[i] == '\0') {
			break;
		}
	}

	// Check if rebooted
	if (rebootnack) {
		if (cmdline[0] != '!') {
			send_prompt(EXCLAIMPROMPT);		
			return;
		} else {
			send_prompt(GREATERPROMPT);
			rebootnack = 0;
			return;
		}
	}

	// Echo the command back to the user
	echo_cmd(cmdline);

	if (strlen(cmdline) == 0) {		// Catch a terminal cr
		send_prompt(GREATERPROMPT);
		return;
	}

	parse_cmd(cmdline, cstack);
	verb = pcmd[cstack].cverb;
	object = pcmd[cstack].cobject;

	switch (verb) {
		case 'c':				// close
			prompt_flag = close_PNEU(object);
			break;

		case 'o':				// open
			prompt_flag = open_PNEU(object);
			break;

		case 'r':				// Report
			prompt_flag = report(cstack);
			break;

		case 's':				// Set
			prompt_flag = set(cstack);
			break;

		case 'R':				// Reboot
			send_prompt(GREATERPROMPT);
			_delay_ms(100);		// Avoid finishing the command loop before reboot
			reboot();
			return;

		default:
			prompt_flag = ERRORPROMPT;
			break;			
	}

	cstack = (cstack + 1) % CSTACKSIZE;
	send_prompt(prompt_flag);

}

/*------------------------------------------------------------------------------
void echo_cmd(char *cmdline)
	Echo the command line back to the user, adding the NMEA header and checksum.
------------------------------------------------------------------------------*/
void echo_cmd(char *cmdline)
{

	char format_CMD[] = "$S%dCMD,%s";
	char strbuf[BUFSIZE+10];		// Add $SXCMD, and *HH checksum

		// Format and echo the command line
	sprintf(strbuf, format_CMD, get_SPECID(), cmdline);
	checksum_NMEA(strbuf);
	send_USART(0, (uint8_t*) strbuf, strlen(strbuf));

		// Put it on the display
	clear_OLED(1);
	on_OLED(1);
	writestr_OLED(1, cmdline, 1);

}
/*
void initialize(void)
{

		// Curiosity nano on-board button
	PORTF.PIN6CTRL = PORT_PULLUPEN_bm | PORT_ISC_FALLING_gc;
	init_PORTS();
	init_LED();
	init_BEEPER();
	init_RTC(64);	// Fast flashing LED before reboot ACK
	init_SPECID();
	init_TWI();
	init_PNEU();
	init_USART();
	init_OLED(0);
	init_OLED(1);
	init_EEPROM();
	init_MMA8451();

}
*/
/*------------------------------------------------------------------------------
uint8_t isaletter(char c)
	Checks if the character is in the range A-Z and a-z
------------------------------------------------------------------------------*/
uint8_t isaletter(char c)
{

	if (c >= 'a' && c <= 'z') {
		return(1);
	}

	if (c >= 'A' && c <= 'Z') {
		return(1);
	}

	return(0);

}

/*------------------------------------------------------------------------------
void parse_cmd(char *ptr, uint8_t n)
	Breaks up the command line into its components. The components are:
		verb - The command verb, a single character.
		object - The commanded object, a single character.
		value - The object's new value, a character string.
		ID - An identifier selected by the user, a character string.
	These components are in the ParsedCMD structure defined in main.c. The pcmd
	variable is a ParsedCMD array.
------------------------------------------------------------------------------*/
void parse_cmd(char *ptr, uint8_t n)
{

	uint8_t i;

		// Clear the command parts
	pcmd[n].cverb = '?';
	pcmd[n].cobject = '?';
	pcmd[n].cvalue[0] = '\0';
	pcmd[n].cid[0] = '\0';

		// Find the verb
	while (!isaletter(*ptr)) {
		if (*ptr == '\0') {
			return;
		}
		ptr++;
	}
	pcmd[n].cverb = *ptr++;

		// Find the object
	while (!isaletter(*ptr)) {
		if (*ptr == '\0') {
			return;
		}
		ptr++;
	}
	pcmd[n].cobject = *ptr++;

		// Get the value, if there is one
	for (i = 0; i < CVALUESIZE; i++) {
		if (*ptr == '\0') {
			pcmd[n].cvalue[i] = '\0';
			return;
		}
		if (*ptr == ';') {
			pcmd[n].cvalue[i] = '\0';
			break;
		}
		pcmd[n].cvalue[i] = *ptr++;
	}

		// get the command ID if there is one
	ptr++;
	for (i = 0; i < CIDSIZE; i++) {
		if (*ptr == '\0') {
			pcmd[n].cid[i] = '\0';
			return;
		}
		pcmd[n].cid[i] = *ptr++;
	}	

	return;

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
			sprintf(outbuf, format_BTM, get_SPECID(), boottime, pcmd[cstack].cid);
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
			sprintf(outbuf, format_ENV, get_SPECID(), currenttime, t0, h0, t1, h1, t2, h2, t3, pcmd[cstack].cid);
			checksum_NMEA(outbuf);
			send_USART(0, (uint8_t*) outbuf, strlen(outbuf));
			break;

		case 'o':					// Orientation
			get_orientation(MMA8451ADDR, &x, &y, &z);
			get_time(currenttime);
			sprintf(outbuf, format_ORI, get_SPECID(), currenttime, x, y, z, pcmd[cstack].cid);
//			sprintf(outbuf, "x=%03.1f y=%03.1f z=%03.1f", x, y, z);
			checksum_NMEA(outbuf);
			send_USART(0, (uint8_t*) outbuf, strlen(outbuf));
			break;

		case 'p':
			get_time(currenttime);
			read_pneusensors(&shutter, &left, &right, &air);
			sprintf(outbuf, format_PNU, get_SPECID(), currenttime, shutter, left, right, air, pcmd[cstack].cid);
			checksum_NMEA(outbuf);
			send_USART(0, (uint8_t*) outbuf, strlen(outbuf));
			break;

		case 't':					// Report current time on specMech clock
			get_time(currenttime);
			read_FRAM(FRAMADDR, SETTIMEADDR, (uint8_t*) lastsettime, 20);
			get_BOOTTIME(boottime);
			sprintf(outbuf, format_TIM, get_SPECID(), currenttime, lastsettime,
				boottime, pcmd[cstack].cid);
			checksum_NMEA(outbuf);
			send_USART(0, (uint8_t*) outbuf, strlen(outbuf));
			break;

		case 'v':
			redvac = read_ionpump(REDPUMP);
			bluvac = read_ionpump(BLUEPUMP);
			get_time(currenttime);
			sprintf(outbuf, format_VAC, get_SPECID(), currenttime, redvac, bluvac, pcmd[cstack].cid);
			checksum_NMEA(outbuf);
			send_USART(0, (uint8_t*) outbuf, strlen(outbuf));
			break;

		case 'V':
			get_VERSION(version);	// Send the specMech version
			get_time(currenttime);
			sprintf(outbuf, format_VER, get_SPECID(), currenttime, version, pcmd[cstack].cid);
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

	Input:
		cstack - The array index in the ParsedCMD array (pcmd). The command
			must have been parsed already

	Returns:
		GREATERPROMPT on success
		ERRORPROMPT if an invalid object was requested.
------------------------------------------------------------------------------*/
uint8_t set(uint8_t cstack)
{

	char object;

	object = pcmd[cstack].cobject;

	switch(object) {
		case 't':
			if (strlen(pcmd[cstack].cvalue) != 19) {
				return(ERRORPROMPT);
			}
			put_time(pcmd[cstack].cvalue);
			write_FRAM(FRAMADDR, SETTIMEADDR, (uint8_t*) pcmd[cstack].cvalue, 20);
			break;

		default:
			return(ERRORPROMPT);
	}
	return(GREATERPROMPT);
}
