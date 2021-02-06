/*------------------------------------------------------------------------------
beeper.c
	specMech beeper is on pin PB2.
------------------------------------------------------------------------------*/

#ifndef BEEPERC
#define BEEPERC

// Function prototypes
void init_BEEPER(void);
void off_BEEPER(void);
void on_BEEPER(void);
void toggle_BEEPER(void);

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

#endif
