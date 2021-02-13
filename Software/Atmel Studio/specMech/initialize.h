#ifndef INITIALIZEH
#define INITIALIZEH

void init_PORTS(void);
void init_LED(void);
void init_BEEPER(void);
void init_SPECID(void);
void init_EEPROM(void);
void init_USART(void);
void init_TWI(void);
uint8_t init_MMA8451(void);
uint8_t init_PNEU(void);
void init_OLED(uint8_t);
void init_RTC(uint16_t);	// Also called in commands.c

#endif