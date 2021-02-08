#include "globals.h"

void initialize(void)
{

	init_PORTS();
	PORTF.PIN6CTRL = PORT_PULLUPEN_bm | PORT_ISC_FALLING_gc;
	init_SPECID();
	init_LED();
	init_BEEPER();
	init_RTC(64);	// Fast, for blinking the LED at startup
	init_USART();
	init_TWI();
	init_MMA8451();
	init_PNEU();
	init_OLED(0);
	init_OLED(1);
	init_EEPROM();	// Needs TWI for get_time()

}

