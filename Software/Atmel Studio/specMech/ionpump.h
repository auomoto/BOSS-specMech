#ifndef IONPUMPH
#define IONPUMPH

#define REDPUMP		1
#define BLUEPUMP	2
#define ISO224SLOPE	(2.04545)	// To log10(pressure) from ISO224 voltage
#define ISO224INTER	(-6.86373)	// Transfer function from Modion pump voltage

float read_ionpump(uint8_t);

#endif