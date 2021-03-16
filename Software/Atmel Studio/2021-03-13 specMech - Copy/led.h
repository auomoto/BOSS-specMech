#ifndef LEDH
#define LEDH

#define off_LED		(PORTF.OUTSET = PIN5_bm)
#define on_LED		(PORTF.OUTCLR = PIN5_bm)
#define toggle_LED	(PORTF.OUTTGL = PIN5_bm)

#endif /* LEDH */