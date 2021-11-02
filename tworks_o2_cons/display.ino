

#include "display.h"


char     lcd_temp_string[LCD_COLS + 1];



void lcd_clear_buf (char * bufp) {

    int   i;

    if (bufp) {
        for (i = 0; i < LCD_COLS; i++)
            bufp[i] = ' ';
    }
    bufp[i] = '\0';
}





void init_7segments (void) {
  /*
   The MAX72XX is in power-saving mode on startup,
   we have to do a wakeup call
   */
  lc.shutdown (0, false);
  /* Set the brightness to a medium values */
  lc.setIntensity (0, 0x8);
  /* and clear the display */
  lc.clearDisplay (0);
  
}

/*
 This method will display the characters for the
 word "Arduino" one after the other on digit 0. 
 */
void display_banner (void) {

    /*
     * a = 'a'      j = 
     * b =          k = 
     * c =          l = 
     * d = 'd'      m =
     * e =          n = 
     * f =
     * g =
     * h =
     * i = 
     */
    
    lc.setRow(0,0,B00010000);
    delay(delaytime);
    lc.setRow(0,1,B00010000);
    delay(delaytime);
    lc.setRow(0,2,B00010000);
    delay(delaytime);
    lc.setRow(0,3,B00010000);
    delay(delaytime);
    lc.setRow(0,4,B00010000);
    delay(delaytime);
    lc.setRow(0,5,B00010000);
    delay(delaytime);
    lc.setRow(0,6,B00010000);
    delay(delaytime);
    lc.setRow(0,7,B00010000);
    delay(delaytime);
    
} 

/*
  This method will scroll all the hexa-decimal
 numbers and letters on the display. You will need at least
 four 7-Segment digits. otherwise it won't really look that good.
 */
void scrollDigits (void) {
    
    for(int i = 0; i < 13; i++) 
    {
        lc.setDigit(0, 7, 8, false);
        lc.setDigit(0, 6, 8, false);
        lc.setDigit(0, 5, 8, false);
        lc.setDigit(0, 4, 8, false);
        lc.setDigit(0, 3, 8, false);
        lc.setDigit(0, 2, 8, false);
        lc.setDigit(0, 1, 8, false);
        lc.setDigit(0, 0, 8, false);
        delay(delaytime);
    }
    
    lc.clearDisplay(0);
    delay(delaytime);
  
}

void test_7segments (void) { 
    
  display_banner();
  scrollDigits();
  
}

#define TOTAL_DIGITS    (7)
#define DIGIT_VALUE_MAX (9)


/*
 *  Two LED boards Board 1 with 5 LEDs A0 to A4
 *   and  Board 2 with 2 LEDs A0 & A1 (will refere these as a0 & a1)
 *   
 *   From host controller 3 pins are used for interface 
 *    MISO, PD6 & PD7, these are connected from display board JP5 connector to 
 *    J1 connector of LED Board1, LED Board2 is cascaed with LED Board1
 *    
 */

 
#define NEO_PIXCELL_LED_COUNT   (7)
void test_neo_pixcell_leds (void) {

    int    count;

    Serial.println("Testing Neo LEDs..");

    count = 0;
    for (count = 0; count <= 7; count++)
    {
    
        digitalWrite(MISO_pin,    count & 0x04);
        digitalWrite(PD6_pin,     count & 0x02);
        digitalWrite(PD7_pin,     count & 0x01);
    
        delay(200);
        Serial.print("NEO LED no: ");
        Serial.println(count);
    }

    Serial.println("Testing Neo LEDs.. done");
    // OFF all be fore leaving
    digitalWrite(MISO_pin,    0);
    digitalWrite(PD6_pin,     0);
    digitalWrite(PD7_pin,     0);
}



void neo_pixcel_data (enum ERROR_CODE_E error_no, uint8_t  on_off) {

    static uint8_t  led_byte;
    uint8_t         code_bit;

    switch (error_no)
    {
        case UNUSED_LED:            code_bit = A0;   break;
        case UNIT_OVER_HEAT:        code_bit = A1;   break;
        case POWER_FAIL:            code_bit = A2;   break;
        case OUTPUT_FLOW_OBSTRUCT:  code_bit = A3;   break;
        case LOW_O2_PURITY:         code_bit = A4;   break;
        
        case CRN_DISPLAY:           code_bit = a0;   break;
        case TRN_DISPLAY:           code_bit = a1;   break;          

        case ALL_LEDs_OFF:              
            code_bit = 0;    
            led_byte = 0;
            break;   // temp fix, need to revisit here
        default:
          // nop
          break;
    }

    if (on_off) {
        led_byte |= code_bit;   
    }
    else {
        led_byte &= ~code_bit;   
    }
            
    digitalWrite(MISO_pin,    led_byte & 0x04);
    digitalWrite(PD6_pin,     led_byte & 0x02);
    digitalWrite(PD7_pin,     led_byte & 0x01);  
    
}


