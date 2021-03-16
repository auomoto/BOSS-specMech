/*------------------------------------------------------------------------------
specMech.c
	BOSS motion controller based on a Microchip Curiosity Nano with an
	ATMega4809 microcontroller.
------------------------------------------------------------------------------*/

#include "globals.h"
#include "initialize.h"
#include "usart.h"
#include "commands.h"

ParsedCMD pcmd[CSTACKSIZE];	// Split the command line into its parts

extern void initialize(void);

int main(void)
{

	firstpass = YES;		// Set to YES in commands.c
	initialize0();
	sei();
	initialize1();

	for (;;) {
		if (recv0_buf.done) {
			recv0_buf.done = NO;
			commands();
		}
	}
}
