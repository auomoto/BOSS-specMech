/*------------------------------------------------------------------------------
specMech.c
	BOSS motion controller based on a Microchip Curiosity Nano with an
	ATMega4809 microcontroller.
------------------------------------------------------------------------------*/

#include "globals.h"
#include "errors.h"
#include "initialize.h"
#include "usart.h"
#include "roboclaw.h"
#include "oled.h"
#include "commands.h"

ParsedCMD pcmd[CSTACKSIZE];	// Split the command line into its parts

uint8_t firstpass;

int main(void)
{

	firstpass = YES;		// Set to NO in commands.c
	squelchErrors = YES;	// Forbids messages during startup

	// Change clock speed to 10 MHz
	CPU_CCP = CCP_IOREG_gc;
	CLKCTRL.MCLKCTRLB = CLKCTRL_PDIV_2X_gc | CLKCTRL_PEN_bm;
	CLKCTRL.MCLKCTRLB = CLKCTRL_LOCKEN_bm;

	initialize();
	squelchErrors = NO;

	for (;;) {
		// Check for incoming command
		if (recv0_buf.done) {
			recv0_buf.done = NO;
			commands();
		}

		// Display timeout
		if (timerOLED > timeoutOLED) {
			squelchErrors = YES;
			clear_OLED(0);
			clear_OLED(1);
			timerOLED = 0;
			squelchErrors = NO;
		}

		// Save motor encoder values to FRAM
		if ((timerSAVEENCODER > timeoutSAVEENCODER) && rebootackd) {
			squelchErrors = YES;
			put_FRAM_ENCODERS();	// Maybe set a global error variable?
			timerSAVEENCODER = 0;
			squelchErrors = NO;
		}
	}

}
