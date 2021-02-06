/*------------------------------------------------------------------------------
interrupts.c
	Curiosity Nano ATMega4809 on-board LED. The LED is on pin PF5.
------------------------------------------------------------------------------*/

#ifndef INTERRUPTSC
#define INTERRUPTSC

#include "wdt.c"

/*----------------------------------------------------------------------
	Curiosity Nano on-board pushbutton reboots the system
----------------------------------------------------------------------*/
ISR(PORTF_PORT_vect)
{

	if (PORTF.INTFLAGS & PIN6_bm) {		// Curiosity Nano button
		PORTF.INTFLAGS = PIN6_bm;		// Clear the interrupt flag
		reboot();
	}

}

#endif
