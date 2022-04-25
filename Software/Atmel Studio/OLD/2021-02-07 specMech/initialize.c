#include "globals.h"

void initialize(void)
{

	// Curiosity nano on-board button
	PORTF.PIN6CTRL = PORT_PULLUPEN_bm | PORT_ISC_FALLING_gc;
	init_PORTS();
	init_LED();
	init_BEEPER();
	init_RTC(64);	// Fast flashing LED before reboot ACK
	init_SPECID();
	init_TWI();
	init_PNEU();
	init_USART();
	init_OLED(0);
	init_OLED(1);
	init_EEPROM();
	init_MMA8451();

}

