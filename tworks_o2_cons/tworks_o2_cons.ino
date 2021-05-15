#include "platform.h"
#include "display.h"
#include "data_base.h"
#include "ui.h"

/*

  The circuit:
 * LCD RS pin to digital pin 12
 * LCD Enable pin to digital pin 11
 * LCD D4 pin to digital pin 5
 * LCD D5 pin to digital pin 4
 * LCD D6 pin to digital pin 3
 * LCD D7 pin to digital pin 2
 * LCD R/W pin to ground
 * LCD VSS pin to ground
 * LCD VCC pin to 5V
 * 10K resistor:
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)

*/




void setup() {


    platform_init();
    

    // Print a message to the LCD.
    lcd.setCursor(0, 0);
    lcd.print("Welcome to Tworks");
    lcd.setCursor(0, 1);
    lcd.print("Hyderabad");
    lcd.setCursor(0, 2);
    lcd.print("Oxygen Concentrator!");
  
}


void loop() {
    
    // set the cursor to column 0, line 1
    // (note: line 1 is the second row, since counting begins with 0):
    lcd.setCursor(0, 3);
    // print the number of seconds since reset:
    lcd.print(millis() / 1000);
  
}
