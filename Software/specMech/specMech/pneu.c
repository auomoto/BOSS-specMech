#ifndef PNEUC
#define PNEUC
#endif

#define PNEUPWR	(0x40)		// Pneumatic valve power port
#define SHTROPEN_bm	0x01	// Bit maps of pins
#define SHTRCLOS_bm	0x02
#define LEFTOPEN_bm	0x04
#define LEFTCLOS_bm	0x08
#define RITEOPEN_bm	0x10
#define RITECLOS_bm	0x20

#define PNEUPORT	(0x40)		// MCP23008 that controls the pneumatic poppet valves

// Function Prototypes
void openclose(uint8_t);

void openclose(uint8_t thing)
{

	// Read the GPIO port
	// flip the bits
	// Write to OLAT port

}