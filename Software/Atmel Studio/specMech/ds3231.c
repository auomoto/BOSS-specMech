/*------------------------------------------------------------------------------
ds3231.c
	TWI routines for the DS3231 day/time clock. Includes routines to convert
	DS3231 time format to ISO time (YYYY-MM-DDTHH:MM:SS) and vice versa.

	The first two digits of the year are assumed always to be "20" since this
	simplifies reading and writing the registers.
------------------------------------------------------------------------------*/

#include "globals.h"
#include "twi.h"
#include "errors.h"
#include "ds3231.h"

uint8_t check_isotime(char *isotime)
{

	char sbuf[10], strbuf[80];
	uint8_t i;
	int year, month, day, hours, minutes, seconds;
	const char msg0[] = "check_isotime: bad year";
	const char msg1[] = "check_isotime: bad month";
	const char msg2[] = "check_isotime: bad day";
	const char msg3[] = "check_isotime: bad hour";
	const char msg4[] = "check_isotime: bad minutes";
	const char msg5[] = "check_isotime: bad seconds";

	for (i = 0; i < 4; i++) {
		sbuf[i] = isotime[i];
	}
	sbuf[4] = '\0';
	year = atoi(sbuf);
	if ((year < 2021) || year > 2030) {
		sprintf(strbuf, msg0);
		printError(ERR_ISO, strbuf);
		return(ERROR);
	}
	for (i = 5; i < 7; i++) {
		sbuf[i-5] = isotime[i];
	}
	sbuf[2] = '\0';
	month = atoi(sbuf);
	if ((month < 1) || (month > 12)) {
		sprintf(strbuf, msg1);
		printError(ERR_ISO, strbuf);
		return(ERROR);
	}
	for (i = 8; i < 10; i++) {
		sbuf[i-8] = isotime[i];
	}
	sbuf[2] = '\0';
	day = atoi(sbuf);
	if ((day < 1) || (day > 31)) {
		sprintf(strbuf, msg2);
		printError(ERR_ISO, strbuf);
		return(ERROR);
	}
	for (i = 11; i < 13; i++) {
		sbuf[i-11] = isotime[i];
	}
	sbuf[2] = '\0';
	hours = atoi(sbuf);
	if ((hours < 0) || (hours > 23)) {
		sprintf(strbuf, msg3);
		printError(ERR_ISO, strbuf);
		return(ERROR);
	}
	for (i = 14; i < 16; i++) {
		sbuf[i-14] = isotime[i];
	}
	sbuf[2] = '\0';
	minutes = atoi(sbuf);
	if ((minutes < 0) || (minutes > 59)) {
		sprintf(strbuf, msg4);
		printError(ERR_ISO, strbuf);
		return(ERROR);
	}
	for (i = 17; i < 19; i++) {
		sbuf[i-17] = isotime[i];
	}
	sbuf[2] = '\0';
	seconds = atoi(sbuf);
	if ((seconds < 0) || (seconds > 59)) {
		sprintf(strbuf, msg5);
		printError(ERR_ISO, strbuf);
		return(ERROR);
	}
	return(NOERROR);
}

/*------------------------------------------------------------------------------
void convert_ds2iso(char *isotime, uint8_t *ds3231time)
	Convert the ds3231 time registers to ISO format.

	Input:
		ds3231time - array with first 7 ds3231 registers

	Output:
		isotime - 20 character array with the UTC time in ISO format:
			YYYY-MM-DDThh:mm:ss
------------------------------------------------------------------------------*/
void convert_ds2iso(char *isotime, uint8_t *ds3231time)
{

	const char format_ISO[] = "20%02x-%02x-%02xT%02x:%02x:%02x";
	uint8_t year, month, date, hours, minutes, seconds;

	seconds = (*ds3231time++);
	minutes = (*ds3231time++);
	hours = (*ds3231time++);
	date = (*ds3231time++);		// dummy assignment for day of week
	date = (*ds3231time++);
	month = (*ds3231time++);
	year = (*ds3231time);

	sprintf(isotime, format_ISO, year, month, date, hours, minutes, seconds);

}

/*------------------------------------------------------------------------------
void convert_iso2ds(uint8_t (uint8_t *ds3231time, char *isotime)
	Creates a 7-byte ds3231time array.

	ISO time format is YYYY-MM-DDThh:mm:ss
------------------------------------------------------------------------------*/
void convert_iso2ds(uint8_t *ds3231time, char *isotime)
{

	ds3231time[0] = (((isotime[17]-48) << 4) | (isotime[18]-48));	// sec
	ds3231time[1] = (((isotime[14]-48) << 4) | (isotime[15]-48));	// min
	ds3231time[2] = (((isotime[11]-48) << 4) | (isotime[12]-48));	// hr
	ds3231time[3] = 1;												// day of wk
	ds3231time[4] = (((isotime[8]-48) << 4) | (isotime[9]-48));		// date
	ds3231time[5] = (((isotime[5]-48) << 4) | (isotime[6]-48));		// mon
	ds3231time[6] = (((isotime[2]-48) << 4) | (isotime[3]-48));		// yr

}

