#ifndef FRAMH
#define FRAMH

#define FRAMADDR	(0x50)	// A0, A1, A2 grounded
#define SETTIMEADDR	(0x00)	// When the day/time clock was set (20 bytes)

uint8_t read_FRAM(uint8_t, uint16_t, uint8_t *, uint8_t);
uint8_t write_FRAM(uint8_t, uint16_t, uint8_t *, uint8_t);

#endif /* FRAMH */