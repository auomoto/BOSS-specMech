/*------------------------------------------------------------------------------
Watchdog Timer
	We use this only to reboot from the command line.

------------------------------------------------------------------------------*/

#ifndef WDTC
#define WDTC
#endif

#include <avr/wdt.h>

// Function Prototypes
void reboot(void);

void reboot(void)
{

	CPU_CCP = CCP_IOREG_gc;
	WDT.CTRLA = WDT_PERIOD_8CLK_gc;

}
