/*------------------------------------------------------------------------------
pneu.c
	Pneumatic valves and their sensors
------------------------------------------------------------------------------*/

#include "globals.h"

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
uint8_t close_PNEU(char mech)
{

	switch (mech) {

		case 'b':
			set_PNEUVALVES(LEFTBM, LEFTCLOSE);
			set_PNEUVALVES(RIGHTBM, RIGHTCLOSE);
			break;

		case 'l':
			set_PNEUVALVES(LEFTBM, LEFTCLOSE);
			break;
			
		case 'r':
			set_PNEUVALVES(RIGHTBM, RIGHTCLOSE);
			break;

		case 's':										// Close shutter
			set_PNEUVALVES(SHUTTERBM, SHUTTERCLOSE);
			break;

		default:
			return(ERRORPROMPT);
			break;

	}

	return(GREATERPROMPT);

}

/*------------------------------------------------------------------------------
uint8_t init_PNEU(void)
	Initializes the MCP23008 port expander connected to the high current
	driver. The MCP23008 port expander connected to the GMR (pneumatic) sensors
	is assumed to be in input mode.
------------------------------------------------------------------------------*/
uint8_t init_PNEU(void)
{

	uint8_t retval;

	if ((retval = write_MCP23008(HIGHCURRENT, IODIR, 0x00))) {
		return(retval);
	}
	if ((retval = write_MCP23008(HIGHCURRENT, OLAT, 0x00))) {
		return(retval);
	}
	return(0);

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
uint8_t open_PNEU(char mechanism)
{

	switch (mechanism) {

		case 'b':
			set_PNEUVALVES(LEFTBM, LEFTOPEN);
			set_PNEUVALVES(RIGHTBM, RIGHTOPEN);
			break;

		case 'l':
			set_PNEUVALVES(LEFTBM, LEFTOPEN);
			break;
		
		case 'r':
			set_PNEUVALVES(RIGHTBM, RIGHTOPEN);
			break;

		case 's':
			set_PNEUVALVES(SHUTTERBM, SHUTTEROPEN);
			break;

		default:
			return(ERRORPROMPT);

	}

	return(GREATERPROMPT);

}

/*------------------------------------------------------------------------------
void read_PNEUSENSORS(char *shutter, char *left, char *right, char *air)
	Reports the GMR sensors on the shutter and Hartmann doors, as well as the
	state of the air pressure switch.
------------------------------------------------------------------------------*/
void read_PNEUSENSORS(char *shutter, char *left, char *right, char *air)
{

	uint8_t sensors, state;

	read_MCP23008(PNEUSENSORS, GPIO, &sensors);

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
set_PNEUVALVES.c
	Set the Clippard valves.
	Read the current valve state, AND that value with the new pattern, then
	write the new valve state.
------------------------------------------------------------------------------*/
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
