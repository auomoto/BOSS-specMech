#include "globals.h"
#include "timers.h"

/*------------------------------------------------------------------------------
void start_TCB0(uint16_t msPeriod)
	Start an interrupt timer

	Input:
		msPeriod - the tick period in ms. Must be < 19 for 3.33 MHz clock.

	Output:
		None
------------------------------------------------------------------------------*/
void start_TCB0(uint16_t msPeriod)
{
ticks = 0;
	// Check for overflows; msPeriod=19ms is max for 3.33MHz
	TCB0.CCMP = msPeriod * (uint16_t) (F_CPU/1000UL);
	TCB0.INTCTRL = TCB_CAPT_bm;				// Interrupt at TOP
	TCB0.CTRLA = TCB_ENABLE_bm;				// Start the clock

}

/*------------------------------------------------------------------------------
void stop_TCB0(void)
	Turn off the TCB0 timer
------------------------------------------------------------------------------*/
void stop_TCB0(void)
{
	
	TCB0.CTRLA = 0;

}

ISR(TCB0_INT_vect)
{

	TCB0_INTFLAGS = TCB_CAPT_bm;	// Clear interrupt flag
	ticks++;
	TWI_ticks++;
	USART1_ticks++;

}