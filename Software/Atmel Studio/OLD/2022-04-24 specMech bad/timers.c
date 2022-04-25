#include "globals.h"
#include "timers.h"

/*----------------------------------------------------------------------
void init_RTC(uint16_t ticksRTC)
	Initialize the real time counter

	The real time counter (RTC) uses the on-board 32.768 kHz crystal
	to generate two interrupts: one every 1 ms and the other at 1 sec
	intervals (except at the start, to blink the LED faster).

	The RTC overflow is configured to run at 512 Hz (64x divider).

	Input:
		ticksRTC - sets the time between overflow interrupts such that
			ticksRTC = (512*secs)-1 where secs is the number of seconds
			between interrupts. ticksRTC = 30719 generates an interrupt
			every minute. ticksRTC = 511 generates an interrupt every
			second. A separate periodic interrupt is set to happen every
			1 ms.

	Setting up the crystal oscillator tricky. See
	https://ww1.microchip.com/downloads/en/Appnotes/TB3213-Getting-Started-with-RTC-DS90003213B.pdf
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

	// Select the external crystal by setting the SEL bit on XOSC32KCTRLA
	// (bit 2) to 0
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

	RTC.PER = ticksRTC;

	// Select the external crystal oscillator in RTC.CLKSEL register
	RTC.CLKSEL = RTC_CLKSEL_TOSC32K_gc;

	// Enable running in debug mode by setting bit 0 in RTC.CLKSEL to 1
	RTC.DBGCTRL |= RTC_DBGRUN_bm;

	// Enable running in standby mode by setting bit 7 in RTC.CTRLA to 1,
	// set the prescaler to DIV64 (512 Hz) and set the RTC enable bit
	RTC.CTRLA = RTC_PRESCALER_DIV64_gc | RTC_RUNSTDBY_bm | RTC_RTCEN_bm;
	RTC.INTCTRL |= RTC_OVF_bm;	// Enable overflow interrupt

	// Set periodic interrupts every 33 ticks (1 ms)
	RTC.PITCTRLA = RTC_PERIOD_CYC32_gc | RTC_PITEN_bm;
	RTC.PITINTCTRL = RTC_PI_bm;	// Enable periodic interrupt

}

/*---------------------------------------------------------------------
RTC overflow interrupt routine
----------------------------------------------------------------------*/
ISR(RTC_CNT_vect)
{

	RTC.INTFLAGS = RTC_OVF_bm;		// Clear interrupt flag

	timerOLED++;					// Turn off the OLED display
	toggle_LED;						// Blink the light
	timerSAVEENCODER++;				// Save the motor encoder values

}

/*---------------------------------------------------------------------
RTC periodic interrupt routine. This executes every ms.
----------------------------------------------------------------------*/
ISR(RTC_PIT_vect)
{

	RTC.PITINTFLAGS = RTC_PI_bm;	// Clear interrupt flag
	testTicks++;
	USART0_ticks++;
	USART1_ticks++;
	USART3_ticks++;
	TWI_ticks++;

}

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

}