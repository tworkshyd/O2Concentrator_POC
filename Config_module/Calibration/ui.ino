// ui.c


#include <string.h>

#include "ui.h"





// Variables Definitions -----------------------------------------------------
// define some values used by the panel and buttons
int adc_key_in        = btnNONE;
int key_detected      = btnNONE;
int last_key_detected = btnNONE;
// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled



// variable definitions
char	key_str[LCD_CHARS_MAX];
// char	button_pressed = btnNONE;
// char	debounce_delay;
char	f_do_once;



void ui_print_welcome (void)    {

	lcd.setCursor(0, 0);
	lcd.print("WelcomeTo Tworks");
	lcd.setCursor(0, 1);
	lcd.print("Hyderabad, India");

}

void ui_init (void) {

	lcd.begin (LCD_CHARS_MAX, LCD_NO_ROWS);              // start the library
	lcd.setCursor (0, 0);

	pinMode(BUZZER_PIN,   OUTPUT);
	pinMode(LED_PIN,      OUTPUT);

	beep_for (100);

	// Print a message to the LCD.
	ui_print_welcome ();

}



void beep_for (int msecs) {

	digitalWrite(BUZZER_PIN,  BUZZ_ON);
	delay (msecs);
	digitalWrite(BUZZER_PIN,  BUZZ_OFF);

}

void multi_beeps (int count) {

	int   i;
	if (count > 10) {
		count = 10;
	}

	for (i = 0; i < count; i++)
	{
		beep_for (50);
		delay (100);
	}

}




void blink_LED (int msecs) {

	digitalWrite(LED_PIN,  LED_ON);
	delay (msecs);
	digitalWrite(LED_PIN,  LED_OFF);

}



// read the buttons
int read_LCD_buttons (void) {
	
	adc_key_in = analogRead(ADC_pin);      // read the value from the sensor
	// my buttons when read are centered at these valies: 0, 144, 329, 504, 741
	// we add approx 50 to those values and check to see if we are close
	if (adc_key_in > 1000) return btnNONE; // We make this the 1st option for speed reasons since it will be the most likely result
	if (adc_key_in < 50)   return btnRIGHT;
	if (adc_key_in < 195)  return btnUP;
	if (adc_key_in < 380)  return btnDOWN;
	if (adc_key_in < 555)  return btnLEFT;
	if (adc_key_in < 790)  return btnSELECT;
	
	return btnNONE;  // when all others fail, return this...
	
}

void key_scan (void) {

	static int	last_key_sensed;
	int			key_sensed;
	

	key_sensed = read_LCD_buttons();  // read the buttons

	// check to see if you just pressed the button
	// (i.e. the input went from LOW to HIGH), and you've waited long enough
	// since the last press to ignore any noise:

	// If the switch changed, due to noise or pressing:
	if (key_sensed != last_key_sensed) {
		// reset the debouncing timer
		lastDebounceTime = millis();
	}

	if ((millis() - lastDebounceTime) > DEBOUNCE_DELAY) {
		// whatever the reading is at, it's been there for longer than the debounce
		// delay, so take it as the actual current state:

		// if the button state has changed:
		if (key_sensed != key_detected) {
			key_detected = key_sensed;
		}
	}

	// save the reading. Next time through the loop, it'll be the lastButtonState:
	last_key_sensed = key_sensed;
	
}





void ui_task_main (void)    {

	static int            button_debounce_delay;
	static unsigned long  time_tag;
	static unsigned int   state_time;
	char                  str_temp[6];
	char                  str_temp2[6];


	key_scan ();
	
	// if the button state has changed:
        // if the button state has changed:
    if (key_detected != last_key_detected   &&   key_detected != btnNONE) {
	    last_key_detected = key_detected;		// press detected
		switch (key_detected)					// depending on which button was pushed, we perform an action
		{
			case btnRIGHT:
				sprintf(key_str, "RIGHT     ");
				break;
			case btnLEFT:
				sprintf(key_str, "LEFT      ");
				break;
			case btnUP:
				sprintf(key_str, "UP        ");
				break;
			case btnDOWN:
				sprintf(key_str, "DOWN      ");
				break;
			case btnSELECT:
				sprintf(key_str, "SELECT    ");
				break;
			case btnNONE:
				// sprintf(key_str, "NONE ");
				sprintf(key_str, "          ");
				break;
			        
		}
		Serial.println (key_str);
		beep_for  (33);
		lcd.setCursor(0, 1);		// move cursor to start
		lcd.print (key_str);
		blink_LED (33);
	}
}
		

	
