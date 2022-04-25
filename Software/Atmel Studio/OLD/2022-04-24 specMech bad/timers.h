#ifndef TIMERSH
#define TIMERSH

#include "oled.h"
#include "roboclaw.h"

volatile uint16_t USART0_ticks, USART1_ticks, USART3_ticks, TWI_ticks, testTicks;

void init_RTC(uint16_t);
void start_TCB0(uint16_t);
void stop_TCB0(void);

#endif /* TIMERSH */
