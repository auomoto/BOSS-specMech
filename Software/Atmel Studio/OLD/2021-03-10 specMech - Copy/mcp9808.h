#ifndef MCP9808H
#define MCP9808H

#define MCP9808ADDR		(0x18)	// TWI address
#define TEMPREGISTER	(0x05)	// Ambient temperature register

float read_MCP9808(void);

#endif /* MCP9808H */