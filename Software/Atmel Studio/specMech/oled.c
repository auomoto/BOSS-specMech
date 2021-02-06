/*------------------------------------------------------------------------------
oled.c
	Routines for the Newhaven NHD-0216AW-1B3 OLED display:
	(https://drive.google.com/file/d/1IUcGcaOP8XE-vmXZrItMla3q6HhXh7-s/view)
	This is a small 2-line 16-character blue OLED display with an I2C (TWI)
	interface.

	Two TWI (I2C) addresses are allowed: (0x3c<<1) and (0x3d<<1). Outside
	this file (the writestr_OLED routine), these are referred to as displays
	0 and 1 respectively. There are only two choices for this display.
------------------------------------------------------------------------------*/

#ifndef OLEDC
#define OLEDC

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

#include "twi.c"

// Function Prototypes
void clear_OLED(uint8_t);
void init_OLED(uint8_t);
void write_OLED(uint8_t, uint8_t, uint8_t);
void writestr_OLED(uint8_t, char*, uint8_t);

/*------------------------------------------------------------------------------
void clear_OLED(uint8_t displaynumber)
	Clears the OLED screen by writing blank spaces to both lines.
------------------------------------------------------------------------------*/
void clear_OLED(uint8_t displaynumber)
{

	char blanks[] = "                ";	// Does const put this in flash?

	writestr_OLED(displaynumber, blanks, 1);
	writestr_OLED(displaynumber, blanks, 2);

}

/*------------------------------------------------------------------------------
void init_OLED(uint8_t displaynumber)
	Initialize a Newhaven NHD-0216AW-1B3 OLED display. This is taken straight
	from https://github.com/NewhavenDisplay/NHD_US2066. The manual is not easy
	to understand but this seems to work.

	The displaynumber can be either 0 or 1. When the SA0 pin is grounded, you
	get displaynumber 0. A displaynumber not 0 acts on the other display.
------------------------------------------------------------------------------*/
void init_OLED(uint8_t displaynumber)
{

	uint8_t twiaddr;
/*
	if (displaynumber == 0) {
		twiaddr = OLEDADDR0;
		PORTE.OUTCLR = PIN0_bm;		// Change these for real life
		PORTE.DIRSET = PIN0_bm;		// PE0 is the /RESET pin for the OLED0 display
		_delay_ms(1);
		PORTE.OUTSET = PIN0_bm;

	} else {
		twiaddr = OLEDADDR1;
		PORTE.OUTCLR = PIN1_bm;		// Change these for real life
		PORTE.DIRSET = PIN1_bm;		// PE1 is the /RESET pin for the OLED1 display
		_delay_ms(1);
		PORTE.OUTSET = PIN1_bm;

	}
*/

	PORTD.OUTCLR = PIN6_bm;		// PD6 is the /RESET pin for the OLED displays
	PORTD.DIRSET = PIN6_bm;
	_delay_ms(1);
	PORTD.OUTSET = PIN6_bm;

	if (displaynumber == 0) {
		twiaddr = OLEDADDR0;
	} else {
		twiaddr = OLEDADDR1;
	}

	write_OLED(twiaddr, OLEDCMD, 0x2A);	// Function set (extended command set)
	write_OLED(twiaddr, OLEDCMD, 0x71);	//function selection A
//	write_OLED(twiaddr, OLEDDATA, 0x00);	// disable internal VDD regulator (2.8V I/O). data(0x5C) = enable regulator (5V I/O)
	write_OLED(twiaddr, OLEDDATA, 0x5C);	// Enable internal VDD regulator (data(0x5C) = enable regulator (5V I/O)
	write_OLED(twiaddr, OLEDCMD, 0x28);	//function set (fundamental command set)
	write_OLED(twiaddr, OLEDCMD, 0x08);	//display off, cursor off, blink off
	write_OLED(twiaddr, OLEDCMD, 0x2A);	//function set (extended command set)
	write_OLED(twiaddr, OLEDCMD, 0x79);	//OLED command set enabled
	write_OLED(twiaddr, OLEDCMD, 0xD5);	//set display clock divide ratio/oscillator frequency
	write_OLED(twiaddr, OLEDCMD, 0x70);	//set display clock divide ratio/oscillator frequency
	write_OLED(twiaddr, OLEDCMD, 0x78);	//OLED command set disabled
	write_OLED(twiaddr, OLEDCMD, 0x08);	//extended function set (2-lines)
	write_OLED(twiaddr, OLEDCMD, 0x06);	//COM SEG direction
	write_OLED(twiaddr, OLEDCMD, 0x72);	//function selection B
	write_OLED(twiaddr, OLEDDATA, 0x00);	//ROM CGRAM selection
	write_OLED(twiaddr, OLEDCMD, 0x2A);	//function set (extended command set)
	write_OLED(twiaddr, OLEDCMD, 0x79);	//OLED command set enabled
	write_OLED(twiaddr, OLEDCMD, 0xDA);	//set SEG pins hardware configuration
	write_OLED(twiaddr, OLEDCMD, 0x00);	//set SEG pins hardware configuration
	write_OLED(twiaddr, OLEDCMD, 0xDC);	//function selection C
	write_OLED(twiaddr, OLEDCMD, 0x00);	//function selection C
	write_OLED(twiaddr, OLEDCMD, 0x81);	//set contrast control
	write_OLED(twiaddr, OLEDCMD, 0x7F);	//set contrast control
	write_OLED(twiaddr, OLEDCMD, 0xD9);	//set phase length
	write_OLED(twiaddr, OLEDCMD, 0xF1);	//set phase length
	write_OLED(twiaddr, OLEDCMD, 0xDB);	//set VCOMH deselect level
	write_OLED(twiaddr, OLEDCMD, 0x40);	//set VCOMH deselect level
	write_OLED(twiaddr, OLEDCMD, 0x78);	//OLED command set disabled
	write_OLED(twiaddr, OLEDCMD, 0x28);	//function set (fundamental command set)
	write_OLED(twiaddr, OLEDCMD, 0x01);	//clear display
	write_OLED(twiaddr, OLEDCMD, 0x80);	//set DDRAM address to 0x00
	write_OLED(twiaddr, OLEDCMD, 0x0C);	// Display ON
	_delay_ms(100);				// Wait after display-on command

}

