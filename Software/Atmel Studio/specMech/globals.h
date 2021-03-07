#ifndef GLOBALSH
#define GLOBALSH

#define F_CPU		3333333UL
#define VERSION		"2021-03-06"
#define	YES			1
#define	NO			0
#define GREATERPROMPT	0	// Standard return prompt >
#define EXCLAIMPROMPT	1	// No REBOOT ACK prompt !
#define ERRORPROMPT		2	// Generate error line, then >

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>		// sprintf
#include <stdlib.h>		// atol()
#include <string.h>		// for strcpy, strlen

// beeper.c
#define off_BEEPER		(PORTB.OUTCLR = PIN2_bm)
#define on_BEEPER		(PORTB.OUTSET = PIN2_bm)
#define toggle_BEEPER	(PORTB.OUTTGL = PIN2_bm)

// specID.c
#define get_SPECID	(PORTF.IN & PIN2_bm ? 2 : 1)


// wdt.c
void reboot(void);

// temperature.c
float get_temperature(uint8_t);

// humidity.c
float get_humidity(uint8_t);

// set.c
uint8_t set(uint8_t);

// report.c
uint8_t report(uint8_t);

// timers.c
volatile uint16_t ticks;
void start_TCB0(uint16_t);
void stop_TCB0(void);

// testroutine.c
void testroutine(void);
uint8_t scratchbuf[120];

// errors
#define ERR_GETTIME		(101)	// DS3231 day-time clock not responding
void printError(uint8_t, char*);

#endif
