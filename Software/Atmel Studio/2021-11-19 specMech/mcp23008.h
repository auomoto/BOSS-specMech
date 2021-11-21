#ifndef MCP23008H
#define MCP23008H

#define IODIR	(0x00)	// Pin direction; 1 for input, 0 for output
#define IPOL	(0x01)	// Pin polarity on GPIO; 1 for inverted, not used here
#define GPINTEN	(0x02)	// Interrupt on change; 1 to enable that pin, set DEFVAL & INTCON
#define DEFVAL	(0x03)	// Default comparison value for GPINTEN
#define INTCON	(0x04)	// Interrupt control; 1 compares w/DEFVAL, 0 for pin change
#define IOCON	(0x05)	// Set parameters (see page 15)
#define GPPU	(0x06)	// Pullups; 100K pullup if 1, tri-state if 0 (default)
#define INTF	(0x07)	// Interrupt flag (find out which pin caused the interrupt)
#define INTCAP	(0x08)	// GPIO state when interrupt occurred; cleared when GPIO is read
#define GPIO	(0x09)	// Read for input
#define OLAT	(0x0A)	// Write for output
#define MCP23008ERROR_bm	(0b00000001)	// bit 0 is an MCP23008 error

uint8_t read_MCP23008(uint8_t, uint8_t);
uint8_t write_MCP23008(uint8_t, uint8_t, uint8_t);

#endif