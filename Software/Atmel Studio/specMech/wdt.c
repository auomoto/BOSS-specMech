/*------------------------------------------------------------------------------
wdt.c
	Watchdog timer setup
	We use this only to reboot from the command line.
------------------------------------------------------------------------------*/

#ifndef WDTC
#define WDTC

#include "globals.h"

void reboot(void)
{

	CPU_CCP = CCP_IOREG_gc;
	WDT.CTRLA = WDT_PERIOD_8CLK_gc;

}

#endif
