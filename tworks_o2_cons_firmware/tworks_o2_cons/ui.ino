// ui.c


#include <string.h>

#include "platform.h"
#include "logs.h"
#include "ui.h"
#include "db.h"


static int  ui_state, prev_ui_state;
static int  f_state_changed;


int         start_switch_dbnc_dly;
int         alarm_clear_bttn_dbnc_dly;

uint8_t		alarm_button_press_count;


void button_check (void)  {

    int     start_switch_state = 0;         // variable for reading the pushbutton status
    int     alarm_clear_buttn_state = 0;    // variable for reading the pushbutton status

	static long	int	time_tag;
	

    start_switch_state      = digitalRead(startSwitchPin);
    alarm_clear_buttn_state = digitalRead(alarmClearButton);

    // 1. Start Switch Press detection
    if (start_switch_state == START_SWITCH_PRESSED) {   // press detection
        start_switch_dbnc_dly++;
        if (start_switch_dbnc_dly > BUTTON_DEBOUNCE_DLY)  {
            start_switch_pressed = true; 
            start_switch_dbnc_dly = 0;
        }
    }
    // 2. Start Switch Release detection
    else if (start_switch_state == START_SWITCH_RELEASED) {   // press detection
        start_switch_dbnc_dly++;
        if (start_switch_dbnc_dly > BUTTON_DEBOUNCE_DLY)  {
            start_switch_pressed = false;
            start_switch_dbnc_dly = 0;
        }

    }
    else {
        // nop
    }
  
  
    // 3. Alarm Clear Button Press detection
    if (alarm_clear_buttn_state == ALARM_CLEAR_BUTTON_PRESSED) {   // press detection
        alarm_clear_bttn_dbnc_dly++;
        if (alarm_clear_bttn_dbnc_dly > BUTTON_DEBOUNCE_DLY)  {
            alarm_clear_button_pressed = true;
            alarm_clear_bttn_dbnc_dly = 0;
			alarm_button_press_count++;
			multi_beeps (alarm_clear_button_pressed);
			DBG_PRINT   (__FILE__);
			DBG_PRINT (" : Alarm Clear Button Pressed.. : ");
			DBG_PRINT (alarm_button_press_count);
			DBG_PRINTLN (" times..");
			time_tag = systemtick_msecs;
		}
    }
    else {
         // reset press count after 1.5 sec of no activity
		 if (time_elapsed(time_tag) > 1500)	{
				alarm_button_press_count = 0;
		 }
    }
     
}



void ui_print_welcome (void)    {

    lcd.setCursor(0, 0);
    lcd.print(" Welcome to Tworks");
    lcd.setCursor(0, 1);
    lcd.print("     Hyderabad");
    lcd.setCursor(0, 2);
    lcd.print("Oxygen Concentrator!");

}

void ui_init (void) {

    // Print a message to the LCD.
    ui_print_welcome ();

}



void beep_for (int msecs) {

    BUUZZER_CNTRL (BUZZ_ON);
    new_delay_msecs (msecs);
    BUUZZER_CNTRL (BUZZ_OFF);

}


void multi_beeps (int count) {

    int   i;
    if (count > 10) {
        count = 10;
    }

    for (i = 0; i < count; i++)
    {
        beep_for (50);
        new_delay_msecs (100);
    }

}



