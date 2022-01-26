// display.h

#ifndef _DISPLAY_h
#define _DISPLAY_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif


#include "platform.h"

// include the library code:
#include <LiquidCrystal.h>
#include "LedControl.h"


//==================== 7-segments LED driver ====================
#define NO_OF_MAX7219_CASCADED_ICs	(1)

// #define NO_OP           (0x00)
// #define DIGIT_0         (0x01)
// #define DIGIT_1         (0x02)
// #define DIGIT_2         (0x03)
// #define DIGIT_3         (0x04)
// #define DIGIT_4         (0x05)
// #define DIGIT_5         (0x06)
// #define DIGIT_6         (0x07)
// #define DIGIT_7         (0x08)
// #define DECODE_MODE     (0x09)
// #define INTENSITY       (0x0A)
// #define SCAN_LIMIT      (0x0B)
// #define SHUT_DOWN       (0x0C)
// #define DISPLAY_TEST    (0x0F)
// 
#define BLANK           (0x0F)

#define OP_NOOP   0
#define OP_DIGIT0 1
#define OP_DIGIT1 2
#define OP_DIGIT2 3
#define OP_DIGIT3 4
#define OP_DIGIT4 5
#define OP_DIGIT5 6
#define OP_DIGIT6 7
#define OP_DIGIT7 8
#define OP_DECODEMODE  9
#define OP_INTENSITY   10
#define OP_SCANLIMIT   11
#define OP_SHUTDOWN    12
#define OP_DISPLAYTEST 15


extern uint8_t   digit_to_seg_value[];



//==================== Neo-pixel LED driver ====================
#if   (HW_REVISION == HW_REV_1_0)
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
	#define     NEO_PXL_ALL_OFF             (0b000)
	#define     NEO_PXL_LED_1             (0b001)
	#define     NEO_PXL_LED_2             (0b010)
	#define     NEO_PXL_LED_3             (0b011)
	#define     NEO_PXL_LED_4             (0b100)
	#define     NEO_PXL_LED_5             (0b101)
	#define     NEO_PXL_CURR_RUN_TIME       (0b110)
	#define     NEO_PXL_TOTAL_RUN_TIME      (0b111)

	#define     ON_LED                      (true)
	#define     OFF_LED                     (false)


#elif (HW_REVISION == HW_REV_2_0)
	/*
	LEDs position on display board of Rev 2.0
	----------------------------------------------------------------------------
	|																			|
	|			NPLED#1		NPLED#2		NPLED#3		"LED#1"		NPLED#4			|
	|																			|
	|			alarm#1		alarm#2		alarm#3		   -	    unused			|
	|																			|
	.																			.																		
	.																			.
	.																			.
	|																			|
	----------------------------------------------------------------------------
	Note: LED#1 is not a neo-pixcel and is a supply failure alarm LED triggers 
			directly from power failure circuit.
	
	 * JP5 of display board of rev2.0 SCH,
	 * --------------------------------------------------
	 *      MISO    PD6     PD7     |       output
	 * --------------------------------------------------
	 *      0       0       0       |   all off
	 *      0       1       0       |   Alarm-1 (NPLED#4)
	 *      1       0       0       |   Alarm-2 (NPLED#3)
	 *      0       0       1       |   Alarm-3 (NPLED#2)
	 *      0       1       1       |   Alarm-4 (NPLED#1)
	 * --------------------------------------------------
	*/

	#define     NEO_PXL_ALL_OFF				(0b000)
	#define     NEO_PXL_LED_1				(0b001)
	#define     NEO_PXL_LED_2				(0b010)
	#define     NEO_PXL_LED_3				(0b011)
	#define     NEO_PXL_LED_4				(0b100)
	#define     NEO_PXL_LED_5				(0b101)
	#define     NEO_PXL_LED_6				(0b110)
	#define     NEO_PXL_LED_7				(0b111)


	#define     ON_LED                      (true)
	#define     OFF_LED                     (false)

#else
	// nop
#endif



/*
    The circuit:
    LCD RS pin to digital pin 12
    LCD Enable pin to digital pin 11
    LCD D4 pin to digital pin 5
    LCD D5 pin to digital pin 4
    LCD D6 pin to digital pin 3
    LCD D7 pin to digital pin 2
    LCD R/W pin to ground
    LCD VSS pin to ground
    LCD VCC pin to 5V
    10K resistor:
    ends to +5V and ground
    wiper to LCD VO pin (pin 3)
*/


#define LCD_ROWS    ( 4)
#define LCD_COLS    (20)



enum ALIGN {LEFT, CENTER, RIGHT};
extern char			lcd_temp_string[LCD_COLS + 1];


void lcd_clear_buf			     (char * bufp);
void neo_led_data_send		     (uint8_t  select_bits);
void update_neo_pixel_leds	     (void);    
void neo_pixel_control		     (uint8_t led_no, uint8_t on_off);    
void neo_pixel_leds_test	     (void);
void  set7segmentDigit			 (int seg_no, int value, uint8_t  point);
void  set7segment_disp_char		 (int seg_no, char character);

void init_7segments				 (void);
void blank_7segments			 (void);
void test_7segments				 (void);
void display_banner				 (void);
void scrollDigits				 (void);
void test_7segments				 (void);
void display_o2					 (float o2value);
void display_pressure			 (float pressure_value);
void display_temprature			 (float tempr_value);
void display_current_run_hours   (uint16_t hours, uint16_t mins);
void display_total_run_hours     (uint32_t runhours);
 

// initialize the library by associating any needed LCD interface pin
// as per aurdino nano - base board,  SCH Dated: 09-02-2021

//  line_no (1 to LCD_ROWS), start_col ( 0 to LCD_COLS-1)
void lcd_wirte_to_line (char line_no, char start_col, char * str_ptr, char align);

void display_task (void);




#endif
