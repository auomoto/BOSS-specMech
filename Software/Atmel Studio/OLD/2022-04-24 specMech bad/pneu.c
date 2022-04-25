/*------------------------------------------------------------------------------
pneu.c
	Pneumatic valves and their sensors
------------------------------------------------------------------------------*/

#include "globals.h"
#include "pneu.h"

volatile uint8_t pneuState;

//NEED TO FIX ERROR RETURN SITUATION

/*------------------------------------------------------------------------------
uint8_t close_PNEU(char mech)
	Close the shutter or Hartmann doors

	Pneumatic cylinders move the shutter and Hartmann doors. Each cylinder is
	controlled by a pair of air valves, both of which must be commanded to
	move the shutter or door. One of the pair must be open and the other closed
	to make the item move.

	The valves are electronically activated via an MCP23008 port expander that
	is controlled by the set_PNEUVALVES routine in pneu.c.

	Input:
		mech - a character that selects the shutter, left Hartmann door,
		right Hartmann door, or both doors.
------------------------------------------------------------------------------*/
uint8_t close_PNEU(uint8_t cstack)
{

	const char dformat_CLO[] = "Close %s";
	char outbuf[17];

	switch (pcmd[cstack].cobject) {

		case 'b':
			set_PNEUVALVES(LEFTBM, LEFTCLOSE);
			set_PNEUVALVES(RIGHTBM, RIGHTCLOSE);
			sprintf(outbuf, dformat_CLO, "both");
			break;

		case 'l':
			set_PNEUVALVES(LEFTBM, LEFTCLOSE);
			sprintf(outbuf, dformat_CLO, "left");
			break;
			
		case 'r':
			set_PNEUVALVES(RIGHTBM, RIGHTCLOSE);
			sprintf(outbuf, dformat_CLO, "right");
			break;

		case 's':										// Close shutter
			set_PNEUVALVES(SHUTTERBM, SHUTTERCLOSE);
			sprintf(outbuf, dformat_CLO, "shutter");
			break;

		default:
			printError(ERR_PNUMECH, "close_PNEU bad object");
			strcpy(outbuf, "close what?");
			return(ERROR);
			break;

	}

	clear_OLED(1);
	writestr_OLED(1, outbuf, 1);
	return(NOERROR);

}

/*------------------------------------------------------------------------------
uint8_t init_PNEU(void)
	Initializes the MCP23008 port expander connected to the high current
	driver. The MCP23008 port expander connected to the GMR (pneumatic) sensors
	is assumed to be in input mode.
------------------------------------------------------------------------------*/
uint8_t init_PNEU(void)
{

	if (write_MCP23008(HIGHCURRENT, IODIR, 0x00) == ERROR) {
		return(ERROR);
	}
	if (write_MCP23008(HIGHCURRENT, OLAT, 0x00) == ERROR) {
		return(ERROR);
	}
	if (write_MCP23008(PNEUSENSORS, IODIR, 0xFE) == ERROR) {	// Inputs
		return(ERROR);
	}
	if (write_MCP23008(PNEUSENSORS, IPOL, 0x00) == ERROR) {
		return(ERROR);
	}
	if (write_MCP23008(PNEUSENSORS, GPINTEN, 0b11111100) == ERROR) {
		return(ERROR);
	}
	if (write_MCP23008(PNEUSENSORS, INTCON, 0x00) == ERROR) {
		return(ERROR);
	}
	if (write_MCP23008(PNEUSENSORS, IOCON, 0x20) == ERROR) { // Don't increment addr
		return(ERROR);
	}
	if (write_MCP23008(PNEUSENSORS, GPPU, 0x7F) == ERROR) { // Pullups (not really needed)
		return(ERROR);
	}

	PORTD.PIN7CTRL = PORT_PULLUPEN_bm | PORT_ISC_BOTHEDGES_gc;	// PNEUSENSORS

	return(NOERROR);

}

