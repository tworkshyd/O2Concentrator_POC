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


#define LCD_ROWS   ( 4)
#define LCD_COLS   (20)

LiquidCrystal   lcd(RS, EN, D4, D5, D6, D7);
LedControl      lc = LedControl(dataPin_7segment, clckPin_7segment, loadPin_7segment, 1);


enum ALIGN {LEFT, CENTER, RIGHT};
extern char     lcd_temp_string[LCD_COLS + 1];




void lcd_clear_buf		(char * bufp);

void neo_led_data_send		(uint8_t  select_bits);
void update_neo_pixel_leds	(void);    
void neo_pixel_control		(uint8_t led_no, uint8_t on_off);    
void neo_pixel_leds_test	(void);


void init_7segments		(void);
void blank_7segments	(void);
void test_7segments		(void);
void display_banner		(void);
void scrollDigits		(void);
void test_7segments		(void);
// void display_o2			(float o2value);
void display_current_run_hours   (uint16_t hours, uint16_t mins);
void display_total_run_hours     (uint32_t runhours);
 

// initialize the library by associating any needed LCD interface pin
// as per aurdino nano - base board,  SCH Dated: 09-02-2021

//  line_no (1 to LCD_ROWS), start_col ( 0 to LCD_COLS-1)
void lcd_wirte_to_line (char line_no, char start_col, char * str_ptr, char align);

void display_task (void);




#endif
