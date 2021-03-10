#ifndef TWIH
#define TWIH

#define TWIFREQ		100000UL		// Fastest allowed for F_CPU = 3.33 MHz
#define TWIBAUD		((uint8_t) (F_CPU/(2*TWIFREQ)) - 5)	// Ignore rise time
#define TWIWRITE	0
#define TWIREAD		1
#define TWIBUSERR	2
#define TWIARBLOST	3
#define TWINODEVICE	4

uint8_t read_TWI(void);
uint8_t readlast_TWI(void);
uint8_t start_TWI(uint8_t, uint8_t);
void stop_TWI(void);
uint8_t write_TWI(uint8_t);

#endif /* TWIH */