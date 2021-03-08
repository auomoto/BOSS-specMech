#ifndef BEEPERH
#define BEEPERH

#define off_BEEPER		(PORTB.OUTCLR = PIN2_bm)
#define on_BEEPER		(PORTB.OUTSET = PIN2_bm)
#define toggle_BEEPER	(PORTB.OUTTGL = PIN2_bm)

#endif /* BEEPERH */