#include "globals.h"
#include "beeper.h"

void beeper(char *message)
{

	uint8_t i, len;

	len = strlen(message);
	for (i = 0; i < len; i++) {
		switch (*message++) {

			case '.':
				on_BEEPER;
//				PORTB.OUTSET = PIN2_bm;
				_delay_ms(75);
				break;

			case '-':
				on_BEEPER;
//				PORTB.OUTSET = PIN2_bm;
				_delay_ms(225);
				break;

			case ' ':
				_delay_ms(225);
				break;

			default:
				break;
		}

		off_BEEPER;
//		PORTB.OUTCLR = PIN2_bm;
		_delay_ms(75);

	}
}