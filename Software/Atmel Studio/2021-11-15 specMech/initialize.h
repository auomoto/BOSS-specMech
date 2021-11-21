/*
#ifndef INITIALIZEH
#define INITIALIZEH

//void init_BEEPER(void);
//void init_EEPROM(void);
void initialize0(void);
void initialize1(void);
//void init_LED(void);
//uint8_t init_MMA8451(void);
//void init_OLED(uint8_t);
//uint8_t init_PNEU(void);
//void init_PORTS(void);
//void init_PUSHBUTTON(void);
//void init_RTC(uint16_t);
//void init_SPECID(void);
//void init_TWI(void);
//void init_XPORT(void);
//void init_USART(void);

extern uint8_t rebootackd;

#endif
*/

#ifndef INITIALIZEH
#define INITIALIZEH


void init_BEEPER(void);
void init_EEPROM(void);
void initialize0(void);
void initialize1(void);
void init_LED(void);
uint8_t init_MMA8451(void);
void init_OLED(void);
uint8_t init_PNEU(void);
void init_PORTS(void);
void init_RTC(uint16_t);
void init_SPECID(void);
void init_TWI(void);
void init_XPORT(void);
void init_USART(void);

extern uint8_t rebootackd;

#endif