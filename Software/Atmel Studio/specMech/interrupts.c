/*------------------------------------------------------------------------------
interrupts.c
	Curiosity Nano ATMega4809 on-board LED. The LED is on pin PF5.
------------------------------------------------------------------------------*/

#ifndef INTERRUPTSC
#define INTERRUPTSC

<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
//#include "led.c"
=======
>>>>>>> parent of b752f80... Set up RTC for flashing led
=======
>>>>>>> parent of b752f80... Set up RTC for flashing led
=======
>>>>>>> parent of b752f80... Set up RTC for flashing led
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

<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
/*---------------------------------------------------------------------
Interrupt routine for RTC
	Every tick of the RTC executes here
----------------------------------------------------------------------*/
ISR(RTC_CNT_vect)
{

	RTC.INTFLAGS = RTC_OVF_bm;		// Clear interrupt flag
	toggle_LED();					// Defined in led.c

}

=======
>>>>>>> parent of b752f80... Set up RTC for flashing led
=======
>>>>>>> parent of b752f80... Set up RTC for flashing led
=======
>>>>>>> parent of b752f80... Set up RTC for flashing led
#endif
