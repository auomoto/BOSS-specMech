/*------------------------------------------------------------------------------
test.c
------------------------------------------------------------------------------*/

#ifndef TESTC
#define TESTC

#include "oled.c"

// Function prototypes
void test_OLED(uint8_t*);

void test_oLED(uint8_t *ptr)
{

	ptr++;
	writestr_OLED(0, ptr, 1);

}

void test_OLED(uint8_t *ptr)
{

	ptr++;
	writestr_OLED(0, ptr, 2);

}

#endif
