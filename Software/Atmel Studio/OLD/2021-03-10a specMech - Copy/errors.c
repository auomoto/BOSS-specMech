#include "globals.h"
#include "usart.h"
#include "specID.h"
#include "nmea.h"
#include "errors.h"

/*------------------------------------------------------------------------------
void printError(uint8_t errorNumber, char *errorString)
	Prints an error report on USART0
------------------------------------------------------------------------------*/
void printError(uint16_t errorNumber, char *errorString)
{

	char strbuf[81];
	const char errorFormat[] = "$S%dERR,%d,%s";

	sprintf(strbuf, errorFormat, get_SPECID, errorNumber, errorString);
	checksum_NMEA(strbuf);
	send_USART(0, (uint8_t*) strbuf, strlen(strbuf));

}
