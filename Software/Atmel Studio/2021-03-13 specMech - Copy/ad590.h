#ifndef AD590H
#define AD590H

#define AD590DRIVER		(0x27)		// MCP23008 address (selects which sensor)
#define AD590RESISTOR	(1000.0)	// Ohms

uint8_t read_AD590(uint8_t, float*);

#endif /* AD590H */