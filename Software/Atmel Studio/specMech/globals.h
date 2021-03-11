#ifndef GLOBALSH
#define GLOBALSH

#define F_CPU		3333333UL
#define VERSION		"2021-03-08"
#define	YES			1
#define	NO			0
#define BADFLOAT	(-666.0)
#define GREATERPROMPT	0	// Standard return prompt >
#define EXCLAIMPROMPT	1	// No REBOOT ACK prompt !
#define ERRORPROMPT		2	// Generate error line, then >

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>		// sprintf
#include <stdlib.h>		// atol()
#include <string.h>		// for strcpy, strlen

uint8_t notfirstpass;
uint8_t rebootackd;

#endif
