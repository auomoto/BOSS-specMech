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

uint8_t old_recv0bufhead;	// debugging

	initialize();
	sei();

old_recv0bufhead = 0;

	for (;;) {
		if (old_recv0bufhead != recv0_buf.head) {
			old_recv0bufhead = recv0_buf.head;
			on_BEEPER;
			_delay_ms(50);
			off_BEEPER;
		}
		if (recv0_buf.done) {
			recv0_buf.done = NO;
			commands();

		}
	}

}

