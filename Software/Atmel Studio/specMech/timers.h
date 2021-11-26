#ifndef TIMERSH
#define TIMERSH

volatile uint16_t USART0_ticks, USART1_ticks, USART3_ticks, TWI_ticks;

void start_TCB0(uint16_t);
void stop_TCB0(void);

#endif /* TIMERSH */