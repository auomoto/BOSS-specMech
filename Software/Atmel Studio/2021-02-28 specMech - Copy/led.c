#include "globals.h"

void init_LED(void)
{
	PORTF.OUTSET = PIN5_bm;
	PORTF.DIRSET = PIN5_bm;
}
