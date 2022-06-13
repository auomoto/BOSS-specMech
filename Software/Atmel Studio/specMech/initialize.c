#include "globals.h"
#include "initialize.h"

uint8_t rebootackd;

void initialize(void)
{

	char versionstr[11];

	rebootackd = NO;
	timerOLED = 0;					// See oled.h
	timeoutOLED = 30 * (512/32);	// 30 seconds for version display; fast until !

	init_PORTS();
	init_TWI();
	init_PNEU();
	init_RTC(32);	// Fast, 1/16 sec, for blinking LED at startup
	init_MMA8451();	// Accelerometer; needs TWI
	init_OLED();	// Needs TWI
	init_EEPROM();	// Needs TWI to get boot time from clock
	init_PSVOLTS_ADC();
	init_USART();
//	start_TCB0(1);	// 1 ms ticks for TWI & USART timeouts
	sei();
	init_MOTORS();	// Needs USART & interrupts on
	get_VERSION(versionstr);
	writestr_OLED(0,"specMech Version", 1);
	writestr_OLED(0, versionstr, 2);
	beeper(".... ..");

}