void disp_digit_on_7seg (uint8_t place, uint8_t value)   {

    if (place > TOTAL_DIGITS  &&  value > DIGIT_VALUE_MAX)  {
        // invalid parameters
        return;
    }

    lc.setDigit (0, place, value, false);
    
}

void display_o2 (float o2value) {

    uint16_t     int_o2value;
    uint8_t     decimal_digit;
    uint8_t     unit_digit;
    uint8_t     tens_digit;

    int_o2value   = (uint16_t)(o2value * 10);
    decimal_digit = int_o2value % 10;
    int_o2value   = int_o2value / 10;
    unit_digit    = int_o2value % 10;
    int_o2value   = int_o2value / 10;
    tens_digit    = int_o2value % 10;
    
    
    lc.setDigit(0, 0, tens_digit,    false);
    lc.setDigit(0, 1, unit_digit,    true);
    lc.setDigit(0, 2, decimal_digit, false);
   
    
}

void display_run_hours (uint32_t runhours) {

    uint8_t     ten_th_digit, thnd_digit, hund_digit, tens_digit, unit_digit;

    unit_digit    = runhours % 10;
    runhours      = runhours / 10;
    tens_digit    = runhours % 10;
    runhours      = runhours / 10;
    hund_digit    = runhours % 10;
    runhours      = runhours / 10;
    thnd_digit    = runhours % 10;
    runhours      = runhours / 10;    
    ten_th_digit  = runhours % 10;
    runhours      = runhours / 10;

    
    lc.setDigit(0, 3, ten_th_digit, false);
    lc.setDigit(0, 4, thnd_digit,   false);
    lc.setDigit(0, 5, hund_digit,   false);
    lc.setDigit(0, 6, tens_digit,   false);
    lc.setDigit(0, 7, unit_digit,   false);  
     
    // 10,000th digit
    if (ten_th_digit) {
      lc.setDigit(0, 3, ten_th_digit, false);
    }
    else {
      lc.setRow(0, 3, 0b00000000);
    }
    // 1000th digit
    if (thnd_digit) {
      lc.setDigit(0, 4, thnd_digit, false);
    }
    else {
      lc.setRow(0, 4, 0b00000000);
    }

    // 100th digit
    //lc.setDigit(0, 5, hund_digit,   true);
    if (hund_digit) {
      lc.setDigit(0, 5, hund_digit, false);
    }
    else {
      lc.setRow(0, 5, 0b00000000);
    }

    // 10th digit
    // lc.setDigit(0, 6, tens_digit,   true);
    //lc.setDigit(0, 6, tens_digit,   false);
    if (tens_digit) {
      lc.setDigit(0, 6, tens_digit, false);
    }
    else {
      lc.setRow(0, 6, 0b00000000);
    }

    // unit's digit
    lc.setDigit(0, 7, unit_digit,   false);   

        
}

void display_run_time (uint16_t hours, uint16_t mins) {

    uint8_t     ten_th_digit, thnd_digit, hund_digit, tens_digit, unit_digit;

    // validate parameters
    mins  = mins % 60;
    hours = hours % 999;

    unit_digit    = mins % 10;
    mins          = mins / 10;
    tens_digit    = mins % 10;
    // : 
    hund_digit    = hours % 10;
    hours         = hours / 10;
    thnd_digit    = hours % 10;
    hours         = hours / 10;    
    ten_th_digit  = hours % 10;

    
    // 10,000th digit
    if (ten_th_digit) {
      lc.setDigit(0, 3, ten_th_digit, false);
    }
    else {
      lc.setRow(0, 3, 0b00000000);
    }
    // 1000th digit
    if (thnd_digit) {
      lc.setDigit(0, 4, thnd_digit, false);
    }
    else {
      lc.setRow(0, 4, 0b00000000);
    }

    // 100th digit
    lc.setDigit(0, 5, hund_digit,   true);

    // 10th digit
    // lc.setDigit(0, 6, tens_digit,   true);
    lc.setDigit(0, 6, tens_digit,   false);

    // unit's digit
    lc.setDigit(0, 7, unit_digit,   false);   
    
}



/*

    Prameters :  line_no (1 to LCD_ROWS), start_col ( 0 to LCD_COLS-1)
                align (LEFT, CENTER, RIGHT)
*/
/*
    void lcd_wirte_to_line (char line_no, char start_col, char * str_ptr, char align)   {

    // validate prameters
    if (line_no < 1  ||  line_no > LCD_ROWS)    {
       line_no = 1;
    }

    if (start_col >= LCD_COLS)   {
       start_col = 0;
    }

    if (str_ptr == NULL)    {
       return;
    }
    // 'lcd.setCursor()' - char (0 to LCD_COLS-1),  line no (0 to LCD_ROWS-1)
    lcd.setCursor(line_no-1, start_col);

    lcd.print("Hyderabad");

    }*/
