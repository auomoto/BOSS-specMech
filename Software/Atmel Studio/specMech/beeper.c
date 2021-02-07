/*------------------------------------------------------------------------------
beeper.c
	specMech beeper is on pin PB2.
------------------------------------------------------------------------------*/

#include "globals.h"

void init_BEEPER(void)
{

	PORTB.OUTSET = PIN2_bm;
	PORTB.DIRSET = PIN2_bm;
	PORTB.OUTCLR = PIN2_bm;

}

void off_BEEPER(void)
{
	PORTB.OUTCLR = PIN2_bm;
}

void on_BEEPER(void)
{
	PORTB.OUTSET = PIN2_bm;
}

void toggle_BEEPER(void)
{
	PORTB.OUTTGL = PIN2_bm;
}