/*------------------------------------------------------------------------------
uint8_t get_time(char *isotime)
	Reads the DS3231 clock and returns the time in ISO format.

	Input
		None

	Output
		isotime: YYYY-MM-DDThh:mm:ss format (array size must be 20 or larger

	Returns
		0 on success or TWI error
------------------------------------------------------------------------------*/
uint8_t get_time(char *isotime)
{

	uint8_t ds3231time[7];

	if (read_DS3231(DS3231ADDR, ds3231time) == ERROR) {
		printError(ERR_GETTIME, "get_time DS3231");
		strcpy(isotime, "DateTime?");
		return(ERROR);
	}

	convert_ds2iso(isotime, ds3231time);
	return(NOERROR);

}

/*------------------------------------------------------------------------------
uint8_t put_time(char *isotime)
	Converts a time in ISO format to DS3231 clock format and writes to the
	DS3231 clock

	Input
		*isotime - array with the 7-bytes of DS3231 time data

	Output
		None

	Returns
		0 on success or TWI error
------------------------------------------------------------------------------*/
uint8_t put_time(char *isotime)
{

	uint8_t ds3231time[7];

	if (check_isotime(isotime) == ERROR) {
		printError(ERR_ISO, "put_time: check_isotime error");
		return(ERROR);
	}
	convert_iso2ds(ds3231time, isotime);
	if (write_DS3231(DS3231ADDR, ds3231time) == ERROR) {
		printError(ERR_PUTTIME, "put_time: write_DS3231 error");
		return(ERROR);
	}
	return(NOERROR);

}

/*------------------------------------------------------------------------------
uint8_t read_DS3231(uint8_t addr, uint8_t *ds3231time)
	Read the DS3231 day/time clock

	Input:
		addr: DS3231 TWI address

	Output:
		ds3231time: The 7 relevant registers in the DS3231 chip

	Returns:
		ERROR for TWI start error
		NOERROR otherwise

	NOTES
	This reads the 7 registers (0x00 to 0x06) in the DS3231 clock and puts the
	data into the ds3231time array. The ds3231time array must be at least 7
	bytes long.

	The ds3231time array will contain the seven time registers of the DS3231
	chip:

	|===================================|
	| ds3231time|  ds3231	|			|
	|  element	| register	| contents	|
	|-----------------------------------|
	|   [0]		|   00h		|  Seconds	|
	|	[1]		|	01h		|  Minutes	|
	|	[2]		|	02h		|   Hour	|
	|	[3]		|	03h		|	Day		|	(Day of week, Mon, Tue, etc.)
	|	[4]		|	04h		|	Date	|
	|	[5]		|	05h		|  Month	|
	|	[6]		|	06h		|	Year	|
	|===================================|

	The values are in BCD. When writing, bit 6 in the hours register should
	be clear (0) to indicate a 24 hour clock system.

	The clock time is UTC. Communications to the outside is via the ISO time
	format: YYYY-MM-DDThh:mm:ss (19 characters).
------------------------------------------------------------------------------*/
uint8_t read_DS3231(uint8_t addr, uint8_t ds3231time[])
{

	uint8_t i;

	if (start_TWI(addr, TWIWRITE) == ERROR) {
		stop_TWI();
		return(ERROR);
    }  

	if (write_TWI(0x00) == ERROR) {	// Register 00h
		stop_TWI();
		return(ERROR);
	}

	if (start_TWI(DS3231ADDR, TWIREAD) == ERROR) {
		stop_TWI();
		return(ERROR);
	}

	for (i = 0; i < 6; i++) {
		ds3231time[i] = read_TWI();
	}

	ds3231time[6] = readlast_TWI();

	stop_TWI();

	return(NOERROR);

}

/*------------------------------------------------------------------------------
uint8_t write_DS3231(uint8_t addr, char *ds3231time)
	Write the time in the DS3231 clock chip registers

	Inputs:
		addr: DS3231 TWI address
		ds3231time: pointer to an array with the 7 DS3231 register values

	Returns:
		ERROR for TWI start error
		NOERROR otherwise
------------------------------------------------------------------------------*/
uint8_t write_DS3231(uint8_t addr, uint8_t *ds3231time)
{

	uint8_t i;

	if (start_TWI(addr, TWIWRITE) == ERROR) {
		stop_TWI();
		return(ERROR);
	}

	if (write_TWI(0x00) == ERROR) {	// Start with register 0x00 (seconds)
		stop_TWI();
		return(ERROR);
	}

	for (i = 0; i < 7; i++) {
		if (write_TWI(*ds3231time++) == ERROR) {	// send the new time
			stop_TWI();
			return(ERROR);
		}
	}
	stop_TWI();

	return(NOERROR);

}
