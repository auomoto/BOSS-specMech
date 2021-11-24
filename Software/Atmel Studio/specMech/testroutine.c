#include "globals.h"
#include "timers.h"
#include "usart.h"
#include "roboclaw.h"
#include "fram.h"
#include "testroutine.h"

void testroutine(void)
{

	move_MOTORAbsolute(128, 2147510715UL);
	return;
}