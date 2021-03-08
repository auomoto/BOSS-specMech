#ifndef DS3231H
#define DS3231H

#define DS3231ADDR	(0x68)	// Day/Time clock TWI address

void convert_ds2iso(char*, uint8_t*);
void convert_iso2ds(uint8_t *, char*);
uint8_t get_time(char*);
uint8_t put_time(char*);
uint8_t read_DS3231(uint8_t, uint8_t*);
uint8_t uint8_to_bcd(uint8_t);
uint8_t write_DS3231(uint8_t, uint8_t*);

#endif /* DS3231H */