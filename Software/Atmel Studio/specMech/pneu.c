/*------------------------------------------------------------------------------
pneu.c
	Pneumatic valves and their sensors
------------------------------------------------------------------------------*/

#ifndef PNEUMATICSC
#define PNEUMATICSC

// MCP23008 addresses
#define HIGHCURRENT	(0x48)		// High current driver for pneumatic valves
//#define HIGHCURRENT		(0x40)	// test device is at 0x40

// Valve actions
#define SHUTTERBM		(0x22)	// OR existing value with this first, then
#define SHUTTEROPEN		(0xCE)	// AND with this pattern to open
#define SHUTTERCLOSE	(0xEC)	// AND with this pattern to close

#define LEFTBM			(0x44)	// OR existing value with this, then
#define LEFTOPEN		(0xAE)	// AND with this pattern to open
#define LEFTCLOSE		(0xEA)	// AND with this pattern to close

#define RIGHTBM			(0x88)	// OR existing value with this, then
#define RIGHTOPEN		(0x6E)	// AND with this pattern to open
#define RIGHTCLOSE		(0xE6)	// AND with this pattern to close

#include "mcp23008.c"

// Function prototypes
uint8_t init_pneu(void);
uint8_t pneu_close(char *ptr);
uint8_t pneu_open(char *ptr);
uint8_t set_valves(uint8_t, uint8_t);

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
uint8_t pneu_close(char *ptr)
	Close the shutter or Hartmann doors

	Pneumatic cylinders move the shutter and Hartmann doors. Each cylinder is
	controlled by a pair of air valves, both of which must be commanded to
	move the shutter or door. One of the pair must be open and the other closed
	to make the item move.

	The valves are electronically activated via an MCP23008 port expander that
	is controlled by the set_valves routine in pneu.c.

	Input:
		*ptr - a character that selects the shutter, left Hartmann door,
		right Hartmann door, or both doors.

MOVE THIS TO pneu.c?
------------------------------------------------------------------------------*/
uint8_t pneu_close(char *ptr)
{

	ptr++;

	switch (*ptr) {

		case 'b':
			set_valves(LEFTBM, LEFTCLOSE);
			set_valves(RIGHTBM, RIGHTCLOSE);
			break;

		case 'l':
			set_valves(LEFTBM, LEFTCLOSE);
			break;
			
		case 'r':
			set_valves(RIGHTBM, RIGHTCLOSE);
			break;

		case 's':										// Close shutter
			set_valves(SHUTTERBM, SHUTTERCLOSE);
			break;

		default:
			return(ERRORPROMPT);
			break;

	}

	return(GREATERPROMPT);

}

/*------------------------------------------------------------------------------
uint8_t pneu_open(char *ptr)
	Open the shutter or Hartmann doors

	Pneumatic cylinders move the shutter and Hartmann doors. Each cylinder is
	controlled by a pair of air valves, both of which must be commanded to
	move the shutter or door. One of the pair must be open and the other closed
	to make the item move.

	The valves are electronically activated via an MCP23008 port expander that
	is controlled by the set_valves routine in pneu.c.

	Input:
		*ptr - a character that selects the shutter, left Hartmann door,
		right Hartmann door, or both doors.

MOVE THIS TO pneu.c?
------------------------------------------------------------------------------*/
uint8_t pneu_open(char *ptr)
{

	ptr++;
	switch (*ptr) {

		case 'b':
			set_valves(LEFTBM, LEFTOPEN);
			set_valves(RIGHTBM, RIGHTOPEN);
			break;

		case 'l':
			set_valves(LEFTBM, LEFTOPEN);
			break;
		
		case 'r':
			set_valves(RIGHTBM, RIGHTOPEN);
			break;

		case 's':
			set_valves(SHUTTERBM, SHUTTEROPEN);
			break;

		default:
			return(ERRORPROMPT);

	}

	return(GREATERPROMPT);

}

/*------------------------------------------------------------------------------
set_valves.c
	Set the Clippard valves.
	Read the current valve state, AND that value with the new pattern, then
	write the new valve state.
------------------------------------------------------------------------------*/
uint8_t set_valves(uint8_t bitmap, uint8_t action)
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

#endif
