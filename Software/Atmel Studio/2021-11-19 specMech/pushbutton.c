#include "globals.h"
#include "pushbutton.h"

/*------------------------------------------------------------------------------
void init_PUSHBUTTON(void)
	PORTF PIN6 is connected to the Curiosity Nano on-board pushbutton. This
	pin is set up here to trigger an interrupt on a falling edge.
------------------------------------------------------------------------------*/
void init_PUSHBUTTON(void)
{

	PORTF.PIN6CTRL = PORT_PULLUPEN_bm | PORT_ISC_FALLING_gc;	// Pushbutton setup

}
