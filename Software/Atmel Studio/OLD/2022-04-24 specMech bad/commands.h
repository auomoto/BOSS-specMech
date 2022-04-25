#ifndef COMMANDSH
#define COMMANDSH

#include "initialize.h"
#include "wdt.h"
#include "ds3231.h"
#include "usart.h"
#include "set.h"
#include "nmea.h"
#include "roboclaw.h"
#include "oled.h"
#include "pneu.h"
#include "timers.h"
#include "report.h"
#include "errors.h"
#include "testroutine.h"

#define CVALUESIZE		41	// Maximum length of a command value string
#define CIDSIZE			9	// Maximum length of a command ID string
#define CSTACKSIZE		10	// Number of stacked up commands allowed

typedef struct {
	char cverb,				// Single character command
		cobject,			// Single character object
		cvalue[CVALUESIZE],	// Input value string for object
		cid[CIDSIZE];		// Command ID string
} ParsedCMD;

extern ParsedCMD pcmd[CSTACKSIZE];	// Split the command line into its parts (see main.c)
extern uint8_t firstpass;

void commands(void);
void echo_cmd(char*);
void get_cmdline(char*);
uint8_t isadigit(char);
uint8_t isaletter(char);
void parse_cmd(char*, uint8_t);
void printLine(char*);
uint8_t rebootACKd(char*);
void send_EXprompt(void);
void send_GTprompt(void);
//void send_prompt(char);
//void send_sprompt(char*);

#endif