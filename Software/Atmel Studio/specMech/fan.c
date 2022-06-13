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

void fanstate(uint8_t cstack)
{

	switch (pcmd[cstack].cobject) {
		case '+':
			fan(ON);
			break;

		case '-':
			fan(OFF);
			break;

		default:
			break;
	}

}