/*------------------------------------------------------------------------------
led.c
	Curiosity Nano ATMega4809 on-board LED. The LED is on pin PF5.
------------------------------------------------------------------------------*/

#include "globals.h"

void init_LED(void)
{
	PORTF.OUTSET = PIN5_bm;
	PORTF.DIRSET = PIN5_bm;
}

void off_LED(void)
{
	PORTF.OUTSET = PIN5_bm;
}

void on_LED(void)
{
	PORTF.OUTCLR = PIN5_bm;
}

void toggle_LED(void)
{
	PORTF.OUTTGL = PIN5_bm;
}
