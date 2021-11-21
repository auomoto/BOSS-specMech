#ifndef ADS1115H
#define ADS1115H

#define ADC_TE	(0x48)		// ADDR pin connected to GND (temperature)
#define ADC_RH	(0x48)		// Relative Humidity (same unit as temperature)
#define ADC_IP	(0x49)		// ADDR pin connected to VDD (ion pump)
#define ADS1115CONFIG		(0x01)	// Config register
#define ADS1115CONVERSION	(0x00)	// Conversion register
#define ADS1115ERROR		(-32768)

// ADS1115 analog input pin (bits 6, 5, & 4)
#define AIN0	0b01000000		// 100
#define AIN1	0b01010000		// 101
#define AIN2	0b01100000		// 110
#define AIN3	0b01110000		// 111
#define AIN01	0b00000000		// 000 Differential between AIN0 and AIN1
#define AIN23	0b00110000		// 011 Differential between AIN2 and AIN3

// ADS1115 programmable gain amp setting (bits 3, 2, 1 in MSB config)
#define PGA0256	0b00001010		// 101	+/-0.256V
#define PGA0512	0b00001000		// 100	+/-0.512V
#define PGA1024	0b00000110		// 011	+/-1.024V
#define PGA2048	0b00000100		// 010	+/-2.048V
#define PGA4096	0b00000010		// 001	+/-4.096V
#define PGA6144	0b00000000		// 000	+/-6.144V
#define SCA0256	(7.812738E-6)	// 15-bit scale
#define SCA0512	(1.562547E-5)
#define SCA1024	(3.125095E-5)
#define SCA2048	(6.250119E-5)
#define SCA4096	(1.250038E-4)
#define SCA6144	(1.875057E-4)

#define DR008	0x00000000		// ADC data rates in samples per second
#define DR016	0b00100000		// Top 3 bits of lower CONFIG register
#define DR032	0b01000000
#define DR064	0b01100000
#define DR128	0b10000000		// Default
#define DR250	0b10100000
#define DR475	0b11000000
#define DR860	0b11100000

uint8_t read_ADS1115(uint8_t, uint8_t, uint8_t, uint8_t, float*);

#endif
