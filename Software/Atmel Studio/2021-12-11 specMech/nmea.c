#include "globals.h"
#include "usart.h"
//#include "specID.h"
#include "nmea.h"

/*------------------------------------------------------------------------------
void checksum_NMEA(char *str)
	Append checksum and \r\n to a string
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
