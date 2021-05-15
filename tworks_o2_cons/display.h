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

#define LCD_ROWS   ( 4)
#define LCD_COLS   (20)

LiquidCrystal lcd(RS, EN, D4, D5, D6, D7);



enum ALIGN {LEFT, CENTER, RIGHT};

// initialize the library by associating any needed LCD interface pin
// as per aurdino nano - base board,  SCH Dated: 09-02-2021

//  line_no (1 to LCD_ROWS), start_col ( 0 to LCD_COLS-1)
void lcd_wirte_to_line (char line_no, char start_col, char * str_ptr, char align);






#endif
