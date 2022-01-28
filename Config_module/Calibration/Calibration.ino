//Sample using LiquidCrystal library
#include <LiquidCrystal.h>

// select the pins used on the LCD panel
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);






// define some values used by the panel and buttons
extern int adc_key_in       ;
extern int key_detected     ;
extern int last_key_detected;



void setup ()    {

	// start serial port at 115200 bps and wait for port to open:
	Serial.begin (115200);

	ui_init ();

}



void loop (void) {

	char	c;
	char	str[10];


	// if we get a valid byte, read analog ins:
	if (Serial.available() > 0) {
		// get incoming byte:
		c = Serial.read();
		
		// Serial.print("-");
		Serial.print(c);
		lcd.setCursor(0, 1);		// move cursor to start
		sprintf (str, "%c", c);
		lcd.print (str);			// display analog value

	}
  

	// Test area -------------------------------------------------------------
/*
	lcd.setCursor (9, 1);          // move cursor to second line "1" and 9 spaces over
	//lcd.print(millis()/1000);   // display seconds elapsed since power-up
	lcd.print ("      ");         // display analog value
	lcd.setCursor(9, 1);          // move cursor to second line "1" and 9 spaces over
	lcd.print (adc_key_in);       // display analog value
	lcd.print (key_detected);       // display analog value
*/
	// -----------------------------------------------------------------------
	
	
	ui_task_main ();
	
			
}
