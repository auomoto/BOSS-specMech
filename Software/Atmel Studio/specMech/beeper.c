#include "globals.h"
#include "beeper.h"

void beeper(char *message)
{

	uint8_t i, len;

	len = strlen(message);
	for (i = 0; i < len; i++) {
		switch (*message++) {
			case '.':
				PORTB.OUTSET = PIN2_bm;
				_delay_ms(75);
				break;
			case '-':
				PORTB.OUTSET = PIN2_bm;
				_delay_ms(225);
				break;
			case ' ':
				_delay_ms(225);
				break;
			default:
				break;
		}
		PORTB.OUTCLR = PIN2_bm;
		_delay_ms(75);
	}

/*
	PORTB.OUTSET = PIN2_bm;
	if (duration == 0) {
		_delay_ms(50);
	} else {
		_delay_ms(150);
	}
	PORTB_OUTCLR = PIN2_bm;
	_delay_ms(50);
*/
}