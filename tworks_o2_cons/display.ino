// display.ino

#include "display.h"


char     lcd_temp_string[LCD_COLS + 1];
/* we always wait a bit between updates of the display */
unsigned long delaytime = 50;



void lcd_clear_buf (char * bufp) {

    int   i;

    if (bufp) {
        for (i = 0; i < LCD_COLS; i++)
        {
            bufp[i] = ' ';
        }
    }
    bufp[i] = '\0';
}




//==================== Neo-pixel LED driver ====================
/*
 * JP5 of display board of rev1.0 SCH,
 * --------------------------------------------------
 *      MISO    PD6     PD7     |       output
 * --------------------------------------------------
 *      0       0       0       |   all off
 *      0       0       1       |   Alarm-1
 *      0       1       0       |   Alarm-2
 *      0       1       1       |   Alarm-3
 *      1       0       0       |   Alarm-4
 *      1       0       1       |   Alarm-5
 *      1       1       0       |   Current Run Time
 *      1       1       1       |   Total Run Time
 * --------------------------------------------------
*/
//#define     NEO_PXL_ALL_OFF             (0b000)
//#define     NEO_PXL_ALARM_1             (0b001)
//#define     NEO_PXL_ALARM_2             (0b010)
//#define     NEO_PXL_ALARM_3             (0b011)
//#define     NEO_PXL_ALARM_4             (0b100)
//#define     NEO_PXL_ALARM_5             (0b101)
//#define     NEO_PXL_CURR_RUN_TIME       (0b110)
//#define     NEO_PXL_TOTAL_RUN_TIME      (0b111)

void neo_led_data_send (uint8_t  select_bits)    {

    // bit 2
    if (select_bits & 0x04) {   //0b100)    {
        digitalWrite(miso_neo_data1,    HIGH );
    }
    else {
        digitalWrite(miso_neo_data1,    LOW );
    }

    // bit 1
    if (select_bits & 0x02) {   //0b010)    {
        digitalWrite(pd6_neo_data2,    HIGH );
    }
    else {
        digitalWrite(pd6_neo_data2,    LOW );
    }

    // bit 20
    if (select_bits & 0x01) {   //0b001)    {
        digitalWrite(pd7_neo_data3,    HIGH );
    }
    else {
        digitalWrite(pd7_neo_data3,    LOW );
    }


    delay(150);
	
    
}


uint8_t tb_bit8_led_mask[8] = {

    0x00, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 
   
};


void update_neo_pixel_leds (void)    {

    static uint8_t  state;
    static uint8_t  bit_no;


    switch (state)
    {
        case 0:
            if (prev_neo_pixel_leds ^ neo_pixel_leds_byte)  {
                prev_neo_pixel_leds = neo_pixel_leds_byte;
                // update neo pixel leds
                // 1. clear all of them using clear command..
                neo_led_data_send (NEO_PXL_ALL_OFF);
                bit_no = 1;
                state++;
            }
            else {
                // nop
            }
            break;

        case 1:
            // 2. light-up one by one
            while (bit_no < 8)
            {               
                if (neo_pixel_leds_byte  &  tb_bit8_led_mask[bit_no])    {
                    neo_led_data_send (bit_no);
                    bit_no++;  
                    break;
                }  
                bit_no++;  
            }
            if (bit_no >= 7) {
                state++;
            }            
            break;

        default:
            state = 0;
            break;
    }
    
}


void neo_pixel_control (uint8_t led_no, uint8_t on_off)    {

    uint8_t     check_byte = neo_pixel_leds_byte;
    
    if (led_no == 0) {
        // handle this special case separately
        neo_pixel_leds_byte = 0;
    }    
    else if (led_no < 8) {
        if (on_off == true) {
            neo_pixel_leds_byte |=  tb_bit8_led_mask[led_no];
        }
        else {
            neo_pixel_leds_byte &= ~tb_bit8_led_mask[led_no];
        }
    }
    else {
        // nop
    }
    
}



