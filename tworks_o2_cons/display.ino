

#include "display.h"
#include "LedControl.h"


char     lcd_temp_string[LCD_COLS + 1];
/* we always wait a bit between updates of the display */
unsigned long delaytime = 250;



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

/*
  This method will scroll all the hexa-decimal
 numbers and letters on the display. You will need at least
 four 7-Segment digits. otherwise it won't really look that good.
 */
void scrollDigits (void) {
    
    for(int i=0;i<13;i++) 
    {
        lc.setDigit(0,7, 8,false);
        lc.setDigit(0,6, 8,false);
        lc.setDigit(0,5, 8,false);
        lc.setDigit(0,4, 8,false);
        lc.setDigit(0,3, 8,false);
        lc.setDigit(0,2, 8,false);
        lc.setDigit(0,1, 8,false);
        lc.setDigit(0,0, 8,false);
        delay(delaytime);
    }
    
    lc.clearDisplay(0);
    delay(delaytime);
  
}

void test_7segments (void) { 
    
  display_banner();
  scrollDigits();
  
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



    
