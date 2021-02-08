/*------------------------------------------------------------------------------
specMech.c
	BOSS motion controller based on a Microchip Curiosity Nano with an
	ATMega4809 microcontroller.
------------------------------------------------------------------------------*/

#include "globals.h"

ParsedCMD pcmd[CSTACKSIZE];	// Split the command line into its parts

int main(void)
{

	initialize();
	sei();

writestr_OLED(1,"hi1 there", 1);
writestr_OLED(0,"hi0 there", 1);

	for (;;) {
		if (recv0_buf.done) {
			recv0_buf.done = NO;
			commands();

		}
	}

}

