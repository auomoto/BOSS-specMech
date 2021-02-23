#include "globals.h"

/*------------------------------------------------------------------------------
uint8_t set (char *ptr)

	Input:
		cstack - The array index in the ParsedCMD array (pcmd). The command
			must have been parsed already

	Returns:
		GREATERPROMPT on success
		ERRORPROMPT if an invalid object was requested.
------------------------------------------------------------------------------*/
uint8_t set(uint8_t cstack)
{

	char object;

	object = pcmd[cstack].cobject;

	switch(object) {
		case 't':
			if (strlen(pcmd[cstack].cvalue) != 19) {
				return(ERRORPROMPT);
			}
			put_time(pcmd[cstack].cvalue);
			write_FRAM(FRAMADDR, SETTIMEADDR, (uint8_t*) pcmd[cstack].cvalue, 20);
			break;

		default:
			return(ERRORPROMPT);
	}
	return(GREATERPROMPT);
}
