#ifndef GLOBALSH
#define GLOBALSH

#define F_CPU	3333333UL
#define VERSION		"2021-02-07"
#define	YES			1
#define	NO			0
#define GREATERPROMPT	0	// Standard return prompt >
#define EXCLAIMPROMPT	1	// No REBOOT ACK prompt !
#define ERRORPROMPT		2	// Generate error line, then >

#include <avr/io.h>
#include <avr/eeprom.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <stdio.h>		// sprintf
#include <string.h>		// for strcpy, strlen

// initialize.c
void initialize(void);

// ports.c
void init_PORTS(void);

// led.c
void init_LED(void);
void tog_LED(void);
#define off_LED		(PORTF.OUTSET = PIN5_bm)
#define on_LED		(PORTF.OUTCLR = PIN5_bm)
#define toggle_LED	(PORTF.OUTTGL = PIN5_bm)

// beeper.c
void init_BEEPER(void);
#define off_BEEPER	(PORTB.OUTCLR = PIN2_bm)
#define on_BEEPER	(PORTB.OUTSET = PIN2_bm)
#define toggle_BEEPER	(PORTB.OUTTGL = PIN2_bm)

// specID.c
void init_SPECID(void);
#define get_SPECID	(PORTF.IN & PIN2_bm ? 2 : 1)

// eeprom.c
#define VERSIONADDR		(0)		// Version string must have exactly 11 bytes
#define VERSIONSIZE		(11)	// Version is the date, as YYYY-DD-MM
#define BOOTTIMEADDR	(11)	// EEPROM address for boot time
#define BOOTTIMESIZE	(21)	// ISO date format, exactly 21 bytes
void get_BOOTTIME(char*);
void get_VERSION(char*);
void init_EEPROM(void);
void update_BOOTTIME(void);
void update_VERSION(void);

// wdt.c
void reboot(void);

// rtc.c
void init_RTC(uint16_t);

// usart.c
#define BUFSIZE 128
typedef struct {
	uint8_t					// Serial I/O buffer
	data[BUFSIZE],		// Data to send or data received
	head,				// Ring buffer head index
	tail,				// Ring buffer tail index
	nbytes,				// Number of data bytes in data[];
	nxfrd;				// Temporary counter (number of bytes transferred)
	uint8_t	volatile done;	// Is the transfer complete ('\r' seen)?
} USARTBuf;

extern USARTBuf
	send0_buf, send1_buf, send3_buf,
	recv0_buf, recv1_buf, recv3_buf;

void init_USART(void);
void send_USART(uint8_t, uint8_t*, uint8_t);

// TWI
#define TWIFREQ		100000UL		// Fastest allowed for F_CPU = 3.33 MHz
#define TWIBAUD		((uint8_t) (F_CPU/(2*TWIFREQ)) - 5)	// Ignore rise time
#define TWIWRITE	0
#define TWIREAD		1
#define TWIBUSERR	1
#define TWIARBLOST	2
#define TWIACKERR	3
#define TWINODEVICE	4
void init_TWI(void);
uint8_t read_TWI(void);
uint8_t readlast_TWI(void);
uint8_t start_TWI(uint8_t, uint8_t);
void stop_TWI(void);
uint8_t write_TWI(uint8_t);

// ds3231.c day/time clock
#define DS3231ADDR	(0xD0)	// Day/Time clock TWI address
void convert_ds2iso(char*, uint8_t*);
void convert_iso2ds(uint8_t *, char*);
uint8_t get_time(char*);
uint8_t put_time(char*);
uint8_t read_DS3231(uint8_t, uint8_t*);
uint8_t uint8_to_bcd(uint8_t);
uint8_t write_DS3231(uint8_t, uint8_t*);

// mcp23008.c
#define IODIR	(0x00)	// Pin direction; 1 for input, 0 for output
#define IPOL	(0x01)	// Pin polarity on GPIO; 1 for inverted, not used here
#define GPINTEN	(0x02)	// Interrupt on change; 1 to enable that pin, set DEFVAL & INTCON
#define DEFVAL	(0x03)	// Default comparison value for GPINTEN
#define INTCON	(0x04)	// Interrupt control; 1 compares w/DEFVAL, 0 for pin change
#define IOCON	(0x05)	// Set parameters (see page 15)
#define GPPU	(0x06)	// Pullups; 100K pullup if 1, tri-state if 0 (default)
#define INTF	(0x07)	// Interrupt flag (find out which pin caused the interrupt)
#define INTCAP	(0x08)	// GPIO state when interrupt occurred; cleared when GPIO is read
#define GPIO	(0x09)	// Read for input
#define OLAT	(0x0A)	// Write for output
#define MCP23008ERROR_bm	(0b00000001)	// bit 0 is an MCP23008 error
uint8_t read_MCP23008(uint8_t, uint8_t, uint8_t*);
uint8_t write_MCP23008(uint8_t, uint8_t, uint8_t);

