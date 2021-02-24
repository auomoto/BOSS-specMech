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

/*
int main(void)
{
	initialize();
	sei();

	for (;;) {
		if (recv0_buf.char_rcvd) {
			if (((recv0_buf.head + 2) % BUFSIZE) == recv0_buf.tail) {
				recv0_buf.head = recv0_buf.tail = 0;					// Overrun; start over
			} else if (recv0_buf.char_rcvd == '\r') {
				recv0_buf.data[recv0_buf.head] = '\0';
				recv0_buf.head = (recv0_buf.head + 1) % BUFSIZE;
				commands();
			} else {
				recv0_buf.head = (recv0_buf.head + 1) % BUFSIZE;
			}
		}
		recv0_buf.char_rcvd = 0;
	}
}


int xmain(void)
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

		if (recv0_buf.char_rcvd) {

on_BEEPER;
_delay_ms(50);
off_BEEPER;

			if (((recv0_buf.head + 2) % BUFSIZE) == recv0_buf.tail) {
				recv0_buf.head = recv0_buf.tail = 0;					// Overrun; start over	
			} else if (recv0_buf.char_rcvd == '\r') {
				recv0_buf.data[recv0_buf.head] = '\0';
				recv0_buf.head = (recv0_buf.head + 1) % BUFSIZE;
				commands();
			} else {
				recv0_buf.head = (recv0_buf.head + 1) % BUFSIZE;
			}
		}
		recv0_buf.char_rcvd = 0;
	}
}

*/