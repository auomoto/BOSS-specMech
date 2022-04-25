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

#include "fram.h"	// remove after testing

ParsedCMD pcmd[CSTACKSIZE];	// Split the command line into its parts

uint8_t firstpass;

int main(void)
{

	firstpass = YES;		// Set to NO in commands.c
	squelchErrors = YES;
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
		} if ((timerSAVEENCODER > timeoutSAVEENCODER) && rebootackd) {
			squelchErrors = YES;

char str[80];
uint8_t tbuf[4];
int32_t encoderValue;

get_MOTOREncoder(128, ROBOREADENCODERCOUNT, &encoderValue);
tbuf[0] = (encoderValue >> 24) & 0xFF;
tbuf[1] = (encoderValue >> 16) & 0xFF;
tbuf[2] = (encoderValue >> 8) & 0xFF;
tbuf[3] = encoderValue & 0xFF;
write_FRAM(FRAMTWIADDR, ENCAFRAMADDR, tbuf, 4);
sprintf(str, "saved encval = %ld", encoderValue);
printLine(str);

/*
read_FRAM(FRAMTWIADDR, ENCAFRAMADDR, tbuf, 4);
encoderValue =  (uint32_t) tbuf[0] << 24;
encoderValue |= (uint32_t) tbuf[1] << 16;
encoderValue |= (uint32_t) tbuf[2] << 8;
encoderValue |= (uint32_t) tbuf[3];
sprintf(str, "saved and re-read encval = %ld", encoderValue);
printLine(str);
*/


/*
encoderValue = 250000;
set_MOTOREncoder(128, encoderValue);
encoderValue = 0;
get_MOTOREncoder(128, ROBOREADENCODERCOUNT, &encoderValue);
sprintf(str, "encval = %ld", encoderValue);
printLine(str);
*/

//			saveFRAM_MOTOREncoders();
			timerSAVEENCODER = 0;
			squelchErrors = NO;
		}
	}
}
