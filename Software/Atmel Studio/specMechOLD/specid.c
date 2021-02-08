/*------------------------------------------------------------------------------
specID.c
	Curiosity Nano ATMega4809 on-board LED. The LED is on pin PF5.
------------------------------------------------------------------------------*/

#include "globals.h"

/*------------------------------------------------------------------------------
void init_SPECID(void)
	Sets the spectrograph ID pin on the ATMega4809 to be an input without
	pullup. Must be called AFTER init_PORTS().
------------------------------------------------------------------------------*/
void init_SPECID(void)
{

	PORTF.PIN2CTRL &= ~PORT_PULLUPEN_bm;	// Remove pullup
	PORTF.DIRCLR = PIN2_bm;					// Set as input
	
}

/*------------------------------------------------------------------------------
uint8_t get_SPECID(void);
	Reads the hardware jumper that selects between spectrograph 1 or 2.

	Input:
		None
	Returns:
		1 - jumper set to spectrograph 1
		2 - jumper set to spectrograph 2
------------------------------------------------------------------------------*/
uint8_t get_SPECID(void)
{

	uint8_t id;

	id = 0;
	if (PORTF.IN & PIN2_bm) {
		id = 2;
	}

	if (!(PORTF.IN & PIN2_bm)) {
		id = 1;
	}

	return(id);

}
