/*------------------------------------------------------------------------------
ionpump.c
	Read the ion pump signal and return the vacuum measurement.

	The ion pump signal ranges from 1.45 V to 5.85 V representing pressures
	of 1E-8 Torr to 1E-5 Torr (1.333E-6 Pascals to 1.333E-3 Pascals). Higher
	voltages mean lower pressures.

	The ion pump signal is processed through an isolation amplifier that
	generates a differential signal ranging from 0.49 V to 1.99 V for the
	same pressure limits.

	The differential signal is sent to an ADS1115 analog-to-digital converter,
	which can handle two separate differential inputs, one for each of the two
	ion pumps we have.

	Note: The transfer function from ion pump voltage to vacuum pressure and
		the transfer function from ion pump voltage to the isolated amplifier
		output were estimated from datasheet plots. These two transfer functions
		were combined to get the isolated differential voltage to pressure
		conversion indicated by ISO224SLOPE and ISO224INTER. So it's pretty
		rough, but good enough to tell you when to be worried about the vacuum.
------------------------------------------------------------------------------*/

#include "globals.h"
#include "ads1115.h"
#include "ionpump.h"

#define ISO224SLOPE	(2.0455)	// To log10(pressure) from ISO224 voltage
#define ISO224INTER	(-6.8637)	// Transfer function from Modion pump voltage

/*------------------------------------------------------------------------------
float read_ionpump(uint8_t pumpid)
	Read the ion pump signal that indicates pressure.

	Input:
		pumpid - Either REDPUMP or BLUEPUMP to select which ion pump to read.

	Output:
		None

	Returns:
		The log10(pressure) where pressure is in Pascals. Subtracting 2.125
		from this value provides the log10(pressure) for pressure in Torr.
		If the voltage is out of range, -999.9 is returned..
------------------------------------------------------------------------------*/
float read_ionpump(uint8_t pumpid) {

	uint8_t pins;
	float voltage, vacuum;

	if (pumpid == REDPUMP) {
		pins = AIN01;
	} else if (pumpid == BLUEPUMP) {
		pins = AIN23;
	} else {
		return(-666.0);
	}

	voltage = read_ADS1115(ADC_IP, PGA4096, pins, DR128);

	if (voltage < 0.4) {						// Useful range for the Modion pump
		return(-666.0);							// after ISO224 op-amp is 0.5 to 2.0 V
	} else if (voltage > 2.0) {
		return(-666.0);
	}

	vacuum = ISO224SLOPE * voltage + ISO224INTER;

	return(vacuum);
//return(voltage);

}
