// platform.c


#include "platform.h"
#include "display.h"


void platform_init (void) {

    // set up the LCD's number of columns and rows:
    lcd.begin(LCD_COLS, LCD_ROWS);

    pinMode(RLY_1,      OUTPUT);
    pinMode(RLY_2,      OUTPUT);
    pinMode(RLY_3,      OUTPUT);
    pinMode(RLY_4,      OUTPUT);
    pinMode(BUZZR,      OUTPUT);
    pinMode(COMPRSSR,   OUTPUT);
    
    
    
    
    
}
