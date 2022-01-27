//Sample using LiquidCrystal library
#include <LiquidCrystal.h>

// select the pins used on the LCD panel
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);


// GPIO allocations
#define ADC_pin     (A0)
#define BUZZER_PIN  (A1)
#define LED_PIN     (A3)


// define some values used by the panel and buttons
int key_sensed  = 0;
int adc_key_in  = 0;

#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5


// read the buttons
int read_LCD_buttons (void) {
    
  adc_key_in = analogRead(ADC_pin);      // read the value from the sensor
  // my buttons when read are centered at these valies: 0, 144, 329, 504, 741
  // we add approx 50 to those values and check to see if we are close
  if (adc_key_in > 1000) return btnNONE; // We make this the 1st option for speed reasons since it will be the most likely result
  if (adc_key_in < 50)   return btnRIGHT;
  if (adc_key_in > 65 && adc_key_in < 195)  return btnUP;
  if (adc_key_in < 380)  return btnDOWN;
  if (adc_key_in < 555)  return btnLEFT;
  if (adc_key_in < 790)  return btnSELECT;
  return btnNONE;  // when all others fail, return this...
  
}

//------------------------------------------------
// Practical observations:
// Pin High --> Buzzer  ON
// pin Low  --> Buzzer  OFF
//------------------------------------------------
#define BUZZ_ON             (0)
#define BUZZ_OFF            (1)

//------------------------------------------------
// Pin High --> LED  ON
// pin Low  --> LED  OFF
//------------------------------------------------
#define LED_ON              (1)
#define LED_OFF             (0)

#define HIGH_BEEP           (200)
#define LOW_BEEP            ( 50)
#define SYS_ON_BEEP         (555)
#define SYS_OFF_BEEP        (111)


#define LCD_CHARS_MAX		(16)
#define LCD_NO_ROWS 		(2)


#define	DEBOUNCE_TIMEOUT	(55)


// variable definitions
char	key_str[LCD_CHARS_MAX];
char	button_pressed = btnNONE;
char	debounce_delay;
char	f_do_once;


void beep_for (int usecs) {

  digitalWrite(BUZZER_PIN,  BUZZ_ON);
  delayMicroseconds (usecs);
  digitalWrite(BUZZER_PIN,  BUZZ_OFF);

}

void blink_LED (int usecs) {

  digitalWrite(LED_PIN,  LED_ON);
  delayMicroseconds (usecs);
  digitalWrite(LED_PIN,  LED_OFF);

}

void setup ()    {

  // start serial port at 115200 bps and wait for port to open:
  Serial.begin (115200);

  lcd.begin(16, 2);              // start the library
  lcd.setCursor(0, 0);
  lcd.print("Push the buttons"); // print a simple message

  pinMode(BUZZER_PIN,   OUTPUT);
  pinMode(LED_PIN,      OUTPUT);

  beep_for (100);

}



void loop (void) {

	char	c;
	char	str[10];

  // if we get a valid byte, read analog ins:
  if (Serial.available() > 0) {
		// get incoming byte:
		c = Serial.read();
		
		//Serial.print("-");
		Serial.print(c);
		lcd.setCursor(0, 1);          // move cursor to second line "1" and 9 spaces over
		sprintf (str, "%c", c);
		lcd.print (str);       // display analog value

  }
  
  lcd.setCursor(9, 1);          // move cursor to second line "1" and 9 spaces over
  //lcd.print(millis()/1000);   // display seconds elapsed since power-up
  lcd.print ("      ");         // display analog value
  lcd.setCursor(9, 1);          // move cursor to second line "1" and 9 spaces over
  lcd.print (adc_key_in);       // display analog value

  lcd.setCursor(0, 1);           // move to the beginning of the second line
  key_sensed = read_LCD_buttons();  // read the buttons

  switch (key_sensed)               // depending on which button was pushed, we perform an action
  {
    case btnRIGHT:
        sprintf(key_str, "RIGHT ");
        break;
    case btnLEFT:
        sprintf(key_str, "LEFT  ");
        break;
    case btnUP:
        sprintf(key_str, "UP    ");
        break;
    case btnDOWN:
        sprintf(key_str, "DOWN  ");
        break;
    case btnSELECT:
        sprintf(key_str, "SELECT");
        break;
    case btnNONE:
        sprintf(key_str, "NONE ");
        break;
      
  }

	if (key_sensed != btnNONE) {
		debounce_delay++;
		  
		if (debounce_delay >= DEBOUNCE_TIMEOUT)	{
			button_pressed = key_sensed;
			Serial.println (key_str);
			beep_for  (33);
			lcd.print (key_str);
			blink_LED (33);
		}
	}
	else {
		if (debounce_delay)	{
			debounce_delay--;
			f_do_once = 1;
		}
		else if (f_do_once)	{
			f_do_once = 0;
			button_pressed = btnNONE;
			sprintf(key_str, "NONE ");
			Serial.println (key_str);
			lcd.print (key_str);
		}
	}

}
