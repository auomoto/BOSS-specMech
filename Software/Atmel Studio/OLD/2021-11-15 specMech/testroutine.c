#include "globals.h"
#include "timers.h"
#include "usart.h"
#include "roboclaw.h"
#include "fram.h"
#include "testroutine.h"

void testroutine(void)
{


	init_MOTORS();

/*
	char buf[25];
	uint8_t bufb[8];
	int32_t value;
	saveFRAM_MOTOREncoders();
//	putFRAM_MOTOREncoder(MOTORAADDR);
	read_FRAM(FRAMTWIADDR, ENCAFRAMADDR, bufb, 4);
	value = (uint32_t) bufb[0] << 24;
	value |= (uint32_t) bufb[1] << 16;
	value |= (uint32_t) bufb[2] << 8;
	value |= (uint32_t) bufb[3];
	value /= ROBOCOUNTSPERMICRON;
	sprintf(buf, "value = %ld\r\n", value);
	send_USART(0, (uint8_t*) buf, strlen(buf));
*/
/*
	char buf[50];
	uint8_t bufb[10];
	int32_t value;

	sprintf(buf, "controller = %d\r\n", MOTORAADDR);
	send_USART(0, (uint8_t*) buf, strlen(buf));
	buf[0] = buf[1] = buf[2] = 0;
	buf[3] = 63;
	write_FRAM(FRAMTWIADDR, ENCAFRAMADDR, (uint8_t*) buf, 4);
	read_FRAM(FRAMTWIADDR, ENCAFRAMADDR, bufb, 4);
	value = bufb[0] << 24;
	value |= bufb[1] << 16;
	value |= bufb[2] << 8;
	value |= bufb[3];
	sprintf(buf, "value = %ld\r\n", value);
	send_USART(0, (uint8_t*) buf, strlen(buf));
*/

/*
	start_TCB0(1);
	_delay_ms(100);	// this reports 85 or 86 ticks????????????
	stop_TCB0();
	sprintf(buf, "ticks = %d\r\n", ticks);
	send_USART(0, (uint8_t*) buf, strlen(buf));
*/

/*
	uint8_t addr;	// TWI address for MCP23008
	char scratchbuf[80];
	float volts;

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