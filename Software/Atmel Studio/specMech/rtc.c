#include "globals.h"
#include "led.h"
#include "oled.h"
#include "roboclaw.h"

/*----------------------------------------------------------------------
void init_RTC(uint16_t ticksRTC)
	Initialize the real time clock

	The real time clock (RTC) controller uses an on-board 32.768 kHz
	crystal to generate interrupts. The RTC is configured to run at
	512 Hz (64x divider).

	Input:
		ticksRTC - sets the time between interrupts such that
			ticksRTC = (512*secs)-1 where secs is the number of seconds
			between interrupts. ticksRTC = 30719 generates an interrupt
			every minute. ticksRTC = 511 generates an interrupt every
			second.
----------------------------------------------------------------------*/
void init_RTC(uint16_t ticksRTC)
{

	uint8_t temp;

	// Disable the external oscillator by clearing the enable bit 0
	temp = CLKCTRL.XOSC32KCTRLA;
	temp &= ~CLKCTRL_ENABLE_bm;		// set bit 0 of CLKCTRL.XOSC32KCTRLA to 0
	CPU_CCP = CCP_IOREG_gc;
	CLKCTRL.XOSC32KCTRLA = temp;

	// Wait for status bit (bit 6) in MCLKSTATUS to go to 0 (XOSC32K not running)
	while (CLKCTRL.MCLKSTATUS & CLKCTRL_XOSC32KS_bm) {
		asm("nop");
	}

	// Select the external crystal (as opposed to external clock)
	// by setting the SEL bit on XOSC32KCTRLA (bit 2) to 0
	temp = CLKCTRL.XOSC32KCTRLA;
	temp &= ~CLKCTRL_SEL_bm;
	CPU_CCP = CCP_IOREG_gc;
	CLKCTRL.XOSC32KCTRLA = temp;

	// Enable the external oscillator by setting the enable bit
	// (bit 0) in CLKCTRL.XOSC32KCTRLA to 1
	temp = CLKCTRL.XOSC32KCTRLA;
	temp |= CLKCTRL_ENABLE_bm;
	CPU_CCP = CCP_IOREG_gc;
	CLKCTRL.XOSC32KCTRLA = temp;

	while (RTC.STATUS) {	// Wait for all registers to sync
		asm("nop");
	}

//	RTC.PER = 30719;	// Set 1 minute period for overflow interrupts 
//	RTC.PER = 511;		// Set 1 sec period for overflow interrupts
	RTC.PER = ticksRTC;

	// Select the external crystal oscillator in RTC.CLKSEL register
	RTC.CLKSEL = RTC_CLKSEL_TOSC32K_gc;

	// Enable running in debug mode by setting bit 0 in RTC.CLKSEL to 1
	RTC.DBGCTRL |= RTC_DBGRUN_bm;

	// Enable running in standby mode by setting bit 7 in RTC.CTRLA to 1
	// & set the prescaler to DIV64 (512 Hz)
	// & set the RTC enable bit
	RTC.CTRLA = RTC_PRESCALER_DIV64_gc | RTC_RUNSTDBY_bm | RTC_RTCEN_bm;

	RTC.INTCTRL |= RTC_OVF_bm;	// Enable overflow interrupt

}

/*---------------------------------------------------------------------
Interrupt routine for RTC
	Every tick of the RTC executes here
----------------------------------------------------------------------*/
ISR(RTC_CNT_vect)
{

	RTC.INTFLAGS = RTC_OVF_bm;		// Clear interrupt flag
/*
	if (timerOLED) {
		if (timerOLED > timeoutOLED) {	// Display timeout
			clear_OLED(0);
			clear_OLED(1);
			timerOLED = 0;
		} else {
			timerOLED++;
		}
	}
*/

	timerOLED++;

	toggle_LED;						// Defined in led.c

	timerSAVEENCODER++;

}
