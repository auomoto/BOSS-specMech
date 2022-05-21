#include "globals.h"
#include "fan.h"

/*------------------------------------------------------------------------------
void fan(uint8_t)

------------------------------------------------------------------------------*/
void fan(uint8_t onoff)
{

	if (onoff == ON) {
		PORTB.OUTSET = PIN3_bm;
	} else {
		PORTB.OUTCLR = PIN3_bm;
	}

}
