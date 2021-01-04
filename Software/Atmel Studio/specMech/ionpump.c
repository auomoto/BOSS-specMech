/*------------------------------------------------------------------------------
ionpump.c
	Read the ion pump signal and return the vacuum measurement.
------------------------------------------------------------------------------*/

#ifndef IONPUMPC
#define IONPUMPC

#define REDPUMP		1
#define BLUEPUMP	2

#include "ads1115.c"	// ADC

// Function prototypes
float read_ionpump(uint8_t);

float read_ionpump(uint8_t pumpid) {
/*
	if (pumpid == REDPUMP) {
		return(read_ADS1115(ADC_IP, PGA4096, AIN01, DR128));
	} else if (pumpid == BLUEPUMP) {
		return(read_ADS1115(ADC_IP, PGA4096, AIN23, DR128));
	} else {
		return(-999.0);
	}

*/
	uint8_t pins;
	float voltage, vacuum;

	if (pumpid == REDPUMP) {
		pins = AIN01;
	} else if (pumpid == BLUEPUMP) {
		pins = AIN23;
	} else {
		return(-999.9);
	}

	voltage = read_ADS1115(ADC_IP, PGA4096, pins, DR128);
// compute the actual value here
	vacuum = voltage;
	return(vacuum);

}


#endif
