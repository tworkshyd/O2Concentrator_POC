// display.ino

#include "display.h"
#include "db.h"


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
void neo_led_data_send (uint8_t  select_bits)    {

    // bit 2
    if (select_bits & 0x04) {  
        digitalWrite(miso_neo_data1,    HIGH );
    }
    else {
        digitalWrite(miso_neo_data1,    LOW );
    }

    // bit 1
    if (select_bits & 0x02) {  
        digitalWrite(pd6_neo_data2,    HIGH );
    }
    else {
        digitalWrite(pd6_neo_data2,    LOW );
    }

    // bit 0
    if (select_bits & 0x01) {   
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
            if (bit_no > 7) {
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
    
    if (led_no == 0) {	// all LEDs at once		
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

	static int state = 0;
	
	switch (state)
	{
		case 0:
		neo_pixel_control (NEO_PXL_ALL_OFF       ,  OFF_LED);
		DBG_PRINTLN ("NEO_PXL_ALL");
		// 			neo_pixel_control (NEO_PXL_ALL_OFF       ,  ON_LED);
		// 			DBG_PRINTLN ("NEO_PXL_ALL");
		break;
		case 1:
		// 			neo_pixel_control (NEO_PXL_LED_1       ,  OFF_LED);
		neo_pixel_control (LOW_O2C_ALARM       ,  ON_LED);
		DBG_PRINTLN ("LOW_O2C_ALARM");
		break;
		case 2:
		neo_pixel_control (LOW_O2C_ALARM       ,  OFF_LED);
		neo_pixel_control (LOW_PRESSURE_ALARM       ,  ON_LED);
		DBG_PRINTLN ("LOW_PRESSURE_ALARM");
		break;
		case 3:
		neo_pixel_control (LOW_PRESSURE_ALARM       ,  OFF_LED);
		neo_pixel_control (HIGH_TEMPER_ALARM       ,  ON_LED);
		DBG_PRINTLN ("HIGH_TEMPER_ALARM");
		break;

		
	}
	state++;
	if (state > 3)	{
		state = 0;
	}	
	
    update_neo_pixel_leds ();
    delay (333);
    update_neo_pixel_leds ();
    delay (333);
    update_neo_pixel_leds ();
    delay (333);
	
	delay(3000);
	neo_pixel_control (NEO_PXL_ALL_OFF       ,  OFF_LED);

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

  
void  set7segmentDigit (int seg_no, int value, uint8_t  point) {

    if (point == true)  {
        point = 0x80;
    }
    else {
        point = 0;
    }
    
    digitalWrite(loadPin_7segment,   LOW);
	
    shiftOut (dataPin_7segment, clckPin_7segment, MSBFIRST, seg_no);
	
    shiftOut (dataPin_7segment, clckPin_7segment, MSBFIRST, digit_to_seg_value[value] | point);     	

    digitalWrite(loadPin_7segment,   HIGH);      

}



#define	DISP_CHAR_0			(0b01111110)  // '0'
#define	DISP_CHAR_1			(0b00110000)  // '1'
#define	DISP_CHAR_2			(0b01101101)  // '2'
#define	DISP_CHAR_3			(0b01111001)  // '3'
#define	DISP_CHAR_4			(0b00110011)  // '4'
#define	DISP_CHAR_5			(0b01011011)  // '5'
#define	DISP_CHAR_6			(0b01011111)  // '6'
#define	DISP_CHAR_7			(0b01110000)  // '7'
#define	DISP_CHAR_8			(0b01111111)  // '8'
#define	DISP_CHAR_9			(0b01111011)  // '9'

#define	DISP_CHAR_HYPHEN    (0b00000001)  // '-'
#define	DISP_CHAR_A			(0b01110111)  // 'A'
#define	DISP_CHAR_E			(0b01001111)  // 'E'
#define	DISP_CHAR_H			(0b00110111)  // 'H'
#define	DISP_CHAR_L			(0b00001110)  // 'L'
#define	DISP_CHAR_P			(0b01100111)  // 'P'

#define	DISP_CHAR_POINT		(0b10000000)  // '.' -- 0x0E prints decimal point
#define DISP_SMALL_o		(0b00011101)  // 'o'
#define DISP_SMALL_t		(0b00001111)  // 't'
		// add new characters .. here..



void  set7segment_disp_char (int seg_no, char character) {

	uint8_t		value = 0;
	
	switch (character)
	{
		case '0':	value = DISP_CHAR_0		;	break;
		case '1':	value = DISP_CHAR_1		;	break;
		case '2':	value = DISP_CHAR_2		;	break;
		case '3':	value = DISP_CHAR_3		;	break;
		case '4':	value = DISP_CHAR_4		;	break;
		case '5':	value = DISP_CHAR_5		;	break;
		case '6':	value = DISP_CHAR_6		;	break;
		case '7':	value = DISP_CHAR_7		;	break;
		case '8':	value = DISP_CHAR_8		;	break;
		case '9':	value = DISP_CHAR_9		;	break;

		case '-':	value = DISP_CHAR_HYPHEN;	break;
		case 'A':	value = DISP_CHAR_A		;	break;
		case 'E':	value = DISP_CHAR_E		;	break;
		case 'H':	value = DISP_CHAR_H		;	break;
		case 'L':	value = DISP_CHAR_L		;	break;
		case 'P':	value = DISP_CHAR_P		;	break;

		case '.':	value = DISP_CHAR_POINT	;	break;
		case 'o':	value = DISP_SMALL_o	;	break;
		case 't':	value = DISP_SMALL_t	;	break;
		
		// add new characters .. here..

		default:	value = 0				;	break;
		
		
	}
	
	digitalWrite(loadPin_7segment,   LOW);
	
	shiftOut (dataPin_7segment, clckPin_7segment, MSBFIRST, seg_no);
	
	shiftOut (dataPin_7segment, clckPin_7segment, MSBFIRST, value);

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
    set7segmentRegister (SCAN_LIMIT, 0x07);
	set7segmentRegister (DISPLAY_TEST, 0);
	 
}



void blank_7segments (void) {
	
	set7segmentDigit (1, BLANK, false);
	set7segmentDigit (2, BLANK, false);
	set7segmentDigit (3, BLANK, false);
	set7segmentDigit (4, BLANK, false);
	set7segmentDigit (5, BLANK, false);
	
}



void __7seg_display_float_in_2p2_format (float o2value) {
	
    uint16_t    integer;
    uint8_t     unit_digit;
    uint8_t     tens_digit;
    uint8_t     decm_digit1;
    uint8_t     decm_digit2;

	//     int_o2value   = round (o2value);
	integer   = (unsigned int) (o2value * 100.0);
	
	decm_digit2   = integer % 10;
	integer       = integer / 10;
	decm_digit1   = integer % 10;
	integer       = integer / 10;
	unit_digit    = integer % 10;
	integer       = integer / 10;
	tens_digit    = integer % 10;
	
	
	// digit 2
	set7segmentDigit (2, tens_digit, false);
	// digit 3
	set7segmentDigit (3, unit_digit, true);		// to display '.'
	// digit 4
	set7segmentDigit (4, decm_digit1, false);
	// digit 5
	set7segmentDigit (5, decm_digit2, false);
	
		
}



// ver2: Display 2.0 digits for O2 concentration
/*
void display_o2 (float o2value) {

    uint16_t    int_o2value;
    uint8_t     unit_digit;
    uint8_t     tens_digit;
    uint8_t     decm_digit1;
    uint8_t     decm_digit2;
    

	//     int_o2value   = round (o2value);
    int_o2value   = (unsigned int) (o2value * 100.0);
	
    decm_digit2   = int_o2value % 10;
    int_o2value   = int_o2value / 10;
    decm_digit1   = int_o2value % 10;
    int_o2value   = int_o2value / 10;
    unit_digit    = int_o2value % 10;
    int_o2value   = int_o2value / 10;
    tens_digit    = int_o2value % 10;
    
		
	// digit 1
	set7segment_disp_char (1, 'o');
	// digit 2
	set7segmentDigit (2, tens_digit, false);
	// digit 3
	set7segmentDigit (3, unit_digit, true);		// to display '.'
	// digit 4
	set7segmentDigit (4, decm_digit1, false);
	// digit 5
	set7segmentDigit (5, decm_digit2, false);	
	
	
    
}
*/

// ver 3
void display_o2 (float o2value) {

	// digit 1 display
	set7segment_disp_char (1, 'o');
	
	// digit 2 to 5 display
	__7seg_display_float_in_2p2_format (o2value);
	
}

void display_pressure (float pressure_value) {

	// digit 1
	set7segment_disp_char (1, 'P');
	
	// digit 2 to 5 display
	__7seg_display_float_in_2p2_format (pressure_value);
	
}


void display_o2_moving_avg (float avg) {

	// digit 1
	set7segment_disp_char (1, 'A');

	// digit 2 to 5 display
	__7seg_display_float_in_2p2_format (avg);
	
	
}

void display_temprature (float tempr_value) {

	// digit 1
	set7segment_disp_char (1, 't');

	// digit 2 to 5 display
	__7seg_display_float_in_2p2_format (tempr_value);
	
	
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


    set7segmentDigit (1, ten_th_digit, false);
    set7segmentDigit (2, thnd_digit,   false);
    set7segmentDigit (3, hund_digit,   false);
    set7segmentDigit (4, tens_digit,   false);
    set7segmentDigit (5, unit_digit,   false);
         
}



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




// Please feel free to change below display stay timeout settings as required
enum DISPLAY_DELAY_E	{

	TOTAL_RUN_TIME_SECS			= 05,
	CURR_RUN_TIME_SECS			= 05,
	O2C_DISP_TIME_SECS			= 05,
	PRESSURE_DISP_TIME_SECS		= 05,
	MOVING_AVG_DISP_TIME_SECS	= 05,
	TEMPR_DISP_TIME_SECS		= 05,
	
};


void    display_task (void) {

	static char state;
	static int	stay_time;	
	
	int secs ;
	int mins ;
	int hrs	 ;
	
	// execute once every second only
	if (f_sec_disp_task)	{
		f_sec_disp_task = 0;
		if (stay_time)	{
			stay_time--;
		}
	}
	
	// check if set delay is over
	if (stay_time)	{
		return;
		// control will return from here.. will not execute below code (till closing brace of this function)..
	}
	
	init_7segments ();
	
	// control comes here and executes this part of the code, only if 'stay_time == 0'
	switch (state)
	{
		
		default:
			state = 0;
			break;
		case 1:
			#ifdef	DISPLAY_O2C_ON_7_SEGMENTS
			display_o2 (o2_concentration);
			stay_time = O2C_DISP_TIME_SECS;
			#endif
			break;
		case 2:
			#ifdef	DISPLAY_PRESSURE_ON_7_SEGMENTS
			display_pressure (output_pressure);
			stay_time = PRESSURE_DISP_TIME_SECS;
			#endif
			break;
		case 3:
			#ifdef	DISPLAY_MOVING_AVG_ON_7_SEGMENTS
			display_o2_moving_avg (o2_moving_avg);
			stay_time = MOVING_AVG_DISP_TIME_SECS;
			#endif
			break;		
		case 4:
			#ifdef	DISPLAY_TEMPR1_ON_7_SEGMENTS
			display_temprature (tempr_value_1);	// temperature sensor - 1 is used
			stay_time = TEMPR_DISP_TIME_SECS;
			#endif
			break;
		case 5:
			#ifdef	DISPLAY_CRN_ON_7_SEGMENTS
            secs = ( current_run_time_secs %  60);
            mins = ((current_run_time_secs % (60 * 60)) / 60);
            hrs  = ( current_run_time_secs / (60 * 60));
            display_current_run_hours(hrs, mins);
			stay_time = CURR_RUN_TIME_SECS;
			#endif
			break;
		case 6:
			#ifdef	DISPLAY_TRN_ON_7_SEGMENTS
			hrs  = ( total_run_time_secs / (60 * 60));
			display_total_run_hours(hrs);
			stay_time = TOTAL_RUN_TIME_SECS;
			#endif
			break;
	}
	
	state++;


}













////// scratch pad ////////////////////////////
