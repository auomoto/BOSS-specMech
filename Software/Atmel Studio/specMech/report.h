#ifndef REPORTH
#define REPORTH

#include "wdt.h"
#include "usart.h"
#include "set.h"
#include "commands.h"
#include "ds3231.h"
#include "temperature.h"
#include "humidity.h"
#include "roboclaw.h"
#include "oled.h"
#include "mma8451.h"
#include "pneu.h"
#include "fram.h"
#include "ionpump.h"
#include "nmea.h"
#include "eeprom.h"
#include "errors.h"
#include "ln2.h"

uint8_t report(uint8_t);

#endif