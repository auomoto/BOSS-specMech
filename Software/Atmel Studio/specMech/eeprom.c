#ifndef EEPROMH
#define EEPROMH
#endif

#include <avr/eeprom.h>

// EEPROM defines
#define VERSIONADDR		(0)		// Version string must have exactly 11 bytes
#define VERSIONSIZE		(11)	// Date as in YYYY-DD-MM
#define BOOTTIMEADDR	(11)	// Boot time has exactly 21 bytes
#define BOOTTIMESIZE	(21)	// ISO date format, nearest second

// Function Prototypes
void get_BOOTTIME(char *);
void get_VERSION(char *);
void init_EEPROM(void);
void update_BOOTTIME(void);
void update_VERSION(void);

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

//	get_time(boottime);
	eeprom_update_block((const void *)boottime, (void *)BOOTTIMEADDR, BOOTTIMESIZE);

}

void update_VERSION(void)
{

	char version[11];

	strcpy(version, VERSION);
	eeprom_update_block((const void *)version, (void *)VERSIONADDR, VERSIONSIZE);

}
