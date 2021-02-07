#ifndef GLOBALSH
#define GLOBALSH

#define F_CPU		3333333UL

#include <avr/wdt.h>
#include <avr/eeprom.h>
#include <avr/interrupt.h>
#include <stdio.h>		// sprintf
#include <string.h>		// for strcpy, strlen

#define	YES				1
#define	NO				0
#define VERSION		"2021-02-07"
#define GREATERPROMPT	0	// Standard return prompt >
#define EXCLAIMPROMPT	1	// No REBOOT ACK prompt !
#define ERRORPROMPT		2	// Generate error line, then >

// EEPROM
#define VERSIONADDR		(0)		// Version string must have exactly 11 bytes
#define VERSIONSIZE		(11)	// Version is the date, as YYYY-DD-MM
#define BOOTTIMEADDR	(11)	// Boot time has exactly 21 bytes
#define BOOTTIMESIZE	(21)	// ISO date format, nearest second
void get_BOOTTIME(char*);
void get_VERSION(char*);
void init_EEPROM(void);
void update_BOOTTIME(void);
void update_VERSION(void);

// SPECID
void init_SPECID(void);
uint8_t get_SPECID(void);

// NMEA
void checksum_NMEA(char*);
void format_ERR(char*);

// RTC
void init_RTC(uint16_t);

// USART
void send_USART(uint8_t, uint8_t*, uint8_t);
void init_USART(void);

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

// MMA8451 accelerometer defines
#define MMA8451ADDR			(0x3A)	// Two are allowed on the TWI bus
#define MMA8451OUTXMSB		(0x01)	// Start address of output data
#define MMA8451WHOAMI		(0x0D)	// MMA8451 WHO_AM_I (0x1A is the answer)
#define MMA8451HFCUTOFF		(0x0F)	// MMA8451 HP_FILTER_CUTOFF
#define MMA8451CTRLREG1		(0x2A)	// MMA8451 CTRL_REG1
#define MMA8451CTRLREG2		(0x2B)	// MMA8451 CTRL_REG2
// MMA8451 function prototypes
uint8_t get_orientation(uint8_t, float*, float*, float*);
uint8_t init_MMA8451(void);
uint8_t read_MMA8451(uint8_t, uint8_t, uint8_t*, uint8_t);
uint8_t write_MMA8451(uint8_t, uint8_t, uint8_t);

// OLED display
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
void init_OLED(uint8_t);
void on_OLED(uint8_t);
void off_OLED(uint8_t);
void write_OLED(uint8_t, uint8_t, uint8_t);
void writestr_OLED(uint8_t, char*, uint8_t);

// MCP23008 defines
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

#define BUFSIZE 128			// Buffer size for USARTn
typedef struct {
	uint8_t					// Serial I/O buffer
		data[BUFSIZE],		// Data to send or data received
		head,				// Ring buffer head index
		tail,				// Ring buffer tail index
		nbytes,				// Number of data bytes in data[];
		nxfrd;				// Temporary counter (number of bytes transferred)
	uint8_t	volatile done;	// Is the transfer complete ('\r' seen)?
} USARTBuf;

extern USARTBuf		// Defined in usart.c
	send0_buf, send1_buf, send3_buf,
	recv0_buf, recv1_buf, recv3_buf;

// Function Prototypes
void initialize(void);		// defined in initialize.c
void init_PORTS(void);		// Defined in ports.c
void init_LED(void);
void off_LED(void);
void on_LED(void);
void toggle_LED(void);
// Function prototypes
void init_BEEPER(void);
void off_BEEPER(void);
void on_BEEPER(void);
void toggle_BEEPER(void);

#define DS3231ADDR	(0xD0)	// Day/Time clock TWI address
// Day/time clock DS3231
void convert_ds2iso(char*, uint8_t*);
void convert_iso2ds(uint8_t *, char*);
uint8_t get_time(char*);
uint8_t put_time(char*);
uint8_t read_DS3231(uint8_t, uint8_t*);
uint8_t uint8_to_bcd(uint8_t);
uint8_t write_DS3231(uint8_t, uint8_t*);

// MCP23008 addresses
#define PNEUSENSORS		(0x42)	// GMR sensors on pneumatic cylinders
#define HIGHCURRENT		(0x48)	// High current driver for pneumatic valves
//#define HIGHCURRENT		(0x40)	// test device is at 0x40

// Pneu.c Valve actions
#define SHUTTERBM		(0x22)	// OR existing value with this first, then
#define SHUTTEROPEN		(0xCE)	// AND with this pattern to open
#define SHUTTERCLOSE	(0xEC)	// AND with this pattern to close

#define LEFTBM			(0x44)	// OR existing value with this, then
#define LEFTOPEN		(0xAE)	// AND with this pattern to open
#define LEFTCLOSE		(0xEA)	// AND with this pattern to close

#define RIGHTBM			(0x88)	// OR existing value with this, then
#define RIGHTOPEN		(0x6E)	// AND with this pattern to open
#define RIGHTCLOSE		(0xE6)	// AND with this pattern to close

// pneu.c Function prototypes
void read_pneusensors(char*, char*, char*, char*);
//char read_shutterstate();
uint8_t init_PNEU(void);
uint8_t close_PNEU(char);
uint8_t open_PNEU(char);
uint8_t set_valves(uint8_t, uint8_t);

void reboot(void);			// Defined in wdt.c

#endif
