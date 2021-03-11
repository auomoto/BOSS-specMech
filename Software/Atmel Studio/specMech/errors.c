#include "globals.h"
#include "usart.h"
#include "specID.h"
#include "commands.h"
#include "nmea.h"
#include "errors.h"

/*------------------------------------------------------------------------------
void printError(uint8_t errorNumber, char *errorString)
	Prints an error report on USART0
------------------------------------------------------------------------------*/
void printError(uint16_t errorNumber, char *errorString)
{

	char strbuf[BUFSIZE];
//	const char errorFormat[] = "$S%dERR,%d,%s";
	const char errorFormat[] = "ERR,%d,%s";

	sprintf(strbuf, errorFormat, errorNumber, errorString);
//	checksum_NMEA(strbuf);
//	send_USART(0, (uint8_t*) strbuf, strlen(strbuf));
	printLine(strbuf);

}
