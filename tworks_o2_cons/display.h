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
#include "LedControl.h"


/*
    The circuit:
    LCD RS pin to digital pin 12
    LCD Enable pin to digital pin 11
    LCD D4 pin to digital pin 5
    LCD D5 pin to digital pin 4
    LCD D6 pin to digital pin 3
    LCD D7 pin to digital pin 2
    LCD R/W pin to ground
    LCD VSS pin to ground
    LCD VCC pin to 5V
    10K resistor:
    ends to +5V and ground
    wiper to LCD VO pin (pin 3)
*/


#define LCD_ROWS   ( 4)
#define LCD_COLS   (20)

LiquidCrystal   lcd(RS, EN, D4, D5, D6, D7);
LedControl      lc = LedControl(dataPin_7segment, clckPin_7segment, loadPin_7segment, 1);


enum ALIGN {LEFT, CENTER, RIGHT};
extern char     lcd_temp_string[LCD_COLS + 1];




void lcd_clear_buf		(char * bufp);
void init_7segments		(void);
void blank_7segments	(void);
void display_banner		(void);
void scrollDigits		(void);
void test_7segments		(void);
void display_o2			(float o2value);
void display_current_run_hours   (uint16_t hours, uint16_t mins);
void display_total_run_hours    (uint32_t runhours);


// initialize the library by associating any needed LCD interface pin
// as per aurdino nano - base board,  SCH Dated: 09-02-2021

//  line_no (1 to LCD_ROWS), start_col ( 0 to LCD_COLS-1)
void lcd_wirte_to_line (char line_no, char start_col, char * str_ptr, char align);






#endif
