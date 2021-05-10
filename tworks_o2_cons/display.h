// display.h

#ifndef _DISPLAY_h
#define _DISPLAY_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif


#include "platform.h"

// include the library code:
#include <LiquidCrystal.h>


LiquidCrystal lcd(RS, EN, D4, D5, D6, D7);


// initialize the library by associating any needed LCD interface pin
// as per aurdino nano - base board,  SCH Dated: 09-02-2021









#endif
