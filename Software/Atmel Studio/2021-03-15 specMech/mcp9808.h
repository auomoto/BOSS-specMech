#ifndef MCP9808H
#define MCP9808H

#define MCP9808ADDR		(0x18)	// TWI address
#define TEMPREGISTER	(0x05)	// Ambient temperature register

uint8_t read_MCP9808(float*);

#endif