/*------------------------------------------------------------------------------
hicurrent.c
	Routines for handling the high current circuits with an MCP23008 port
	expander.
------------------------------------------------------------------------------*/
#ifndef HICURRENTC
#define HICURRENTC
								// High current outputs. Bits 0 and 4 not used
//#define HICURRENT		(0x48)	// MCP23008 address for high current driver
#define HICURRENT		(0x40)	// MCP23008 address for high current driver
#define SHUTTEROPEN		(0x02)	// Bit 1 is shutter open
#define LEFTOPEN		(0x04)	// Bit 2 is left Hartmann open
#define RIGHTOPEN		(0x08)	// Bit 3 is right Hartmann open
#define SHUTTERCLOSE	(0x20)	// Bit 5 is shutter close
#define LEFTCLOSE		(0x40)	// Bit 6 is left Hartmann close
#define RIGHTCLOSE		(0x80)	// Bit 7 is right Hartmann close

// Function prototypes
void init_hicurrent(void);

void init_hicurrent(void)
{
	write_MCP23008(HICURRENT, IODIR, 0x00);		// Set all pins as output
	write_MCP23008(HICURRENT, OLAT, 0x00);		// Set all bits low
	write_MCP23008(HICURRENT, OLAT, 0xFF);
}

#endif