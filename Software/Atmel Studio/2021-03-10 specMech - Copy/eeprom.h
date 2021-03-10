#ifndef EEPROMH
#define EEPROMH

#include <avr/eeprom.h>

#define VERSIONADDR		(0)		// Version string must have exactly 11 bytes
#define VERSIONSIZE		(11)	// Version is the date, as YYYY-DD-MM
#define BOOTTIMEADDR	(11)	// EEPROM address for boot time
#define BOOTTIMESIZE	(21)	// ISO date format, exactly 21 bytes

void get_BOOTTIME(char*);
void get_VERSION(char*);
void update_BOOTTIME(void);
void update_VERSION(void);

#endif /* EEPROMH */