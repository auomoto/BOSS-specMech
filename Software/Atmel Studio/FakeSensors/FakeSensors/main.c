/*
 * FakeSensors.c
 *
 * Created: 2/9/2021 2:30:20 PM
 *
 */ 

#define F_CPU		3333333UL

#include <avr/io.h>
#include <util/delay.h>

#define off_LED		(PORTB.OUTSET = PIN7_bm)
#define on_LED		(PORTB.OUTCLR = PIN7_bm)
#define toggle_LED	(PORTB.OUTTGL = PIN7_bm)

// Function prototypes
void close(void);
void initialize(void);
void open(void);

int main(void)
{

	initialize();
	_delay_ms(1000);

	for (;;) {
		open();
		_delay_ms(250);
		close();
		_delay_ms(250);
	}

}

void initialize(void)
{
	
	PORTC.PIN4CTRL = PORT_PULLUPEN_bm;	// On-board pushbutton is on PC4

	PORTB.OUTSET = PIN7_bm;				// LED is on PB7
	PORTB.DIRSET = PIN7_bm;				// Start with it HIGH

	PORTA.OUTCLR = PIN2_bm;				// Start with it LOW
	PORTA.DIRSET = PIN2_bm;				// Shutter open sensor is on PA7

	PORTA.OUTCLR = PIN1_bm;				// Start with it LOW
	PORTA.DIRSET = PIN1_bm;				// Shutter closed sensor is on PA7

}

void open(void)
{

	PORTA.OUTCLR = PIN2_bm;
	PORTA.OUTSET = PIN1_bm;
//	on_LED;

}

void close(void)
{

	PORTA.OUTCLR = PIN1_bm;
	PORTA.OUTSET = PIN2_bm;
//	off_LED;

}