// mma8451.c Accelerometer
#define MMA8451ADDR			(0x3A)	// Two are allowed on the TWI bus
#define MMA8451OUTXMSB		(0x01)	// Start address of output data
#define MMA8451WHOAMI		(0x0D)	// MMA8451 WHO_AM_I (0x1A is the answer)
#define MMA8451HFCUTOFF		(0x0F)	// MMA8451 HP_FILTER_CUTOFF
#define MMA8451CTRLREG1		(0x2A)	// MMA8451 CTRL_REG1
#define MMA8451CTRLREG2		(0x2B)	// MMA8451 CTRL_REG2
uint8_t get_orientation(uint8_t, float*, float*, float*);
uint8_t init_MMA8451(void);
uint8_t read_MMA8451(uint8_t, uint8_t, uint8_t*, uint8_t);
uint8_t write_MMA8451(uint8_t, uint8_t, uint8_t);

// pneu.c
#define PNEUSENSORS		(0x42)	// GMR sensors on pneumatic cylinders
#define HIGHCURRENT		(0x48)	// High current driver for pneumatic valves
#define SHUTTERBM		(0x22)	// OR existing value with this first, then
#define SHUTTEROPEN		(0xCE)	// AND with this pattern to open
#define SHUTTERCLOSE	(0xEC)	// AND with this pattern to close
#define LEFTBM			(0x44)	// OR existing value with this, then
#define LEFTOPEN		(0xAE)	// AND with this pattern to open
#define LEFTCLOSE		(0xEA)	// AND with this pattern to close
#define RIGHTBM			(0x88)	// OR existing value with this, then
#define RIGHTOPEN		(0x6E)	// AND with this pattern to open
#define RIGHTCLOSE		(0xE6)	// AND with this pattern to close
void read_PNEUSENSORS(char*, char*, char*, char*);
uint8_t init_PNEU(void);
uint8_t close_PNEU(char);
uint8_t open_PNEU(char);
uint8_t set_PNEUVALVES(uint8_t, uint8_t);

// oled.c
#define CLEARDISPLAY	0x01		// Newhaven command (not used)
#define DISPLAYON		0x0C		// Newhaven command (not used)
#define DISPLAYOFF		0x08		// Newhaven command (not used)
#define OLEDADDR0		(0x3c << 1)	// TWI bus address
#define OLEDADDR1		(0x3d << 1)	// TWI bus address
#define OLEDCMD			0x00		// Newhaven command was 1
#define OLEDDATA		0x40		// Newhaven command was 0
#define OLEDLINE1		0x80		// Newhaven command
#define OLEDLINE2		0xC0		// Newhaven command
void clear_OLED(uint8_t);
void off_OLED(uint8_t);
void on_OLED(uint8_t);
void init_OLED(uint8_t);
void write_OLED(uint8_t, uint8_t, uint8_t);
void writestr_OLED(uint8_t, char*, uint8_t);

// fram.c
#define FRAMADDR	(0xA0)	// A0, A1, A2 grounded
#define SETTIMEADDR	(0x00)	// When the day/time clock was set (20 bytes)
uint8_t read_FRAM(uint8_t, uint16_t, uint8_t *, uint8_t);
uint8_t write_FRAM(uint8_t, uint16_t, uint8_t *, uint8_t);

// mcp9808.c
#define MCP9808ADDR		(0x30)	// TWI address
#define TEMPREGISTER	(0x05)	// Ambient temperature register
float read_MCP9808(void);

// ads1115.c
#define ADC_TE	0b10010000		// ADDR pin connected to GND (temperature)
#define ADC_RH	0b10010000		// Relative Humidity (same unit as temperature)
#define ADC_IP	0b10010010		// ADDR pin connected to VDD (ion pump)
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
float read_ADS1115(uint8_t, uint8_t, uint8_t, uint8_t);

// ad590.c
#define AD590DRIVER		(0x4E)		// MCP23008 address
#define AD590RESISTOR	(1000.0)	// Ohms
float read_AD590(uint8_t sensor);

// temperature.c
float get_temperature(uint8_t);

// humidity.c
float get_humidity(uint8_t);

// ionpump.c
#define REDPUMP		1
#define BLUEPUMP	2
#define ISO224SLOPE	(2.0053)	// To log10(pressure) from ISO224 voltage
#define ISO224INTER	(-6.8637)	// Transfer function from Modion pump voltage
float read_ionpump(uint8_t);

// nmea.c
void checksum_NMEA(char*);
void format_ERR(char*);

// set.c
uint8_t set(uint8_t);

// commands.c
#define CVALUESIZE		41	// Maximum length of a command value string
#define CIDSIZE			9	// Maximum length of a command ID string
#define CSTACKSIZE		10	// Number of stacked up commands allowed
typedef struct {
	char cverb,			// Single character command
	cobject,			// Single character object
	cvalue[CVALUESIZE],	// Input value string for object
	cid[CIDSIZE];		// Command ID string
} ParsedCMD;
extern ParsedCMD pcmd[CSTACKSIZE];	// Split the command line into its parts

// commands.c
void commands(void);
void echo_cmd(char*);
uint8_t isaletter(char);
void parse_cmd(char*, uint8_t);
void send_prompt(uint8_t);

// report.c
uint8_t report(uint8_t);

#endif
