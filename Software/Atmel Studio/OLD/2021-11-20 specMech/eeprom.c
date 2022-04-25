#include "globals.h"
#include "ds3231.h"
#include "eeprom.h"

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