void neo_pixel_leds_test (void)	{

	int state = 0;
	
	switch (state)
	{
		case 0:	
			neo_pixel_control (NEO_PXL_ALL_OFF       ,  OFF_LED);
			neo_pixel_control (NEO_PXL_ALL_OFF       ,  ON_LED);
			DBG_PRINTLN ("NEO_PXL_ALL");
			break;		
		case 1:	
			neo_pixel_control (NEO_PXL_ALARM_1       ,  OFF_LED);
			neo_pixel_control (NEO_PXL_ALARM_1       ,  ON_LED);
			DBG_PRINTLN ("NEO_PXL_ALARM_1");
			break;
		case 2:	
			neo_pixel_control (NEO_PXL_ALARM_2       ,  OFF_LED);
			neo_pixel_control (NEO_PXL_ALARM_2       ,  ON_LED);
			DBG_PRINTLN ("NEO_PXL_ALARM_2");
			break;
		case 3:	
			neo_pixel_control (NEO_PXL_ALARM_3       ,  OFF_LED);
			neo_pixel_control (NEO_PXL_ALARM_3       ,  ON_LED);
			DBG_PRINTLN ("NEO_PXL_ALARM_3");
			break;
		case 4:	
			neo_pixel_control (NEO_PXL_ALARM_4       ,  OFF_LED);
			neo_pixel_control (NEO_PXL_ALARM_4       ,  ON_LED);
			DBG_PRINTLN ("NEO_PXL_ALARM_4");
			break;
		case 5:	
			neo_pixel_control (NEO_PXL_ALARM_5       ,  OFF_LED);
			neo_pixel_control (NEO_PXL_ALARM_5       ,  ON_LED);
			DBG_PRINTLN ("NEO_PXL_ALARM_5");
			break;
		case 6:	
			neo_pixel_control (NEO_PXL_CURR_RUN_TIME ,  OFF_LED);
			neo_pixel_control (NEO_PXL_CURR_RUN_TIME ,  ON_LED);
			DBG_PRINTLN ("NEO_PXL_CURR_RUN_TIME");
			break;
		case 7:	
			neo_pixel_control (NEO_PXL_TOTAL_RUN_TIME,  OFF_LED);
			neo_pixel_control (NEO_PXL_TOTAL_RUN_TIME,  ON_LED);
			DBG_PRINTLN ("NEO_PXL_TOTAL_RUN_TIME");
			break;
		
	}
	
	delay (1000);

}



//==================== 7 segment driver ========================
#define NO_OP           (0x00)
#define DIGIT_0         (0x01)
#define DIGIT_1         (0x02)
#define DIGIT_2         (0x03)
#define DIGIT_3         (0x04)
#define DIGIT_4         (0x05)
#define DIGIT_5         (0x06)
#define DIGIT_6         (0x07)
#define DIGIT_7         (0x08)
#define DECODE_MODE     (0x09)
#define INTENSITY       (0x0A)
#define SCAN_LIMIT      (0x0B)
#define SHUT_DOWN       (0x0C)
#define DISPLAY_TEST    (0x0F)

#define BLANK           (0x0F)

// Local driver defines
#define DECIMAL_POINT   (0x0A)
#define BLANK_DIGIT     (0x0B)


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
    
    0b00000001,  // '-' 
    0b01001111,  // 'E' 
    0b00110111,  // 'H' 
    0b00001110,  // 'L' 
    
    0b10000000,  // '.' -- 0x0E prints decimal point
    0b00000000,  // ' ' -- 0x0F blanks digit
  
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

    if (point == true)  {
        point = 0x80;
    }
    else {
        point = 0;
    }
    
    digitalWrite(loadPin_7segment,   LOW);
	
    shiftOut (dataPin_7segment, clckPin_7segment, MSBFIRST, digit);
	
    shiftOut (dataPin_7segment, clckPin_7segment, MSBFIRST, digit_to_seg_value[value] | point);     	

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
	
	set7segmentDigit (1, BLANK, false);
	set7segmentDigit (2, BLANK, false);
	set7segmentDigit (3, BLANK, false);
	set7segmentDigit (4, BLANK, false);
	set7segmentDigit (5, BLANK, false);
// 	set7segmentDigit (6, BLANK, false);
// 	set7segmentDigit (7, BLANK, false);
// 	set7segmentDigit (8, BLANK, false);
	
}



void test_7segments (void)	{
	
	int		dly = 3000;
	
	
	DBG_PRINTLN ("test_7segments(). entry..");

	// digit 1
	set7segmentDigit (1, 0, true);
	
	DBG_PRINTLN ("1.");

	// digit 2
	set7segmentDigit (2, 1, false);
	// digit 3
	set7segmentDigit (3, 2, false); // to display '.'
	//set7segmentDigit (6, _HYPHEN, false);    // to display '-'
	// digit 4
	set7segmentDigit (4, 3, false);
	// digit 5
	set7segmentDigit (5, 4, false);
	delay (dly);
	
	// digit 1
	set7segmentDigit (1, 1, false);
	// digit 2
	set7segmentDigit (2, 2, true);
	// digit 3
	set7segmentDigit (3, 3, false); // to display '.'
	//set7segmentDigit (6, _HYPHEN, false);    // to display '-'
	// digit 4
	set7segmentDigit (4, 4, false);
	// digit 5
	set7segmentDigit (5, 5, false);
	delay (dly);
		
	// digit 1
	set7segmentDigit (1, 6, false);
	// digit 2
	set7segmentDigit (2, 7, false);
	// digit 3
	set7segmentDigit (3, 8, true); // to display '.'
	//set7segmentDigit (6, _HYPHEN, false);    // to display '-'
	// digit 4
	set7segmentDigit (4, 9, false);
	// digit 5
	set7segmentDigit (5, 0, false);
	delay (dly);
	
	// digit 1
	set7segmentDigit (1, 7, false);
	// digit 2
	set7segmentDigit (2, 8, false);
	// digit 3
	set7segmentDigit (3, 9, false); // to display '.'
	//set7segmentDigit (6, _HYPHEN, false);    // to display '-'
	// digit 4
	set7segmentDigit (4, 0, true);
	// digit 5
	set7segmentDigit (5, 1, false);
	delay (dly);
	
	// digit 1
	set7segmentDigit (1, 8, false);
	// digit 2
	set7segmentDigit (2, 9, false);
	// digit 3
	set7segmentDigit (3, 0, false); // to display '.'
	//set7segmentDigit (6, _HYPHEN, false);    // to display '-'
	// digit 4
	set7segmentDigit (4, 1, false);
	// digit 5
	set7segmentDigit (5, 2, true);
	delay (dly);
	
	DBG_PRINTLN ("test_7segments(). exit..");

		
}

