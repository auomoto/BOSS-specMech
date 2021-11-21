#include "globals.h"
#include "ports.h"

/*------------------------------------------------------------------------------
void init_PORTS(void)
------------------------------------------------------------------------------*/
void init_PORTS(void)
{

	PORTF.PIN6CTRL = PORT_PULLUPEN_bm | PORT_ISC_FALLING_gc;	// Pushbutton
	PORTF.DIRCLR = PIN2_bm;										// SPECID port
	PORTF.DIRSET = PIN5_bm;										// LED output
	PORTB.DIRSET = PIN2_bm;										// Beeper

}
