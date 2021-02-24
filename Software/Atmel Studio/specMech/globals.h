#ifndef GLOBALSH
#define GLOBALSH

#define F_CPU	3333333UL
#define VERSION		"2021-02-23"
#define	YES			1
#define	NO			0
#define GREATERPROMPT	0	// Standard return prompt >
#define EXCLAIMPROMPT	1	// No REBOOT ACK prompt !
#define ERRORPROMPT		2	// Generate error line, then >

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>		// sprintf
#include <string.h>		// for strcpy, strlen

// led.c
#define off_LED		(PORTF.OUTSET = PIN5_bm)
#define on_LED		(PORTF.OUTCLR = PIN5_bm)
#define toggle_LED	(PORTF.OUTTGL = PIN5_bm)

// beeper.c
#define off_BEEPER		(PORTB.OUTCLR = PIN2_bm)
#define on_BEEPER		(PORTB.OUTSET = PIN2_bm)
#define toggle_BEEPER	(PORTB.OUTTGL = PIN2_bm)

// specID.c
#define get_SPECID	(PORTF.IN & PIN2_bm ? 2 : 1)

// eeprom.c
void get_BOOTTIME(char*);
void get_VERSION(char*);
void update_BOOTTIME(void);
void update_VERSION(void);

// wdt.c
void reboot(void);

// usart.c
#define BUFSIZE 255
typedef struct {
	uint8_t				// Serial I/O buffer
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

void send_USART(uint8_t, uint8_t*, uint8_t);

// TWI
#define TWIFREQ		100000UL		// Fastest allowed for F_CPU = 3.33 MHz
#define TWIWRITE	0
#define TWIREAD		1
uint8_t read_TWI(void);
uint8_t readlast_TWI(void);
uint8_t start_TWI(uint8_t, uint8_t);
void stop_TWI(void);
uint8_t write_TWI(uint8_t);

// ds3231.c day/time clock
uint8_t get_time(char*);
uint8_t put_time(char*);

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
uint8_t read_MCP23008(uint8_t, uint8_t);
uint8_t write_MCP23008(uint8_t, uint8_t, uint8_t);

// mma8451.c Accelerometer
uint8_t get_orientation(float*, float*, float*);

// pneu.c Pneumatic valves and sensors
void read_PNEUSENSORS(char*, char*, char*, char*);
uint8_t close_PNEU(char);
uint8_t open_PNEU(char);
extern volatile uint8_t pneuState;

// oled.c Display
void clear_OLED(uint8_t);
void writestr_OLED(uint8_t, char*, uint8_t);
extern uint16_t timerOLED, timeoutOLED;	// Used to blank the display

// fram.c
#define FRAMADDR	(0x50)	// A0, A1, A2 grounded
#define SETTIMEADDR	(0x00)	// When the day/time clock was set (20 bytes)
uint8_t read_FRAM(uint8_t, uint16_t, uint8_t *, uint8_t);
uint8_t write_FRAM(uint8_t, uint16_t, uint8_t *, uint8_t);

// mcp9808.c Temperature sensor
float read_MCP9808(void);

// ad590.c Temperature sensor
float read_AD590(uint8_t sensor);

// temperature.c
float get_temperature(uint8_t);

// humidity.c
float get_humidity(uint8_t);

// ionpump.c Vacuum monitor voltage
#define REDPUMP		1
#define BLUEPUMP	2
float read_ionpump(uint8_t);

// nmea.c Output format
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

void commands(void);
void echo_cmd(char*);
uint8_t isaletter(char);
void parse_cmd(char*, uint8_t);
void send_prompt(uint8_t);

// report.c
uint8_t report(uint8_t);

#endif