// ver2: Display 2.0 digits for O2 concentration
// void display_o2 (float o2value) {
// 
//     uint16_t    int_o2value;
//     uint8_t     unit_digit;
//     uint8_t     tens_digit;
//     
// 
//     int_o2value   = round (o2value);
//     unit_digit    = int_o2value % 10;
//     int_o2value   = int_o2value / 10;   
//     tens_digit    = int_o2value % 10;
//     
// 
//     // b: only last 2 digits integer value display by blanking the leading digit.
//     set7segmentDigit (1, BLANK_DIGIT, false);
//     set7segmentDigit (2, tens_digit,  false);
//     set7segmentDigit (3, unit_digit,  false);    
//     
// }




// void display_total_run_hours (uint32_t runhours) {
// 
//     uint8_t     ten_th_digit, thnd_digit, hund_digit, tens_digit, unit_digit;
// 
//     unit_digit    = runhours % 10;
//     runhours      = runhours / 10;
//     tens_digit    = runhours % 10;
//     runhours      = runhours / 10;
//     hund_digit    = runhours % 10;
//     runhours      = runhours / 10;
//     thnd_digit    = runhours % 10;
//     runhours      = runhours / 10;    
//     ten_th_digit  = runhours % 10;
//     runhours      = runhours / 10;
// 
// 
//     set7segmentDigit (4, ten_th_digit, false);
//     set7segmentDigit (5, thnd_digit,   false);
//     set7segmentDigit (6, hund_digit,   false);
//     set7segmentDigit (7, tens_digit,   false);
//     set7segmentDigit (8, unit_digit,   false);
//          
// }


// void display_current_run_hours (uint16_t hours, uint16_t mins) {
// 
//     uint8_t     digit1, digit2, digit3, digit4, digit5;
// 
//     // validate parameters
//     mins  = mins % 60;
//     hours = hours % 99;
// 
// 
//     digit1    = hours % 10;
//     hours     = hours / 10;
//     digit2    = hours % 10;
// 
//     // digit3 = blank
//     // : 
//     digit4    = mins % 10;
//     mins      = mins / 10;
//     digit5    = mins % 10;
// 
//     
//     // digit 1
//     set7segmentDigit (4, digit2, false);
//     // digit 2
//     set7segmentDigit (5, digit1, false);
//     // digit 3
//     set7segmentDigit (6, BLANK_DIGIT, true); // to display '.'
//     //set7segmentDigit (6, _HYPHEN, false);    // to display '-'
//     // digit 4
//     set7segmentDigit (7, digit5, false);
//     // digit 5
//     set7segmentDigit (8, digit4, false);
// 
// 
// }

void display_current_run_hours (uint16_t hours, uint16_t mins) {

	uint8_t     digit1, digit2, digit3, digit4, digit5;
	
	
	// init_7segments ();


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
	set7segmentDigit (1, digit2, false );
	// digit 2
	set7segmentDigit (2, digit1, false);
	// digit 3
	set7segmentDigit (3, BLANK, true); // to display '.'
	//set7segmentDigit (6, _HYPHEN, false);    // to display '-'
	// digit 4
	set7segmentDigit (4, digit5, false);
	// digit 5
	set7segmentDigit (5, digit4, false);


}





void    display_task (void) {


     if (f_system_running)   {
//         display_o2 (o2_concentration);
//         if (f_run_hours == 1) {
            // display current run hours : CRN
            int secs = ( current_run_time_secs %  60);
            int mins = ((current_run_time_secs % (60 * 60)) / 60);
            int hrs  = ( current_run_time_secs / (60 * 60));
            display_current_run_hours(hrs, mins);  

//             neo_pixel_control (NEO_PXL_TOTAL_RUN_TIME,  OFF_LED);  
//             neo_pixel_control (NEO_PXL_CURR_RUN_TIME, ON_LED);  
//         }
//         else  {
//             // display total run hours : TRN
//             int hrs = (total_run_time_secs / (60 * 60));
//               
//             display_total_run_hours(hrs);
//             neo_pixel_control (NEO_PXL_CURR_RUN_TIME, OFF_LED);  
//             neo_pixel_control (NEO_PXL_TOTAL_RUN_TIME,  ON_LED);  
//         }
    }

}













////// scratch pad ////////////////////////////
