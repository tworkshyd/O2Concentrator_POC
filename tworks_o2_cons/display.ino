

#include "display.h"

/*
 * 
 * Prameters :  line_no (1 to LCD_ROWS), start_col ( 0 to LCD_COLS-1)
 *              align (LEFT, CENTER, RIGHT)
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
