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

#include "globals.h"
#include "twi.h"
#include "oled.h"

uint16_t timerOLED, timeoutOLED;	// Used to turn off the display

/*------------------------------------------------------------------------------
void clear_OLED(uint8_t displaynumber)
	Clears the OLED screen by writing blank spaces to both lines.
	
	TRY DOING writestr_OLED with zero length string since writestr pads w/blanks
------------------------------------------------------------------------------*/
void clear_OLED(uint8_t displaynumber)
{

	char nullstr[] = "";
	writestr_OLED(displaynumber, nullstr, 1);
	writestr_OLED(displaynumber, nullstr, 2);
	timerOLED = 0;

}

/*------------------------------------------------------------------------------
void init_OLED(void)
	Set up the two OLED displays
------------------------------------------------------------------------------*/
void init_OLED(void)
{

	reset_OLED();				// Need to start with a reset
	init_OLEDDISPLAY(0);
	init_OLEDDISPLAY(1);
	_delay_ms(100);				// Wait after display-on command

}

/*------------------------------------------------------------------------------
void init_OLEDDISPLAY(uint8_t displaynumber)
	Initialize a Newhaven NHD-0216AW-1B3 OLED display. This is taken straight
	from https://github.com/NewhavenDisplay/NHD_US2066. The manual is not easy
	to understand but this seems to work.
------------------------------------------------------------------------------*/
void init_OLEDDISPLAY(uint8_t displaynumber)
{

	uint8_t twiaddr;

	if (displaynumber == 0) {			// Right-side display
		twiaddr = OLEDADDR0;		
	} else if (displaynumber == 1) {	// Left-side display
		twiaddr = OLEDADDR1;
	} else {
		return;
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

}

/*------------------------------------------------------------------------------
void reset_OLED(void)
	Pulls the OLED display reset line low for 1 ms. This seems to be needed.
------------------------------------------------------------------------------*/
void reset_OLED(void)
{
	PORTD.OUTCLR = PIN6_bm;		// PD6 is the /RESET pin for the OLED displays
	PORTD.DIRSET = PIN6_bm;
	_delay_ms(1);				// Reset time
	PORTD.OUTSET = PIN6_bm;
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
	the device at TWI address (0x3c or displaynumber = 0) or the device at
	TWI address (0x3d or displaynumber = 1).
	
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
	if (strlen(strbuf) > 16) {
		strbuf[16] = '\0';
	}
	strcat(strbuf, blanks);		// pad with blanks

	if (lineno == 1) {
		write_OLED(twiaddr, OLEDCMD, OLEDLINE1);

	} else {
		write_OLED(twiaddr, OLEDCMD, OLEDLINE2);
	}

	for (i = 0; i < 16; i++) {
		write_OLED(twiaddr, OLEDDATA, strbuf[i]);
	}

	timerOLED = 1;

}

