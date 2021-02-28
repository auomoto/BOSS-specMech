#include "globals.h"

void init_BEEPER(void)
{

	PORTB.OUTSET = PIN2_bm;
	PORTB.DIRSET = PIN2_bm;
	PORTB.OUTCLR = PIN2_bm;

}
