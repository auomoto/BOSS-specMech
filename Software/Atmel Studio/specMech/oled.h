#ifndef OLEDH
#define OLEDH

#define OLEDADDR0		(0x3C)		// TWI bus address
#define OLEDADDR1		(0x3D)		// TWI bus address
#define CLEARDISPLAY	0x01		// Newhaven command (not used)
#define DISPLAYON		0x0C		// Newhaven command (not used)
#define DISPLAYOFF		0x08		// Newhaven command (not used)
#define OLEDCMD			0x00		// Newhaven command was 1
#define OLEDDATA		0x40		// Newhaven command was 0
#define OLEDLINE1		0x80		// Newhaven command
#define OLEDLINE2		0xC0		// Newhaven command

void clear_OLED(uint8_t);
void init_OLED(uint8_t);
void write_OLED(uint8_t, uint8_t, uint8_t);
void writestr_OLED(uint8_t, char*, uint8_t);

extern uint16_t timerOLED, timeoutOLED;	// Used to blank the display

#endif