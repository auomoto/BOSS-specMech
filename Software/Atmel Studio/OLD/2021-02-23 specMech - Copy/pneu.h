#ifndef PNEUH
#define PNEUH

#define PNEUSENSORS		(0x21)	// TWI address for pneumatic cylinder sensors
#define HIGHCURRENT		(0x24)	// High current driver for pneumatic valves
#define SHUTTERBM		(0x22)	// OR existing value with this first, then
#define SHUTTEROPEN		(0xCE)	// AND with this pattern to open
#define SHUTTERCLOSE	(0xEC)	// AND with this pattern to close
#define LEFTBM			(0x44)	// OR existing value with this, then
#define LEFTOPEN		(0xAE)	// AND with this pattern to open
#define LEFTCLOSE		(0xEA)	// AND with this pattern to close
#define RIGHTBM			(0x88)	// OR existing value with this, then
#define RIGHTOPEN		(0x6E)	// AND with this pattern to open
#define RIGHTCLOSE		(0xE6)	// AND with this pattern to close


uint8_t set_PNEUVALVES(uint8_t, uint8_t);

#endif