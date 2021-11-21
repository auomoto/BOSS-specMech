#ifndef TIMERSH
#define TIMERSH

volatile uint16_t ticks;

void start_TCB0(uint16_t);
void stop_TCB0(void);

#endif /* TIMERSH */