/*------------------------------------------------------------------------------
uint8_t open_PNEU(char mechanism)

	Open the shutter or Hartmann doors

	Pneumatic cylinders move the shutter and Hartmann doors. Each cylinder is
	controlled by a pair of air valves, both of which must be commanded to
	move the shutter or door. One of the pair must be open and the other closed
	to make the item move.

	The valves are electronically activated via an MCP23008 port expander that
	is controlled by the set_PNEUVALVES routine in pneu.c.

	Input:
		mechanism - a character that selects the shutter, left Hartmann door,
		right Hartmann door, or both doors.
------------------------------------------------------------------------------*/
uint8_t open_PNEU(uint8_t cstack)
{

	const char dformat_OPE[] = "Open %s";
	char outbuf[17];

	switch (pcmd[cstack].cobject) {

		case 'b':
			set_PNEUVALVES(LEFTBM, LEFTOPEN);
			set_PNEUVALVES(RIGHTBM, RIGHTOPEN);
			sprintf(outbuf, dformat_OPE, "both");
			break;

		case 'l':
			set_PNEUVALVES(LEFTBM, LEFTOPEN);
			sprintf(outbuf, dformat_OPE, "left");
			break;
		
		case 'r':
			set_PNEUVALVES(RIGHTBM, RIGHTOPEN);
			sprintf(outbuf, dformat_OPE, "right");
			break;

		case 's':
			set_PNEUVALVES(SHUTTERBM, SHUTTEROPEN);
			sprintf(outbuf, dformat_OPE, "shutter");
			break;

		default:
			printError(ERR_PNUMECH, "open_PNEU bad object");
			strcpy(outbuf, "open what?");
			return(ERROR);
			break;

	}

	clear_OLED(1);
	writestr_OLED(1, outbuf, 1);
	return(NOERROR);

}

/*------------------------------------------------------------------------------
void read_PNEUSensors(char *shutter, char *left, char *right, char *air)
	Reports the GMR sensors on the shutter and Hartmann doors, as well as the
	state of the air pressure switch.
------------------------------------------------------------------------------*/
void read_PNEUSensors(char *shutter, char *left, char *right, char *air)
{

	uint8_t sensors, state;
// CHANGE TO pneuState????
	sensors = read_MCP23008(PNEUSENSORS, GPIO);	// NEEDS ERRORCHECK

	// Shutter
	state = sensors >> 6;
	state &= 0b00000011;
	if (state == 1) {
		*shutter = 'c';
	} else if (state == 2) {
		*shutter = 'o';
	} else if (state == 3) {
		*shutter = 't';
	} else {
		*shutter = 'x';
	}

	// Right
	state = sensors >> 2;
	state &= 0b00000011;
	if (state == 1) {
		*right = 'c';
	} else if (state == 2) {
		*right = 'o';
	} else if (state == 3) {
		*right = 't';
	} else {
		*right = 'x';
	}

	// Left
	state = sensors >> 4;
	state &= 0b00000011;
	if (state == 1) {
		*left = 'o';
	} else if (state == 2) {
		*left = 'c';
	} else if (state == 3) {
		*left = 't';
	} else {
		*left = 'x';
	}

	// Air
	if (sensors & 0b00000010) {
		*air = '0';
	} else {
		*air = '1';
	}
}

/*------------------------------------------------------------------------------
set_PNEUVALVES
	Set the Clippard valves.
	Read the current valve state, AND that value with the new pattern, then
	write the new valve state.
------------------------------------------------------------------------------*/
uint8_t set_PNEUVALVES(uint8_t bitmap, uint8_t action)
{

	uint8_t retval, old_state, new_state;

	old_state = read_MCP23008(HIGHCURRENT, GPIO);
	new_state = ((old_state | bitmap) & action);

	if ((retval = write_MCP23008(HIGHCURRENT, OLAT, new_state))) {
		return(retval);
	}

	return(0);

}

/*
uint8_t set_PNEUVALVES(uint8_t bitmap, uint8_t action)
{

	uint8_t retval, old_state, new_state;

	if ((retval = read_MCP23008(HIGHCURRENT, GPIO, &old_state))) {
		return(retval);
	}

	new_state = ((old_state | bitmap) & action);

	if ((retval = write_MCP23008(HIGHCURRENT, OLAT, new_state))) {
		return(retval);
	}

	return(0);

}
*/

ISR(PORTD_PORT_vect)
{

	if (PORTD.INTFLAGS & PIN7_bm) {		// MCP23008 interrupt
		PORTD.INTFLAGS = PIN7_bm;		// Clear the interrupt flag
		pneuState = read_MCP23008(PNEUSENSORS, INTCAP);
	}

}
