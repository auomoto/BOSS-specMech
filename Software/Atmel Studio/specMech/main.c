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
	squelchErrors = YES;

	CPU_CCP = CCP_IOREG_gc;
	CLKCTRL.MCLKCTRLB = CLKCTRL_PDIV_2X_gc | CLKCTRL_PEN_bm;
	CLKCTRL.MCLKCTRLB = CLKCTRL_LOCKEN_bm;

	initialize();
	squelchErrors = NO;

	for (;;) {
		if (recv0_buf.done) {
			recv0_buf.done = NO;
			commands();
		}

		if (timerOLED > timeoutOLED) {	// Display timeout
			squelchErrors = YES;
			clear_OLED(0);
			clear_OLED(1);
			timerOLED = 0;
			squelchErrors = NO;
		}

		if ((timerSAVEENCODER > timeoutSAVEENCODER) && rebootackd) {
			squelchErrors = YES;
			put_FRAM_ENCODERS();	// Maybe set a global error variable?
			timerSAVEENCODER = 0;
			squelchErrors = NO;
		}
	}

}
