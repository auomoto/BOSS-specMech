#include "globals.h"
#include <avr/eeprom.h>

#define VERSIONADDR		(0)		// Version string must have exactly 11 bytes
#define VERSIONSIZE		(11)	// Version is the date, as YYYY-DD-MM
#define BOOTTIMEADDR	(11)	// EEPROM address for boot time
#define BOOTTIMESIZE	(21)	// ISO date format, exactly 21 bytes

/*------------------------------------------------------------------------------
eeprom.c
	Reads and writes the specMech version and system boot time.
	TWI must be enabled first (init_TWI()) since it calls get_time().
------------------------------------------------------------------------------*/
void get_BOOTTIME(char *boottime)
{

	eeprom_read_block((void *)boottime, (const void *)BOOTTIMEADDR, BOOTTIMESIZE);

}

void get_VERSION(char *version)
{

	eeprom_read_block((void *)version, (const void *)VERSIONADDR, VERSIONSIZE);

}

void init_EEPROM(void)
{

	update_BOOTTIME();
	update_VERSION();

}

void update_BOOTTIME(void)
{

	char boottime[21];

	get_time(boottime);
	eeprom_update_block((const void *)boottime, (void *)BOOTTIMEADDR, BOOTTIMESIZE);

}

void update_VERSION(void)
{

	char version[11];

	strcpy(version, VERSION);
	eeprom_update_block((const void *)version, (void *)VERSIONADDR, VERSIONSIZE);

}
