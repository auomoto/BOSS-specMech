#include "globals.h"

/*------------------------------------------------------------------------------
float get_humidity(uint8_t sensor)
	Returns the relative humidity measured by a Honeywell HiH-4030 sensor.

	TRANSFER FUNCTION:

	From the data sheet, page 2, the signal from the HiH-4030 is
		Vout = Vsupply x (0.0062 x (sensorRH) + 0.16) typical at 25C
			-or-
		sensorRH = ((Vout/Vsupply) - 0.16) / 0.0062 (in %)

	Adding the temperature correction:
	TrueRH = (sensorRH)/(1.0546 - 0.00216 x T) (where T is in C)

	Note the voltage reference is the supply voltage. The final specMech
	circuit will use a 5V regulator for power, which ought to be stable
	enough for this measurement.
	
	CREATE a read_hih430x routine

	Honeywell HiH-4031 humidity sensors. Voltage output is roughly 0.75 V to
	4.0 V, 0% to 100% RH at 0 C. There is a temperature correction required.
------------------------------------------------------------------------------*/
float get_humidity(uint8_t sensor)
{

	uint8_t adcpin;
	float humidity, temperature;

	switch (sensor) {
		case 0:
			temperature = get_temperature(0);
//			adcpin = AIN1;		// test unit
			adcpin = AIN0;		// production
			break;
		case 1:
			temperature = get_temperature(1);
			adcpin = AIN1;
			break;
		case 2:
			temperature = get_temperature(2);
			adcpin = AIN2;
			break;
		default:
			// flag an error
			adcpin = AIN1;			// to avoid uninitialized note
			temperature = 20.0;		// to avoid uninitialized note
			break;
	}

	humidity = ((read_ADS1115(ADC_RH, PGA6144, adcpin, DR128)/5.0) - 0.16) / 0.0062;
	humidity = (humidity / (1.0546 - 0.00216 * temperature));
	return(humidity);

}
