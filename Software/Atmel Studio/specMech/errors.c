#include "globals.h"
#include "usart.h"
#include "specID.h"
#include "commands.h"
#include "nmea.h"
#include "errors.h"

uint8_t squelchErrors;

/*------------------------------------------------------------------------------
void printError(uint8_t errorNumber, char *errorString)
	Prints an error report on USART0
------------------------------------------------------------------------------*/
void printError(uint16_t errorNumber, char *errorString)
{

	char strbuf[BUFSIZE];
	const char errorFormat[] = "ERR,%d,%s";

	if (!squelchErrors) {
		sprintf(strbuf, errorFormat, errorNumber, errorString);
		printLine(strbuf);
	}

}
