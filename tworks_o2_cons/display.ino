

#include "display.h"
//#include "LedControl.h"


char     lcd_temp_string[LCD_COLS + 1];
/* we always wait a bit between updates of the display */
unsigned long delaytime = 50;



void lcd_clear_buf (char * bufp) {

    int   i;

    if (bufp) {
        for (i = 0; i < LCD_COLS; i++)
            bufp[i] = ' ';
    }
    bufp[i] = '\0';
}



//==================== 7 segment driver ========================
#define NO_OP     (0x00)
#define DIGIT_0   (0x01)
#define DIGIT_1   (0x02)
#define DIGIT_2   (0x03)
#define DIGIT_3   (0x04)
#define DIGIT_4   (0x05)
#define DIGIT_5   (0x06)
#define DIGIT_6   (0x07)
#define DIGIT_7   (0x08)
#define DECODE_MODE   (0x09)
#define INTENSITY     (0x0A)
#define SCAN_LIMIT    (0x0B)
#define SHUT_DOWN     (0x0C)
#define DISPLAY_TEST  (0x0F)



#define BLANK         (0x0F)

// Local driver defines
#define DECIMAL_POINT (0x0A)
#define BLANK_DIGIT   (0x0B)


uint8_t   digit_to_seg_value[] = {
  
    0b01111110,  // '0' 
    0b00110000,  // '1' 
    0b01101101,  // '2' 
    0b01111001,  // '3' 
    0b00110011,  // '4' 
    0b01011011,  // '5' 
    0b01011111,  // '6' 
    0b01110000,  // '7' 
    0b01111111,  // '8' 
    0b01111011,  // '9' 
    0b10000000,  // '.' -- 0x0A prints decimal point
    0b10000000,  // ' ' -- 0x0B blanks digit
  
};

//#define  DIGIT_0    0b01111110  // '0' 
//#define  DIGIT_1    0b00110000  // '1' 
//#define  DIGIT_2    0b01101101  // '2' 
//#define  DIGIT_3    0b01111001  // '3' 
//#define  DIGIT_4    0b00110011  // '4' 
//#define  DIGIT_5    0b01011011  // '5' 
//#define  DIGIT_6    0b01011111  // '6' 
//#define  DIGIT_7    0b01110000  // '7' 
//#define  DIGIT_8    0b01111111  // '8' 
//#define  DIGIT_9    0b01111011  // '9' 
//#define  POINT      0b10000000  // '.' -- 0x0A prints decimal point
//#define  BLANK      0b10000000  // ' ' -- 0x0B blanks digit

    
void  set7segmentDigit (int digit, int value, uint8_t  point) {

    if (point == true)
        point = 0x80;
    else 
        point = 0;
        
    digitalWrite(loadPin_7segment,   LOW);        
    shiftOut (dataPin_7segment, clckPin_7segment, MSBFIRST, digit);
    shiftOut (dataPin_7segment, clckPin_7segment, MSBFIRST, digit_to_seg_value[value] | point);     

    digitalWrite(loadPin_7segment,   HIGH);         
}

void  set7segmentRegister (int reg, int value) {

    digitalWrite(loadPin_7segment,   LOW);  
    shiftOut (dataPin_7segment, clckPin_7segment, MSBFIRST, reg);
    shiftOut (dataPin_7segment, clckPin_7segment, MSBFIRST, value);     

    digitalWrite(loadPin_7segment,   HIGH);         
}


void init_7segments (void) {
  /*
   The MAX72XX is in power-saving mode on startup,
   we have to do a wakeup call
   */
//  lc.shutdown (0, false);
//  /* Set the brightness to a medium values */
//  lc.setIntensity (0, 0x8);
//  /* and clear the display */
//  lc.clearDisplay (0);



/*
 * D15 D14 D13 D12 D11 D10 D9 D8 D7 D6 D5 D4 D3 D2 D1 D0
   X   X   X   X   ADDRESS-----| MSB     DATA        LSB
 */

    //shiftOut(dataPin, clockPin, bitOrder, value);

 //DBG_PRINTLN("shifting bits...");
 
    
    set7segmentRegister (SHUT_DOWN, 1);
    set7segmentRegister (DECODE_MODE, 0x00);  // 0xFF);
    set7segmentRegister (INTENSITY,   0x0F);
    set7segmentRegister (SCAN_LIMIT,   0x07);



//    set7segmentDigit (1, 0); 
//    set7segmentDigit (2, 1);
//    set7segmentDigit (3, 2);
//    set7segmentDigit (4, 3);
//    set7segmentDigit (5, 4);
//    set7segmentDigit (6, 5);
//    set7segmentDigit (7, 6);
//    set7segmentDigit (8, 7);


  //DBG_PRINTLN("shifting done..");   
  
}

