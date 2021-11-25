#include "globals.h"
#include "initialize.h"
#include "wdt.h"
#include "ds3231.h"
#include "usart.h"
#include "set.h"
#include "nmea.h"
#include "roboclaw.h"
#include "oled.h"
#include "pneu.h"
#include "rtc.h"
#include "report.h"
#include "errors.h"
#include "testroutine.h"
#include "commands.h"

extern uint8_t firstpass;

/*------------------------------------------------------------------------------
void commands(void)
	Command loop
------------------------------------------------------------------------------*/
void commands(void)
{

	char cmdline[BUFSIZE+1];		// BUFSIZE is the size of the ring buffer
	static uint8_t cstack = 0;		// pcmd index

	get_cmdline(cmdline);

	if (!rebootACKd(cmdline)) {		// Reboot not yet acknowledged
		return;
	}

		// <CR> or ! alone are not errors
	if ((cmdline[0] == '\0') || (cmdline[0] == '!')) {
		firstpass = NO;
		send_GTprompt();
		return;
	}

	echo_cmd(cmdline);
	parse_cmd(cmdline, cstack);

	switch (pcmd[cstack].cverb) {
		case 'c':				// close
			close_PNEU(cstack);
			break;

		case 'o':				// open
			open_PNEU(cstack);
			break;

		case 'm':				// move
			move_MOTOR_CMD(cstack);
			break;

		case 'r':				// report
			report(cstack);
			break;

		case 's':				// set
			set(cstack);
			break;

		case 't':
			testroutine();
			break;

		case 'I':	// test init_MOTORS
			init_MOTORS();
			break;

		case 'R':				// Reboot
			squelchErrors = YES;
			if (motorsMoving()) {
				squelchErrors = NO;
				printError(ERR_MOTORMOVING, "Can't reboot, motor moving");
				break;
			} else {
				saveFRAM_MOTOREncoders();
				timerSAVEENCODER = 0;
				send_GTprompt();	// Aidan request
				_delay_ms(100);		// Avoids finishing the command loop before reboot
				reboot();
				return;
			}

		default:
			printError(ERR_BADCOMMAND, "Not a command");
			break;			
	}

	cstack = (cstack + 1) % CSTACKSIZE;
	send_GTprompt();

}

/*------------------------------------------------------------------------------
void echo_cmd(char *cmdline)
	Echo the command line back to the user, adding NMEA header and checksum.
------------------------------------------------------------------------------*/
void echo_cmd(char *cmdline)
{

	const char format_CMD[] = "CMD,%s,%s";
	char currenttime[20], strbuf[BUFSIZE];

	get_time(currenttime);
	sprintf(strbuf, format_CMD, currenttime, cmdline);
	printLine(strbuf);

}

/*------------------------------------------------------------------------------
uint8_t get_cmdline(char *cmdline)
	Copies the USART0 input buffer to the "cmdline" string
------------------------------------------------------------------------------*/
void get_cmdline(char *cmdline)
{

	uint8_t i;

	for (i = 0; recv0_buf.length; i++) {
		cmdline[i] = recv0_buf.data[recv0_buf.tail];
		recv0_buf.length--;
		recv0_buf.tail = (recv0_buf.tail + 1) % BUFSIZE;
		if (cmdline[i] == '\0') {
			break;
		}
	}

	cmdline[i] = '\0';		// Overflow protection

}

/*------------------------------------------------------------------------------
uint8_t isadigit(char d)
	Checks if the character d is in the range 0-9
------------------------------------------------------------------------------*/
uint8_t isadigit(char d)
{
	if ((d >= '0') && (d <= '9')) {
		return(ERROR);
	}
	return(NOERROR);

}

