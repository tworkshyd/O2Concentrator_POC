// ui.c


#include "platform.h"
#include "ui.h"





void ui_init (void) {
    
    // Print a message to the LCD.
    lcd.setCursor(0, 0);
    lcd.print("Welcome to Tworks");
    lcd.setCursor(0, 1);
    lcd.print("Hyderabad");
    lcd.setCursor(0, 2);
    lcd.print("Oxygen Concentrator!");
    
}
