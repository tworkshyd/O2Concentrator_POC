// platform.c


#include "platform.h"
#include "display.h"


void platform_init (void) {

    // set up the LCD's number of columns and rows:
    lcd.begin(LCD_COLS, LCD_ROWS);

    // pin mode setting
    pinMode(RLY_1,          OUTPUT);
    pinMode(RLY_2,          OUTPUT);
    pinMode(RLY_3,          OUTPUT);
    // pinMode(RLY_4,       OUTPUT);

    pinMode(BUZZR,          OUTPUT);
    pinMode(COMPRSSR,       OUTPUT);
    pinMode(buttonPin,      INPUT );
    pinMode(buttonPin,INPUT_PULLUP);
    
    // default pin-state
    digitalWrite(RLY_1,     HIGH);
    digitalWrite(RLY_2,     HIGH);
    digitalWrite(RLY_3,     HIGH);
    // igitalWrite(RLY_4,   HIGH);

    digitalWrite(BUZZR,     HIGH);
    digitalWrite(COMPRSSR,  LOW );
    
    Serial.println("GPIO init done..");

    
}
