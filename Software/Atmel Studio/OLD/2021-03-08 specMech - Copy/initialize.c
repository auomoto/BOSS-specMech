#include "globals.h"
#include "initialize.h"
#include "oled.h"
#include "eeprom.h"

void initialize(void)
{

	char versionstr[11];

	init_PORTS();
	PORTF.PIN6CTRL = PORT_PULLUPEN_bm | PORT_ISC_FALLING_gc;	// Pushbutton setup
	init_XPORT();
	init_SPECID();
	init_LED();
	init_BEEPER();
	init_RTC(32);	// Fast, 1/16 sec, for blinking LED at startup
	init_USART();
	init_TWI();
	init_MMA8451();
	init_PNEU();
	init_OLED(0);
	init_OLED(1);
	init_EEPROM();	// Needs TWI for get_time()
	timeoutOLED = 30 * (512/32);	// 30 seconds for version display
	get_VERSION(versionstr);
	writestr_OLED(1,"specMech Version", 1);
	writestr_OLED(1, versionstr, 2);

}

