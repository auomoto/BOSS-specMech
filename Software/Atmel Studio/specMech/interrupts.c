/*------------------------------------------------------------------------------
interrupts.c
	Curiosity Nano ATMega4809 on-board LED. The LED is on pin PF5.
------------------------------------------------------------------------------*/

#ifndef INTERRUPTSC
#define INTERRUPTSC

#include "led.c"
#include "wdt.c"

/*----------------------------------------------------------------------
	Curiosity Nano on-board pushbutton reboots the system
----------------------------------------------------------------------*/
ISR(PORTF_PORT_vect)
{

	if (PORTF.INTFLAGS & PIN6_bm) {		// Curiosity Nano button
		PORTF.INTFLAGS = PIN6_bm;		// Clear the interrupt flag
		reboot();						// Defined in wdt.c
	}

}

/*---------------------------------------------------------------------
Interrupt routine for RTC

Start a fill after the fill interval has passed. Increment the fill
interval time and the valve-open times. If the valve has been open
longer than MAXOPENTIME or BUFMAXOPEN, then close the valve and set
the MAXOPEN flag. Usually, the valve should be closed by a thermistor
signal before MAXOPEN times are reached.
----------------------------------------------------------------------*/
ISR(RTC_CNT_vect)
{

	RTC.INTFLAGS = RTC_OVF_bm;		// Clear interrupt flag
	toggle_LED();					// Defined in led.c

}

#endif
