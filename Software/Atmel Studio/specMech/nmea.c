#ifndef NMEAC
#define NMEAC

/*------------------------------------------------------------------------------
NMEA checksum computations
	These routines append the NMEA style checksums to the presented character
	string.
------------------------------------------------------------------------------*/
// Function Prototypes
void checksum_NMEA(char*);
uint8_t get_specID(void);

void checksum_NMEA(char *str)
{
	const char format_CSM[]="*%02X\r\n";

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

void get_ERR(char *str)
{

	const char err1[]="$S1ERR*27\r\n";
	const char err2[]="$S2ERR*24\r\n";

	if (get_specID() == 1) {
		strcpy(str, err1);
	} else {
		strcpy(str, err2);
	}

}

uint8_t get_specID(void)
{

	// Read the jumper instead of just returning 2
	return(2);

}

#endif
