/*
 * timers.c
 *
 * Created: 2/28/2021 12:03:00 PM
 *  Author: alanu
 */ 
#include "globals.h"

void start_TCB0(uint16_t msPeriod)
{

	ticks = 0;
	TCB0.CCMP = msPeriod * (uint16_t) (F_CPU/1000UL);	// Check for overflows; msPeriod=19ms is max for 3.33MHz
	TCB0.INTCTRL = TCB_CAPT_bm;				// Interrupt at TOP
//	TCB0.CTRLA = TCB_CLKSEL_CLKDIV1_gc | TCB_ENABLE_bm;	// Start the clock
	TCB0.CTRLA = TCB_ENABLE_bm;				// Start the clock

}

void stop_TCB0(void)
{
	
	TCB0.CTRLA = 0;

}

ISR(TCB0_INT_vect)
{

	TCB0_INTFLAGS = TCB_CAPT_bm;	// Clear interrupt flag
	ticks++;
//	toggle_BEEPER;

}