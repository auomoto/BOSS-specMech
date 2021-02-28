#include "globals.h"

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