void ui_task_main (void)    {

    static int            button_debounce_delay;
    static unsigned long  time_tag;
    static unsigned int   state_time;
    char                  str_temp[6];
    char                  str_temp2[6];


    if (f_sec_change_ui_task) {
        f_sec_change_ui_task = 0;
        state_time++;
    }

    button_check ();
	
	// check for alarm button presses for special functions
	#define EEPROM_ERASE_BUTTON_PRESSES		(5)
	/*
		Note: Erase EEPROM if alarm reset button is pressed for 5 times continuously, 
				when system is in off condition
	*/
	if (!f_system_running && alarm_button_press_count >= EEPROM_ERASE_BUTTON_PRESSES)	{
		alarm_button_press_count = 0;
		BUUZZER_CNTRL (BUZZ_ON);
		delay(100);
		BUUZZER_CNTRL (BUZZ_OFF);
		DBG_PRINTLN ("EEPROM Erase command issued..");
		DBG_PRINTLN ("erasing..");
		eepfill	  (EEPROM_LOGS_START_ADDRESS, 0xFF, EEPROM_LOGS_AREA_SIZE_IN_BYTES);
		DBG_PRINTLN ("erase complete.");
		record_no = 0;
		update_log_rcord_head_ptr (EEPROM_LOGS_START_ADDRESS);
		BUUZZER_CNTRL (BUZZ_ON);
		delay(333);
		BUUZZER_CNTRL (BUZZ_OFF);

	}
	

    switch (ui_state)
    {
        case UI_START:
            // todo
            ui_state = UI_SYS_INIT;
            break;
        case UI_SYS_INIT:
            lcd.clear();
            ui_print_welcome ();
            lcd.setCursor(0, 3);
            lcd.print("Press ButtonToStart!");
            ui_state = UI_SYS_ON_CHECK;
            break;
        case UI_SYS_ON_CHECK:
            // System ON check
            if ( start_switch_pressed == true && !f_critical_alarms )  {
                f_system_running = true;

                DBG_PRINTLN();
                DBG_PRINTLN("Start Button Pressed..!");
                lcd.setCursor(0, 3);
                lcd.print("Start Button Pressed");
                beep_for (SYS_ON_BEEP);   // msecs
                
                lcd.setCursor(0, 3);
                lcd.print("O2 Cons. Starting... ");
                new_delay_msecs (1000);                
                ui_state = UI_SYS_RUNNING;
            }
            break;

        case UI_SYS_RUNNING:
            // system running
            // LCD Line 1
            if (f_state_changed)  {
                f_state_changed = 0;
                lcd_clear_buf (lcd_temp_string);

                lcd.clear();
                lcd.setCursor(0, 0);
                // sprintf(lcd_temp_string, "O2 CONC   PRESSURE ");
                DBG_PRINTLN(lcd_temp_string);
                //        "...................."
                lcd.print(lcd_temp_string);

                // temp : just to trigger LCD refresh for O2 values
                prev_o2_concentration = 0;
            }

            // System OFF check
            if (start_switch_pressed == false || f_critical_alarms)  {
                ui_state = UI_SYS_OFF_CHECK;
            }
            else {
                // no state change
            }
            break;

        case UI_SYS_OFF_CHECK:
            // System OFF check
			f_run_hours = 0;
            f_system_running = false;				
			blank_7segments ();

            last_cycle_run_time_secs = current_run_time_secs;
            current_run_time_secs = 0;
            save_run_hrs_n_calib_constants ();
            
			if (!f_critical_alarms)	{
				DBG_PRINTLN("Stop Button Pressed!");
				lcd.setCursor(0, 3);
				lcd.print("Stop Button Pressed ");
			}
			else {
				DBG_PRINTLN("Critical Alarm(s) triggered..!!!");
				lcd.setCursor(0, 3);
				lcd.print("Alarm Triggered..!!!");
			}
            COMPRSSR_CNTRL (COMPRSSR_OFF);
            beep_for (SYS_OFF_BEEP);   // msecs

            // Bring values to default position
            tworks2_values_to_default_postion ();

            new_delay_msecs (1000);
            lcd.setCursor(0, 3);
            lcd.print("O2 Cons. Stopping.. ");
            new_delay_msecs (1000);

            lcd.setCursor(0, 3);
            lcd.print("O2 Cons. Stopped..! ");
            ui_state = UI_LAST;
            break;
        default:
        case UI_LAST:
            ui_state = UI_START;
            break;
    }

    if (ui_state != prev_ui_state ) {
        prev_ui_state = ui_state;
        f_state_changed = 1;
        state_time = 0;
        // DBG_PRINT("ui_state : ");
        // DBG_PRINTLN(ui_state);
    }
}




void power_on_self_test (void) {

    lcd.setCursor(0, 1);
    lcd.print("Relay Tests..");

    // 1. Relay Z1TSOL
    lcd.setCursor(0, 2);
    lcd.print("Z1TSOL - CLOSE  ");
    digitalWrite(Sieve_A_Valve_Z1,      OPEN_VALVE);
    beep_for (HIGH_BEEP);   // msecs
    new_delay_msecs (5000);

    lcd.setCursor(0, 2);
    lcd.print("Z1TSOL - OPEN   ");
    digitalWrite(Sieve_A_Valve_Z1,      CLOSE_VALVE);
    beep_for (LOW_BEEP);   // msecs

    new_delay_msecs (5000);

    // 2. Relay Z2TSOL
    lcd.setCursor(0, 2);
    lcd.print("Z2TSOL - CLOSE  ");
    digitalWrite(Sieve_B_Valve_Z2,      OPEN_VALVE);
    beep_for (HIGH_BEEP);   // msecs
    new_delay_msecs (5000);
    lcd.setCursor(0, 2);
    lcd.print("Z2TSOL - OPEN   ");
    digitalWrite(Sieve_B_Valve_Z2,      CLOSE_VALVE);
    beep_for (LOW_BEEP);   // msecs
    new_delay_msecs (2000);

    // 3. Relay BCKFSOL
    lcd.setCursor(0, 2);
    lcd.print("BCKFSOL - CLOSE ");
    digitalWrite(PreCharge_Valve_BCKF,  OPEN_VALVE);
    beep_for (HIGH_BEEP);   // msecs
    new_delay_msecs (5000);

    lcd.setCursor(0, 2);
    lcd.print("BCKFSOL - OPEN  ");
    digitalWrite(PreCharge_Valve_BCKF,  CLOSE_VALVE);
    beep_for (LOW_BEEP);   // msecs
    new_delay_msecs (2000);

    //lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print("Compressor Test..");

    // 4. Compressor check
    // Open release valves.. to avoid blocking of compressor o/p during its test-run
    lcd.setCursor(0, 2);
    //        "...................."
    lcd.print("Opening releaseValvs");
    beep_for (HIGH_BEEP);   // msecs
    digitalWrite(Sieve_A_Valve_Z1,      CLOSE_VALVE);
    digitalWrite(Sieve_B_Valve_Z2,      CLOSE_VALVE);
    new_delay_msecs (1000);

    lcd.setCursor(0, 2);
    lcd.print("Compressor - ON     ");
    COMPRSSR_CNTRL (COMPRSSR_ON);
    beep_for (HIGH_BEEP);   // msecs
    new_delay_msecs (5000);
    

    BUUZZER_CNTRL (BUZZ_OFF);

    beep_for (LOW_BEEP);   // msecs
    new_delay_msecs (1000);
    lcd.setCursor(0, 2);
    lcd.print("Compressor - OFF    ");
    COMPRSSR_CNTRL (COMPRSSR_OFF);

    lcd.clear();
    lcd.setCursor(0, 3);
    //  "1.3.5.7.9.........20"
    lcd.print("Testing over...!!   ");
    new_delay_msecs (2000);

}







// ------------------------ EOF ------------------------------------

