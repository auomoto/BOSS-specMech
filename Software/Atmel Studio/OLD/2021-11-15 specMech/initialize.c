#include "globals.h"
#include "oled.h"
#include "eeprom.h"
#include "roboclaw.h"
#include "initialize.h"
//#include "specID.h"

uint8_t rebootackd;

/*
void initialize(void)
{

	char versionstr[11];

	init_PORTS();
	PORTF.PIN6CTRL = PORT_PULLUPEN_bm | PORT_ISC_FALLING_gc;	// Pushbutton setup
	init_XPORT();
	init_SPECID();
	init_LED();
	init_BEEPER();
	init_TWI();
	init_RTC(32);	// Fast, 1/16 sec, for blinking LED at startup
	init_USART();

	init_ROBO();
	init_MMA8451();	// requires TWI interrupts
	init_PNEU();	// requires TWI interrupts
	init_OLED(0);
	init_OLED(1);
	init_EEPROM();	// Needs TWI for get_time()
	timeoutOLED = 30 * (512/32);	// 30 seconds for version display
	get_VERSION(versionstr);
	writestr_OLED(1,"specMech Version", 1);
	writestr_OLED(1, versionstr, 2);

}
*/

void initialize0(void)
{

	init_PORTS();
	init_XPORT();
//	init_BEEPER();
	init_TWI();
	init_RTC(32);	// Fast, 1/16 sec, for blinking LED at startup
	init_USART();

}

void initialize1(void)
{

	char versionstr[11];

	rebootackd = NO;
//	rebootackd = YES;
	init_MOTORS();
	init_MMA8451();
	init_PNEU();
	init_OLED();
	init_EEPROM();	// Needs TWI
	timerOLED = 0;
	timeoutOLED = 30 * (512/32);	// 30 seconds for version display
	get_VERSION(versionstr);
	writestr_OLED(0,"specMech Version", 1);
	writestr_OLED(0, versionstr, 2);

}
