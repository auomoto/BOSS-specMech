#ifndef GLOBALSH
#define GLOBALSH

#define F_CPU		3333333UL
#define VERSION		"2021-12-03"
#define	YES			1
#define	NO			0
#define BADFLOAT	(-666.0)

// Port macros
#define get_SPECID		(PORTF.IN & PIN2_bm ? 2 : 1)
#define off_LED			(PORTF.OUTSET = PIN5_bm)
#define on_LED			(PORTF.OUTCLR = PIN5_bm)
#define toggle_LED		(PORTF.OUTTGL = PIN5_bm)
#define off_BEEPER		(PORTB.OUTCLR = PIN2_bm)
#define on_BEEPER		(PORTB.OUTSET = PIN2_bm)
#define toggle_BEEPER	(PORTB.OUTTGL = PIN2_bm)

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>		// sprintf
#include <stdlib.h>		// atol()
#include <string.h>		// for strcpy, strlen

#endif
