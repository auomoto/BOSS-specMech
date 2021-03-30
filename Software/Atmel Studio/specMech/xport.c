/*------------------------------------------------------------------------------
xport.c
	Lantronix XPort
------------------------------------------------------------------------------*/

#include "globals.h"
#include "xport.h"

void init_XPORT(void)
{

	PORTC.OUTCLR = PIN3_bm;		// PC3 is the /RESET pin for the XPort
	PORTC.DIRSET = PIN3_bm;
	_delay_ms(10);				// Reset signal wait time
	PORTC.OUTSET = PIN3_bm;

}
