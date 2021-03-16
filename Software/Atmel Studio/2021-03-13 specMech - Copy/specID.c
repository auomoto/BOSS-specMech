#include "globals.h"
#include "specID.h"

/*------------------------------------------------------------------------------
void init_specID(void)
	Sets the spectrograph ID pin on the ATMega4809 to be an input without
	pullup. Must be called AFTER init_PORTS().
------------------------------------------------------------------------------*/
void init_SPECID(void)
{

	PORTF.PIN2CTRL &= ~PORT_PULLUPEN_bm;	// Remove pullup
	PORTF.DIRCLR = PIN2_bm;					// Set as input
	
}
