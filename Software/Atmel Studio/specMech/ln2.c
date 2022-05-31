#include "globals.h"
#include "ln2.h"

uint8_t get_ln2(uint8_t *ln2status)
{

	char strbuf[BUFSIZE];
	const char fmt0[] = "LN2 response timeout";

	recv3_buf.nxfrd = 0;
	recv3_buf.done = NO;

	send3_buf.nbytes = 1;
	send3_buf.data[0] = '\r';
	send_USART(3, send3_buf.data, 1);

	USART3_ticks = 0;
	while (recv3_buf.done == NO) {
		if (USART3_ticks > 4000) {
			sprintf(strbuf, fmt0);
			printError(ERR_LN2, strbuf);
			return(ERROR);
		}
	}

	strcpy((char*) ln2status, (char*) recv3_buf.data);
	return(NOERROR);

}