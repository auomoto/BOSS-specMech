#ifndef SPECIDH		// GET RID OF THIS 
#define SPECIDH

#define get_SPECID	(PORTF.IN & PIN2_bm ? 2 : 1)

void init_SPECID(void);

#endif