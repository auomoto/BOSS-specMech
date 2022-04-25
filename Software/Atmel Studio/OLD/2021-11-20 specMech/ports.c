#include "globals.h"
#include "ports.h"

/*------------------------------------------------------------------------------
void init_PORTS(void)
------------------------------------------------------------------------------*/
void init_PORTS(void)
{

	PORTB.DIRSET = PIN2_bm;		// Beeper
	PORTF.DIRCLR = PIN2_bm;		// specID jumper
	PORTF.DIRSET = PIN5_bm;		// On-board LED
	PORTF.PIN6CTRL = PORT_PULLUPEN_bm | PORT_ISC_FALLING_gc;	// Pushbutton
	PORTC.OUTCLR = PIN3_bm;		// Reset the XPort
	PORTC.DIRSET = PIN3_bm;		// Set pin as output
	_delay_ms(10);				// Reset signal wait time
	PORTC.OUTSET = PIN3_bm;		// Enable the XPort

}
