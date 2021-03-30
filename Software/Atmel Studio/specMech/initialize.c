#include "globals.h"
#include "ports.h"			// ATMega4809 I/O pins
#include "pushbutton.h"		// Pin PF6
#include "led.h"			// Pin PF5
#include "specID.h"			// Pin PF2
#include "beeper.h"			// Pin PB2
#include "twi.h"			// ATMega4809 TWI peripheral
#include "rtc.h"			// ATMega4809 32.768 kHz clock
#include "usart.h"			// ATMega4809 USART peripherals
#include "mma8451.h"		// MMA8451 accelerometer
#include "pneu.h"			// Pneumatic actuators and sensors
#include "eeprom.h"			// ATMega4809 EEPROM
#include "oled.h"			// Newhaven NHD-0216AW-1B3 display
#include "roboclaw.h"		// Collimator motor controller
#include "xport.h"			// Lantronix XPort
#include "initialize.h"

uint8_t rebootackd;

/*------------------------------------------------------------------------------
void initialize0(void)
	Initialize things that don't need interrupts. In particular, things that
	use TWI or the USARTs require interrupts because they have timeouts derived
	from a timer interrupt.
------------------------------------------------------------------------------*/
void initialize0(void)
{

	init_PORTS();		// Start in a harmless state
	init_PUSHBUTTON();	// On-board pushbutton
	init_LED();			// On-board LED
	init_SPECID();		// SPECID jumper
	init_BEEPER();		// Just an MCU pin
	init_TWI();			// Sets up the device and its baud rate
	init_RTC(32);		// 32=Fast, 1/16 sec, for blinking LED at startup
	init_USART();		// Sets up the devices and global I/O buffers

}

/*------------------------------------------------------------------------------
void initialize1(void)
	Interrupts must be enabled for these because they use either the TWI
	interface (the start routine has a timer timeout) or the USART interface
	(again, a timeout) for setup or data retrieval.
------------------------------------------------------------------------------*/
void initialize1(void)
{

	char versionstr[11];

	rebootackd = NO;
//	init_MOTORS();
	init_MMA8451();					// Accelerometer TWI has a timeout
	init_PNEU();					// GMR sensors through an MCP23008
	init_EEPROM();					// Needs TWI b/c it reads the DS3231 clock
	init_OLED(0);					// OLED TWI display has a timeout
	init_OLED(1);					// OLED TWI display has a timeout
	init_MOTORS();					// Uses USART0 with a timeout - do this late for powerup?
	init_XPORT();					// This resets the hardware (on the next board version)

//	timeoutOLED = 30 * (512/32);	// 30 seconds for version display before rebootackd
	// Display the version
	get_VERSION(versionstr);
	writestr_OLED(1,"specMech Version", 1);
	writestr_OLED(1, versionstr, 2);

}