/*------------------------------------------------------------------------------
void write_OLED(uint8_t twiaddr, uint8_t type, uint8_t byteToSend)
	Write to the display at TWI address twiaddr. Two bytes are sent. The first
	is the data type, either OLEDCMD or OLEDDATA, indicating the type of the
	next byte, either a command to the controller or a character to write on
	the screen. The next byte sent is either the  command or the screen
	character.

------------------------------------------------------------------------------*/
void write_OLED(uint8_t twiaddr, uint8_t type, uint8_t byteToSend)
{

	start_TWI(twiaddr, TWIWRITE);
	write_TWI(type);
	write_TWI(byteToSend);
	stop_TWI();

}

/*------------------------------------------------------------------------------
void writestr_OLED(uint8_t displaynumber, char *str, uint8_t lineno)
	Write a string to a Newhaven OLED display. The displaynumber selects either
	the device at TWI address (0x3c<<) (displaynumber = 0) or the device at
	TWI address (0x3d<<) (displaynumber != 0).
	
	This routine positions the cursor at the beginning of a line (1 or 2) pads
	the string with blanks, then writes the full 16 characters to the display.

------------------------------------------------------------------------------*/
void writestr_OLED(uint8_t displaynumber, char *str, uint8_t lineno)
{

	uint8_t i, twiaddr;
	char strbuf[33];
	const char blanks[] = "                ";

	if (displaynumber == 0) {
		twiaddr = OLEDADDR0;
	} else {
		twiaddr = OLEDADDR1;
	}

	strcpy(strbuf, str);
	strcat(strbuf, blanks);		// pad with blanks

	if (lineno == 1) {
		write_OLED(twiaddr, OLEDCMD, OLEDLINE1);

	} else {
		write_OLED(twiaddr, OLEDCMD, OLEDLINE2);
	}

	for (i = 0; i < 16; i++) {
		write_OLED(twiaddr, OLEDDATA, strbuf[i]);
	}

}

#endif
