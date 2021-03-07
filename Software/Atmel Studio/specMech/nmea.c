#include "globals.h"
#include "usart.h"
#include "nmea.h"

/*------------------------------------------------------------------------------
void checksum_NMEA(char *str)
	Append checksum to a string
------------------------------------------------------------------------------*/
void checksum_NMEA(char *str)
{
	const char format_CSM[] = "*%02X\r\n";

	char chksum[6];
	uint8_t i, nchar, checksum;

	checksum = 0;
	nchar = strlen(str);
	for (i = 1; i < nchar; i++) {
		checksum ^= str[i];
	}
	sprintf(chksum, format_CSM, checksum);
	strcat(str, chksum);

}

// Move this to report.c? Add command id counter (after the ;)
/*------------------------------------------------------------------------------
void format_ERR(char *str)
	Returns an error line
------------------------------------------------------------------------------*/
void printError(uint8_t errorVal, char *string)
{

	char strbuf[81];
	const char errformat[] = "$S%dERR,%d,%s";

	sprintf(strbuf, errformat, get_SPECID, errorVal, string);
	checksum_NMEA(strbuf);
	send_USART(0, (uint8_t*) strbuf, strlen(strbuf));

}
