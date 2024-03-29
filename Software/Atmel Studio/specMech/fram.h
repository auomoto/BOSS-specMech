#ifndef FRAMH
#define FRAMH

#define FRAMTWIADDR		(0x50)	// TWI address with A0, A1, A2 grounded
#define SETTIMEADDR		(0x00)	// When the day/time clock was set (20 bytes)
#define SETTIMEFRAM		(0)		// Time that the DS3231 clock was set
#define ISOTIMELEN		(20)	// 20 bytes in an ISO time string
#define ENCA_FRAMADDR	(20)	// Motor A encoder value (4 bytes)
#define ENCB_FRAMADDR	(24)	// Motor B encoder value (4 bytes)
#define ENCC_FRAMADDR	(28)	// Motor C encoder value (4 bytes)
#define ENC_SAVETIME	(32)	// Encoder save time (20 bytes ISO time)

uint8_t get_SETTIME(char *lastsettime);
uint8_t read_FRAM(uint8_t, uint16_t, uint8_t *, uint8_t);
uint8_t write_FRAM(uint8_t, uint16_t, uint8_t *, uint8_t);

#endif