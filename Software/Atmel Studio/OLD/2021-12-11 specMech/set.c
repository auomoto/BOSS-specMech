#include "globals.h"
#include "errors.h"
#include "fram.h"
#include "ds3231.h"
#include "commands.h"
#include "set.h"

/*------------------------------------------------------------------------------
uint8_t set (char *ptr)

	Input:
		cstack - The array index in the ParsedCMD array (pcmd). The command
			must have been parsed already

	Returns:
		ERROR if an invalid object was requested.
		NOERROR otherwise
------------------------------------------------------------------------------*/
uint8_t set(uint8_t cstack)
{

	char object;

	object = pcmd[cstack].cobject;

	switch(object) {
		case 't':
			if (strlen(pcmd[cstack].cvalue) != 19) {
				printError(ERR_SETTIME, "set: bad time format");
				return(ERROR);
			}
			if (put_time(pcmd[cstack].cvalue) == ERROR) {
				printError(ERR_ISO, "set time: put_time error");
				return(ERROR);
			}
			write_FRAM(FRAMTWIADDR, SETTIMEADDR, (uint8_t*) pcmd[cstack].cvalue, 20);
			break;

		default:
			printError(ERR_SET, "set what?");
			return(ERROR);
	}
	return(NOERROR);
}
