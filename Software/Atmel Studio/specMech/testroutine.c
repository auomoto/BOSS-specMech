#include "globals.h"
#include "roboclaw.h"

void testroutine(void)
{

	uint8_t addr;	// TWI address for MCP23008
	char scratchbuf[80];
	float volts;
/*
	volts = get_ROBOVoltage(128);
	sprintf(scratchbuf, "%4.1f volts\r\n", volts);
	send_USART(0, (uint8_t*) scratchbuf, strlen(scratchbuf));
*/
/*
	addr = 0x20;
	write_MCP23008(addr, IODIR, 0x00);
	write_MCP23008(addr, OLAT, 0x00);
	_delay_ms(1000);
	write_MCP23008(addr, OLAT, 0xFF);
	send_USART(0, (uint8_t*) buf, strlen(buf));
*/
	return;
}