///*
// This method will display the characters for the
// word "Arduino" one after the other on digit 0. 
// */
//void display_banner (void) {
//
//    /*
//     * a = 'a'      j = 
//     * b =          k = 
//     * c =          l = 
//     * d = 'd'      m =
//     * e =          n = 
//     * f =
//     * g =
//     * h =
//     * i = 
//     */
//    
//    lc.setRow(0,0,B00010000);
//    delay(delaytime);
//    lc.setRow(0,1,B00010000);
//    delay(delaytime);
//    lc.setRow(0,2,B00010000);
//    delay(delaytime);
//    lc.setRow(0,3,B00010000);
//    delay(delaytime);
//    lc.setRow(0,4,B00010000);
//    delay(delaytime);
//    lc.setRow(0,5,B00010000);
//    delay(delaytime);
//    lc.setRow(0,6,B00010000);
//    delay(delaytime);
//    lc.setRow(0,7,B00010000);
//    delay(delaytime);
//    
//} 
//
///*
//  This method will scroll all the hexa-decimal
// numbers and letters on the display. You will need at least
// four 7-Segment digits. otherwise it won't really look that good.
// */
//void scrollDigits (void) {
//    
//    for(int i = 0; i < 13; i++) 
//    {
//        lc.setDigit(0, 7, 8, false);
//        lc.setDigit(0, 6, 8, false);
//        lc.setDigit(0, 5, 8, false);
//        lc.setDigit(0, 4, 8, false);
//        lc.setDigit(0, 3, 8, false);
//        lc.setDigit(0, 2, 8, false);
//        lc.setDigit(0, 1, 8, false);
//        lc.setDigit(0, 0, 8, false);
//        delay(delaytime);
//    }
//    
//    lc.clearDisplay(0);
//    delay(delaytime);
//  
//}
//
//void test_7segments (void) { 
//    
//  display_banner();
//  scrollDigits();
//  
//}

//#define TOTAL_DIGITS    (7)
//#define DIGIT_VALUE_MAX (9)
//
//
//void disp_digit_on_7seg (uint8_t place, uint8_t value)   {
//
//    if (place > TOTAL_DIGITS  &&  value > DIGIT_VALUE_MAX)  {
//        // invalid parameters
//        return;
//    }
//
//    lc.setDigit (0, place, value, false);
//    
//}




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
    
    
//    lc.setDigit(0, 0, tens_digit,    false);
//    lc.setDigit(0, 1, unit_digit,    true);
//    lc.setDigit(0, 2, decimal_digit, false);
    set7segmentDigit (1, tens_digit, false);
    //set7segmentDigit (2, BLANK_DIGIT);    
    set7segmentDigit (2, unit_digit, true);
    set7segmentDigit (3, decimal_digit, false);
    
    
}

void display_total_run_hours (uint32_t runhours) {

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


    set7segmentDigit (4, ten_th_digit, false);
    set7segmentDigit (5, thnd_digit, false);
    set7segmentDigit (6, hund_digit, false);
    set7segmentDigit (7, tens_digit, false);
    set7segmentDigit (8, unit_digit, false);
         
//    // 10,000th digit
//    if (ten_th_digit) {
//
//    }
//    else {
//      set7segmentDigit (4, BLANK, false);
//    }
//    
//    // 1000th digit
//    if (thnd_digit) {
//      // lc.setDigit(0, 5, thnd_digit, false);
//      set7segmentDigit (5, thnd_digit, false);
//    }
//    else {
//      //lc.setRow(0, 5, 0b00000000);
//      set7segmentDigit (5, BLANK, false);
//    }
//
//    // 100th digit
//    //lc.setDigit(0, 5, hund_digit,   true);
//    if (hund_digit) {
//      //lc.setDigit(0, 6, hund_digit, false);
//      set7segmentDigit (6, hund_digit, false);
//    }
//    else {
//      //lc.setRow(0, 6, 0b00000000);
//      set7segmentDigit (6, BLANK, false);
//    }
//
//    // 10th digit
//    // lc.setDigit(0, 6, tens_digit,   true);
//    //lc.setDigit(0, 6, tens_digit,   false);
//    if (tens_digit) {
//      //lc.setDigit(0, 7, tens_digit, false);
//      set7segmentDigit (7, tens_digit, false);
//    }
//    else {
//      //lc.setRow(0, 7, 0b00000000);
//      set7segmentDigit (7, BLANK_DIGIT, false);
//    }
//
//    // unit's digit
//    // lc.setDigit(0, 8, unit_digit,   false);   
//    set7segmentDigit (8, unit_digit, false);
        
}

void display_current_run_hours (uint16_t hours, uint16_t mins) {

    uint8_t     digit1, digit2, digit3, digit4, digit5;

    // validate parameters
    mins  = mins % 60;
    hours = hours % 99;


    digit1    = hours % 10;
    hours     = hours / 10;
    digit2    = hours % 10;

    // digit3 = blank
    // : 
    digit4    = mins % 10;
    mins      = mins / 10;
    digit5    = mins % 10;

    

    
    // digit 1
    set7segmentDigit (4, digit2, false);
    // digit 2
    set7segmentDigit (5, digit1, false);
    // digit 3
    set7segmentDigit (6, BLANK_DIGIT, true);
    // digit 4
    set7segmentDigit (7, digit5, false);
    // digit 5
    set7segmentDigit (8, digit4, false);


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


////// scratch pad ////////////////////////////

/*
 
 This method will display the characters for the
 word "Arduino" one after the other on digit 0. 
/*
void display_banner (void) {

    //
     * a = 'a'      j = 
     * b =          k = 
     * c =          l = 
     * d = 'd'      m =
     * e =          n = 
     * f =
     * g =
     * h =
     * i = 
     
    
    lc.setChar(0,0,'a',false);
    delay(delaytime);
    lc.setRow(0,0,0x05);
    delay(delaytime);
    lc.setChar(0,0,'d',false);
    delay(delaytime);
    lc.setRow(0,0,0x1c);
    delay(delaytime);
    lc.setRow(0,0,B00010000);
    delay(delaytime);
    lc.setRow(0,0,0x15);
    delay(delaytime);
    lc.setRow(0,0,0x1D);
    delay(delaytime);
    lc.clearDisplay(0);
    delay(delaytime);
    
} 
 */


    
