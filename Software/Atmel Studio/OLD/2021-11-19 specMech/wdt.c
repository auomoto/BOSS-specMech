#include "globals.h"
#include "initialize.h"
#include "roboclaw.h"
#include "errors.h"
#include "wdt.h"

void reboot(void)
{

	CPU_CCP = CCP_IOREG_gc;
	WDT.CTRLA = WDT_PERIOD_8CLK_gc;

}

ISR(PORTF_PORT_vect)
{

	if (PORTF.INTFLAGS & PIN6_bm) {		// Curiosity Nano button
		PORTF.INTFLAGS = PIN6_bm;		// Clear the interrupt flag
		reboot();
	}
}
