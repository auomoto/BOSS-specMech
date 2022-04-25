#include "globals.h"
#include "initialize.h"

void reboot(void)
{

	init_XPORT();
	CPU_CCP = CCP_IOREG_gc;
	WDT.CTRLA = WDT_PERIOD_8CLK_gc;

}