/*------------------------------------------------------------------------------
uint8_t isaletter(char c)
	Checks if the character is in the range A-Z and a-z
------------------------------------------------------------------------------*/
uint8_t isaletter(char c)
{

	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) {
		return(ERROR);
	}
	return(NOERROR);

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
	while (!isaletter(*ptr)) {		// the verb is a single letter
		if (*ptr == '\0') {
			return;
		}
		ptr++;
	}
	pcmd[n].cverb = *ptr++;

		// Find the object
	while (!isaletter(*ptr)) {		// objects are single letters
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
		if (*ptr == ';') {			// Command ID separator
			pcmd[n].cvalue[i] = '\0';
			break;
		}
		pcmd[n].cvalue[i] = *ptr++;
	}

		// get the optional command ID if there is one
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
void printLine(char *str)
	Formats and prints a line to USART0

	Input:
		str: a character string

	Output:
		Forwards a modified string to USART0

	Returns:
		Nothing

	Notes:
		This routine adds the NMEA front material (the $S2 or $S1) and tacks
		on the NMEA checksum at the end before sending the string to the serial
		port.

		This routine is disabled until after the initialization routines have
		been run and the system has received a reboot acknowledge (the !). This
		is done to prevent unsolicited text being sent (error messages) at
		initialization or after a reboot.
------------------------------------------------------------------------------*/
void printLine(char *str)
{

	char strbuf[BUFSIZE];
	const char prFormat[] = "$S%d%s";

	if (firstpass) {
		return;
	}

	sprintf(strbuf, prFormat, get_SPECID, str);
	checksum_NMEA(strbuf);
	send_USART(0, (uint8_t*) strbuf, strlen(strbuf));

}

/*------------------------------------------------------------------------------
uint8_t rebootACKd(char *cmdline)
	Checks to see if a processor reboot has been acknowledged with a "!\r"
	command string.

	Input:
		cmdline - the command line string from USART0

	Returns:
		YES if the reboot acknowledge string was received
		NO if it wasn't
------------------------------------------------------------------------------*/
uint8_t rebootACKd(char *cmdline)
{

	if (!rebootackd) {
		if ((cmdline[0] == '!') && (cmdline[1] == '\0')) {
			init_RTC(511);		// 1-sec RTC clock ticks
			timeoutOLED = 5;	// 5-sec display timeout (minimum)
			rebootackd = YES;
			return(YES);
		} else if ((cmdline[0] == '!') && (cmdline[1] != '\0')) {
			reboot();
			return(NO);
		} else {
			send_EXprompt();
//			send_prompt('!');
			return(NO);
		}
	}

	return(YES);

}

/*------------------------------------------------------------------------------
void send_EXprompt(void)
	Sends an exclamation mark on USART port 0
------------------------------------------------------------------------------*/
void send_EXprompt(void)
{

	const char str[] = "!";

	send_USART(0, (uint8_t*) str, 1);

}

/*------------------------------------------------------------------------------
void send_GTprompt(void)
	Sends a greater than (>) on USART port 0
------------------------------------------------------------------------------*/
void send_GTprompt(void)
{

	const char str[] = ">";

	send_USART(0, (uint8_t*) str, 1);

}

/*------------------------------------------------------------------------------
void send_prompt(char prompt) [CHECK IF THIS IS NEEDED]
	Puts a command line prompt on the output line.

	If specMech is rebooted, it will only prompt with a single exclamation
	point (!) until it receives an acknowledgment in the form of a single !.
------------------------------------------------------------------------------*/
/*
void send_prompt(char prompt)
{

	char prompt_str[2];

	prompt_str[0] = prompt;
	prompt_str[1] = '\0';
	send_USART(0, (uint8_t*) prompt_str, 1);

}
*/
/*------------------------------------------------------------------------------
void send_sprompt(char *prompt)
	Puts a command line prompt on the output line.

	If specMech is rebooted, it will only prompt with a single exclamation
	point (!) until it receives an acknowledgment in the form of a single !.
------------------------------------------------------------------------------*/
/*
void send_sprompt(char *prompt)
{

	send_USART(0, (uint8_t*) prompt, strlen(prompt));

}
*/
