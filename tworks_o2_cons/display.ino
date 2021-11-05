

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
    0b00000000,  // ' ' -- 0x0B blanks digit
  
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
    shiftOut (dataPin_7segment, clckPin_7segment, MSBFIRST, digit);
    shiftOut (dataPin_7segment, clckPin_7segment, MSBFIRST, digit_to_seg_value[value] | point);     
    digitalWrite(loadPin_7segment,   LOW);
    digitalWrite(loadPin_7segment,   HIGH);         
}

void  set7segmentRegister (int reg, int value) {
  
/*
 * D15 D14 D13 D12 D11 D10 D9 D8 D7 D6 D5 D4 D3 D2 D1 D0
   X   X   X   X   ADDRESS-----| MSB     DATA        LSB
 */
    //shiftOut(dataPin, clockPin, bitOrder, value);
    shiftOut (dataPin_7segment, clckPin_7segment, MSBFIRST, reg);
    shiftOut (dataPin_7segment, clckPin_7segment, MSBFIRST, value);     
    digitalWrite(loadPin_7segment,   LOW);
    digitalWrite(loadPin_7segment,   HIGH);         
}


void init_7segments (void) {
    
    set7segmentRegister (INTENSITY, 0x07);
    set7segmentRegister (SHUT_DOWN, 1);
 
}



void blank_7segments (void) {
	
	set7segmentDigit (1, BLANK_DIGIT, false);
	set7segmentDigit (2, BLANK_DIGIT, false);
	set7segmentDigit (3, BLANK_DIGIT, false);
	set7segmentDigit (4, BLANK_DIGIT, false);
	set7segmentDigit (5, BLANK_DIGIT, false);
	set7segmentDigit (6, BLANK_DIGIT, false);
	set7segmentDigit (7, BLANK_DIGIT, false);
	set7segmentDigit (8, BLANK_DIGIT, false);
	
}


// ver1: Display 2.1 digits for O2 concentration
//void display_o2 (float o2value) {
//
//    uint16_t     int_o2value;
//    uint8_t     decimal_digit;
//    uint8_t     unit_digit;
//    uint8_t     tens_digit;
//
//    int_o2value   = (uint16_t)(o2value * 10);
//    decimal_digit = int_o2value % 10;
//    int_o2value   = int_o2value / 10;
//    unit_digit    = int_o2value % 10;
//    int_o2value   = int_o2value / 10;
//    
//    tens_digit    = int_o2value % 10;
//    
//    //  2.1 digit display for concentration
//    set7segmentDigit (1, tens_digit, false);
//    //set7segmentDigit (2, BLANK_DIGIT);    
//    set7segmentDigit (2, unit_digit, true);
//    set7segmentDigit (3, decimal_digit, false);
//
//    
//}

// ver2: Display 2.0 digits for O2 concentration
void display_o2 (float o2value) {

    uint16_t    int_o2value;
    uint8_t     unit_digit;
    uint8_t     tens_digit;
    

    int_o2value   = round (o2value);
    unit_digit    = int_o2value % 10;
    int_o2value   = int_o2value / 10;   
    tens_digit    = int_o2value % 10;
    

    // b: only last 2 digits integer value display by blanking the leading digit.
    set7segmentDigit (1, BLANK_DIGIT, false);
    set7segmentDigit (2, tens_digit,  false);
    set7segmentDigit (3, unit_digit,  false);    
    
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
    set7segmentDigit (5, thnd_digit,   false);
    set7segmentDigit (6, hund_digit,   false);
    set7segmentDigit (7, tens_digit,   false);
    set7segmentDigit (8, unit_digit,   false);
         
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


    
