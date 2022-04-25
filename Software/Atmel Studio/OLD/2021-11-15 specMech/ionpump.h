#ifndef IONPUMPH
#define IONPUMPH

#define REDPUMP		1
#define BLUEPUMP	2
#define ISO224SLOPE	(2.0455)	// To log10(pressure) from ISO224 voltage
#define ISO224INTER	(-6.8637)	// Transfer function from Modion pump voltage

float read_ionpump(uint8_t);

#endif