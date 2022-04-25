/*------------------------------------------------------------------------------
specMech.c
	BOSS motion controller based on a Microchip Curiosity Nano with an
	ATMega4809 microcontroller.
------------------------------------------------------------------------------*/

#include "globals.h"

ParsedCMD pcmd[CSTACKSIZE];	// Split the command line into its parts

extern void initialize(void);

int main(void)
{

	initialize();
	sei();

	for (;;) {
		if (recv0_buf.done) {
			recv0_buf.done = NO;
			on_BEEPER;
			_delay_ms(50);
			off_BEEPER;
			commands();
		}
	}
}
