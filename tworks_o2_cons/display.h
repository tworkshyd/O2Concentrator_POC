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

/*
 *  Two LED boards Board 1 with 5 LEDs A0 to A4
 *   and  Board 2 with 2 LEDs A0 & A1 (will refere these as a0 & a1)
 *   
 *   From host controller 3 pins are used for interface 
 *    MISO, PD6 & PD7, these are connected from display board JP5 connector to 
 *    J1 connector of LED Board1, LED Board2 is cascaed with LED Board1
 *   
 *   LED orientation for data 000 to 111 on these control bits is ..
 *   
 *   MISO  PD6  PD7          a1  a0  A4  A3  A2  A1  A0   
 *    0    0     0            -   -   -   -   -   -   - 
 *    0    0     1            -   -   -   -   -   -   1 
 *    0    1     0            -   -   -   -   -   1   - 
 *    0    1     1            -   -   -   -   1   -   - 
 *    1    0     0            -   -   -   1   -   -   - 
 *    1    0     1            -   -   1   -   -   -   - 
 *    1    1     0            -   1   -   -   -   -   - 
 *    1    1     1            1   -   -   -   -   -   - 
 *    
 */
#define   A0        (0b001)
#define   A1        (0b010) 
#define   A2        (0b011)
#define   A3        (0b100)
#define   A4        (0b101) 
#define   a0        (0b110)
#define   a1        (0b111)
#define   ALL_OFF   (0b000)

enum ALIGN {LEFT, CENTER, RIGHT};

enum ERROR_CODE_E {

    TRN_DISPLAY,
    CRN_DISPLAY,
    LOW_O2_PURITY,
    OUTPUT_FLOW_OBSTRUCT,
    POWER_FAIL,
    UNIT_OVER_HEAT,
    UNUSED_LED,

    ALL_LEDs_OFF
};

//enum ALIGN {LEFT, CENTER, RIGHT};
extern char     lcd_temp_string[LCD_COLS + 1];




void lcd_clear_buf  (char * bufp);
void init_7segments (void);
void display_banner (void);
void scrollDigits   (void);
void test_7segments (void);
void display_o2     (float o2value);
void display_run_time  (uint16_t hours, uint16_t mins);
void display_run_hours (uint32_t runhours);

void neo_pixcel_data (enum ERROR_CODE_E error_no, uint8_t  on_off);


// initialize the library by associating any needed LCD interface pin
// as per aurdino nano - base board,  SCH Dated: 09-02-2021

//  line_no (1 to LCD_ROWS), start_col ( 0 to LCD_COLS-1)
void lcd_wirte_to_line (char line_no, char start_col, char * str_ptr, char align);






#endif
