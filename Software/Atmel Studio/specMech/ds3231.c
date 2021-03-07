/*------------------------------------------------------------------------------
ds3231.c
	TWI routines for the DS3231 day/time clock. Includes routines to convert
	device time to ISO time (YYYY-MM-DDTHH:MM:SS) and vice versa.

	The first two digits of the year are assumed always to be "20" since this
	simplifies reading and writing the registers.
------------------------------------------------------------------------------*/

#include "globals.h"
#include "ds3231.h"
#include "twi.h"

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
	uint8_t year, month, date, hours, minutes, seconds;

	seconds = (*ds3231time++);
	minutes = (*ds3231time++);
	hours = (*ds3231time++);
	date = (*ds3231time++);		// dummy assignment for day of week
	date = (*ds3231time++);
	month = (*ds3231time++);
	year = (*ds3231time);

	sprintf(isotime, "20%02x-%02x-%02xT%02x:%02x:%02x",
		year, month, date, hours, minutes, seconds);

}

/*------------------------------------------------------------------------------
void convert_iso2ds(uint8_t (uint8_t *ds3231time, char *isotime)
	Creates a 7-byte ds3231time array.

	ISO time format is YYYY-MM-DDThh:mm:ssZ
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
		Time in a character array isotime. YYYY-MM-DDThh:mm:ssZ (size must be
		21 or larger)

	Returns
		0 on success or TWI error
------------------------------------------------------------------------------*/
uint8_t get_time(char *isotime)
{

	uint8_t retval, ds3231time[7];

	if ((retval = read_DS3231(DS3231ADDR, ds3231time))) {
		strcpy(isotime, "CurrentTime?");
		return(retval);
	}

	convert_ds2iso(isotime, ds3231time);
	return(0);

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

	uint8_t retval, ds3231time[7];

	convert_iso2ds(ds3231time, isotime);
	if ((retval = write_DS3231(DS3231ADDR, ds3231time))) {
		return(retval);
	}
	return(0);

}

/*------------------------------------------------------------------------------
uint8_t read_DS3231(uint8_t addr, uint8_t *ds3231time)
	Read the DS3231 day/time clock

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

	uint8_t i, retval;

	if ((retval = start_TWI(addr, TWIWRITE))) {
		stop_TWI();
		return(retval);
    }  

	if ((retval = write_TWI(0x00))) {	// Register 00h
		stop_TWI();
		return(retval);
	}
	if ((retval = start_TWI(DS3231ADDR, TWIREAD))) {
		stop_TWI();
		return(retval);
	}

	for (i = 0; i < 6; i++) {
		ds3231time[i] = read_TWI();
	}
	ds3231time[6] = readlast_TWI();

	stop_TWI();

	return(0);

}

/*------------------------------------------------------------------------------
uint8_t write_DS3231(uint8_t addr, char *ds3231time)
	Write the time in the DS3231 clock chip registers

	Inputs
		ds3231time is a pointer to an array of 7 DS3231 register values

	Returns
		TWI errors
------------------------------------------------------------------------------*/
uint8_t write_DS3231(uint8_t addr, uint8_t *ds3231time)
{

	uint8_t i, retval;

	if ((retval = start_TWI(addr, TWIWRITE))) {
		stop_TWI();
		return(retval);
	}

	if ((retval = write_TWI(0x00))) {	// Start with register 0x00 (seconds)
		stop_TWI();
		return(retval);
	}

	for (i = 0; i < 7; i++) {
		if ((retval = write_TWI(*ds3231time++))) {	// send the new time
			stop_TWI();
			return(retval);
		}
	}
	stop_TWI();

	return(0);

}
