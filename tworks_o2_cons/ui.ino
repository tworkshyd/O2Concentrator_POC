// ui.c


#include <string.h>

#include "platform.h"
#include "logs.h"
#include "ui.h"
#include "db.h"


static int  ui_state, prev_ui_state;
static int  f_state_changed;


int             start_switch_dbnc_dly;
int             alarm_clear_bttn_dbnc_dly;

void button_check (void)  {

    int     start_switch_state = 0;         // variable for reading the pushbutton status
    int     alarm_clear_buttn_state = 0;         // variable for reading the pushbutton status


    start_switch_state = digitalRead(startSwitchPin);
    alarm_clear_buttn_state = digitalRead(alarmClearButton);

    // 1. Start Switch Press detection
    if (start_switch_state == START_SWITCH_PRESSED) {   // press detection
        start_switch_dbnc_dly++;
        if (start_switch_dbnc_dly > BUTTON_DEBOUNCE_DLY)  {
            start_switch_pressed = true; 
            start_switch_dbnc_dly = 0;
            //DBG_PRINT  ("Start_switch Pressed");
        }
        //DBG_PRINT  ("start_switch_dbnc_dly : ");
        //DBG_PRINTLN(start_switch_dbnc_dly);
    }
    // 2. Start Switch Release detection
    else if (start_switch_state == START_SWITCH_RELEASED) {   // press detection
      start_switch_dbnc_dly++;
      if (start_switch_dbnc_dly > BUTTON_DEBOUNCE_DLY)  {
          start_switch_pressed = false;
          start_switch_dbnc_dly = 0;
          //DBG_PRINT  ("Start_switch RELEASED");
      }
      //DBG_PRINT  ("start_switch_dbnc_dly : ");
      //DBG_PRINTLN(start_switch_dbnc_dly);
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
        //DBG_PRINT  ("Alarm Clear Button Pressed..");
      }
      //DBG_PRINT  ("alarm_clear_bttn_dbnc_dly : ");
      //DBG_PRINTLN(alarm_clear_bttn_dbnc_dly);
    }
    // 4. Alarm Clear Button Release detection
    else if (alarm_clear_buttn_state == ALARM_CLEAR_BUTTON_RELEASED) {   // press detection
      alarm_clear_bttn_dbnc_dly++;
      if (alarm_clear_bttn_dbnc_dly > BUTTON_DEBOUNCE_DLY)  {
        alarm_clear_button_pressed = false;
        alarm_clear_bttn_dbnc_dly = 0;
        //DBG_PRINT  ("Alarm Clear Button RELEASED");
      }
      //DBG_PRINT  ("alarm_clear_bttn_dbnc_dly : ");
      //DBG_PRINTLN(alarm_clear_bttn_dbnc_dly);
    }
    else {
      // nop
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




int             start_switch_dbnc_dly;
int             alarm_clear_bttn_dbnc_dly;

void button_check (void)  {

    int     start_switch_state = 0;         // variable for reading the pushbutton status
    int     alarm_clear_buttn_state = 0;         // variable for reading the pushbutton status


    start_switch_state = digitalRead(startSwitchPin);
    alarm_clear_buttn_state = digitalRead(alarmClearButton);

    // 1. Start Switch Press detection
    if (start_switch_state == START_SWITCH_PRESSED) {   // press detection
        start_switch_dbnc_dly++;
        if (start_switch_dbnc_dly > BUTTON_DEBOUNCE_DLY)  {
            start_switch_pressed = true; 
            start_switch_dbnc_dly = 0;
            DBG_PRINT  ("Start_switch Pressed");
        }
        DBG_PRINT  ("start_switch_dbnc_dly : ");
        DBG_PRINTLN(start_switch_dbnc_dly);
    }
    // 2. Start Switch Release detection
    else if (start_switch_state == START_SWITCH_RELEASED) {   // press detection
      start_switch_dbnc_dly++;
      if (start_switch_dbnc_dly > BUTTON_DEBOUNCE_DLY)  {
          start_switch_pressed = false;
          start_switch_dbnc_dly = 0;
          DBG_PRINT  ("Start_switch RELEASED");
      }
      DBG_PRINT  ("start_switch_dbnc_dly : ");
      DBG_PRINTLN(start_switch_dbnc_dly);
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
        DBG_PRINT  ("Alarm Clear Button Pressed..");
      }
      DBG_PRINT  ("alarm_clear_bttn_dbnc_dly : ");
      DBG_PRINTLN(alarm_clear_bttn_dbnc_dly);
    }
    // 4. Alarm Clear Button Release detection
    else if (alarm_clear_buttn_state == ALARM_CLEAR_BUTTON_RELEASED) {   // press detection
      alarm_clear_bttn_dbnc_dly++;
      if (alarm_clear_bttn_dbnc_dly > BUTTON_DEBOUNCE_DLY)  {
        alarm_clear_button_pressed = false;
        alarm_clear_bttn_dbnc_dly = 0;
        DBG_PRINT  ("Alarm Clear Button RELEASED");
      }
      DBG_PRINT  ("alarm_clear_bttn_dbnc_dly : ");
      DBG_PRINTLN(alarm_clear_bttn_dbnc_dly);
    }
    else {
      // nop
    }
      
  
}


void ui_task_main (void)    {

    static int            button_debounce_delay;
    static unsigned long  time_tag;
    static unsigned int   state_time;
    char                  str_temp[6];
    char                  str_temp2[6];

<<<<<<< HEAD
//    buttonState = digitalRead(startButtonPin);
=======
    button_check ();
>>>>>>> origin/RTC_DS1307_integration

    if (f_sec_change_ui_task) {
        f_sec_change_ui_task = 0;
        state_time++;
    }


    button_check ();
      
//    if (buttonState == BUTTON_ACTIVE) {   // press detection
//        button_debounce_delay++;
//        if (button_debounce_delay >= BUTTON_DEBOUNCE_DLY)   {
//            button_debounce_delay = 0;
//            button_pressed = true;
//        }
//        // temp
//        DBG_PRINT  ("button_debounce_delay : ");
//        DBG_PRINTLN(button_debounce_delay);
//    }
//    else {  // release detection
//        if (button_debounce_delay)
//            button_debounce_delay--;
//        if (button_debounce_delay == 0)    {
//            if (button_pressed) {
//                button_pressed = false;
//                button_press_count++;
//                time_tag = systemtick_msecs;
//                // temp
//                DBG_PRINT  ("button_press_count : ");
//                DBG_PRINTLN(button_press_count);
//            }
//        }
//
//    }


    switch (ui_state)
    {
        case UI_START:
<<<<<<< HEAD:tworks_o2_cons/ui.ino
            //if (powerUpTimer.check())   {
//            if (state_time >= 10) {
//                ui_state = UI_SYS_INIT;
//                lcd.setCursor(0, 3);
//                button_press_count = 0;
//                lcd.print("Press ButtonToStart!");
//            }
//            else if (time_elapsed (time_tag) > 1500) {
//                if (button_press_count >= CALIBRATION_MODE_ENTRY_CHECK)  {
//                    button_press_count = 0;
//                    ui_state = UI_CALIB_MODE;
//                }
//                else if (button_press_count >= FACTORY_MODE_ENTRY_CHECK)  {
//                    button_press_count = 0;
//                    ui_state = UI_FACTORY_MODE;
//                }
//                if (button_press_count >= CONFIG_MODE_ENTRY_CHECK)  {
//                    button_press_count = 0;
//                    ui_state = UI_CONFIG_MODE;
//                }
//            }
//            else {
//                if (f_state_changed)  {
//                    ui_print_welcome ();
//                }
//            }
            ui_state = UI_SYS_INIT;
=======
			ui_state = UI_SYS_INIT;
>>>>>>> From_scratch:O2Concentrator_POC/tworks_o2_cons/ui.ino
            break;
//        case UI_CALIB_MODE:
//            DBG_PRINTLN("Entered Calibration Mode..");
//            lcd.clear();
//            lcd.setCursor(0, 0);
//            lcd.print("Calibration Mode..");
//            lcd.setCursor(0, 1);
//            lcd.print("O2 sensorCalibration");
//            multi_beeps (5);
//            
//            ui_state = UI_SYS_INIT;
//            break;
//        case UI_FACTORY_MODE:
//            DBG_PRINTLN("Factory Mode..");
//            lcd.clear();
//            lcd.setCursor(0, 0);
//            lcd.print("Factory Mode..");
//
//            multi_beeps (3);
//
//            // Power On self test - on demand
//            power_on_self_test ();
//            ui_state = UI_SYS_INIT;
//            break;
//
//        case UI_CONFIG_MODE:
//            DBG_PRINTLN("Entered Configuration Mode..");
//            lcd.clear();
//            lcd.setCursor(0, 0);
//            lcd.print("Configuration Mode..");
//            lcd.setCursor(0, 1);
//
//            multi_beeps (2);
//            ui_state = UI_SYS_INIT;
//            break;

        case UI_SYS_INIT:
            lcd.clear();
            ui_print_welcome ();
            lcd.setCursor(0, 3);
            lcd.print("Press ButtonToStart!");
            ui_state = UI_SYS_ON_CHECK;
            break;
        case UI_SYS_ON_CHECK:
            // System ON check
            if (start_switch_pressed == true)  {
                f_system_running = true;

                DBG_PRINTLN("Start Button Pressed..!");
                lcd.setCursor(0, 3);
                lcd.print("Start Button Pressed");
                beep_for (SYS_ON_BEEP);   // msecs

<<<<<<< HEAD:tworks_o2_cons/ui.ino
                
=======
                COMPRSSR_CNTRL (COMPRSSR_ON);
>>>>>>> From_scratch:O2Concentrator_POC/tworks_o2_cons/ui.ino
                // new_delay_msecs (1000);

                lcd.setCursor(0, 3);
                lcd.print("O2 Cons. Starting... ");
<<<<<<< HEAD:tworks_o2_cons/ui.ino
                new_delay_msecs (1000);
                // COMPRSSR_CNTRL (COMPRSSR_ON);
                
=======
                // new_delay_msecs (1000);

>>>>>>> From_scratch:O2Concentrator_POC/tworks_o2_cons/ui.ino
                ui_state = UI_SYS_RUNNING;
            }
            break;


        case UI_SYS_RUNNING:
            // system running
<<<<<<< HEAD:tworks_o2_cons/ui.ino
            // LCD Line 1
            if (f_state_changed)  {
                f_state_changed = 0;
                lcd_clear_buf (lcd_temp_string);


                lcd.clear();
                lcd.setCursor(0, 0);
                sprintf(lcd_temp_string, "O2 CONC   PRESSURE ");
                DBG_PRINTLN(lcd_temp_string);
                //        "...................."
                lcd.print(lcd_temp_string);

                // temp : just to trigger LCD refresh for O2 values
                prev_o2_concentration = 0;
            }
/*
            // LCD Line 2
            // O2 concentration / output pressure
            if (prev_o2_concentration != o2_concentration || prev_output_pressure != output_pressure)  {
                prev_o2_concentration  = o2_concentration;
                prev_output_pressure = output_pressure;
                // sprintf(lcd_temp_string, "%f \%%   %2d psi", o2_concentration, output_pressure);
                // DBG_PRINTLN(lcd_temp_string);
                // 4 is mininum width, 2 is precision; float value is copied onto str_temp
                dtostrf(o2_concentration, 4, 2, str_temp);


                dtostrf(output_pressure, 5, 2, str_temp2);
                // sprintf(lcd_temp_string, "%f \%%   %2d psi", o2_concentration, output_pressure);
                sprintf(lcd_temp_string, "%s %%   %s psi", str_temp, str_temp2);

                DBG_PRINTLN(lcd_temp_string);
                lcd.setCursor(0, 1);
                lcd.print(lcd_temp_string);
#ifdef  CHANGE_IN_O2_LEVEL
    

#endif
            }

            // LCD Line 3
            // lcd.setCursor(0, 2);
            // blank for now

            // LCD Line 4
            if (prev_current_run_time_secs ^ current_run_time_secs) {
                prev_current_run_time_secs = current_run_time_secs;

                int secs = ( current_run_time_secs %  60);
                int mins = ((current_run_time_secs % (60 * 60)) / 60);
                int hrs  = ( current_run_time_secs / (60 * 60));
                sprintf(lcd_temp_string, "RUN TIME  %02d:%02d:%02d", hrs, mins, secs);
                DBG_PRINTLN(lcd_temp_string);
                lcd.setCursor(0, 3);
                lcd.print(lcd_temp_string);
            }
*/
            // System OFF check
            if (start_switch_pressed == false)  {
                ui_state = UI_SYS_OFF_CHECK;
=======
            // System OFF check
            if (start_switch_pressed == false)  {
                f_system_running = false;
                ui_state = UI_SYS_SHUT_OFF;
>>>>>>> From_scratch:O2Concentrator_POC/tworks_o2_cons/ui.ino
            }
            else {
                // no state change
            }
            break;

<<<<<<< HEAD:tworks_o2_cons/ui.ino
        case UI_SYS_OFF_CHECK:
            // System OFF check
//            if (button_pressed == true)  {
//                button_pressed = false;
                f_system_running = false;

                last_cycle_run_time_secs = current_run_time_secs;
                current_run_time_secs = 0;
                save_record ();
                
                DBG_PRINTLN("Stop Button Pressed!");
                lcd.setCursor(0, 3);
                lcd.print("Stop Button Pressed ");
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
//            }
//            else {
//                // back to run state..
//                ui_state = UI_SYS_RUNNING;
//            }
=======
        case UI_SYS_SHUT_OFF:
            // System OFF 
            COMPRSSR_CNTRL (COMPRSSR_OFF);
            beep_for (SYS_OFF_BEEP);   // msecs
            new_delay_msecs (1000);
            lcd.setCursor(0, 3);
            lcd.print("O2 Cons. Stopping.. ");
            new_delay_msecs (1000);
            ui_state = UI_LAST;           
>>>>>>> From_scratch:O2Concentrator_POC/tworks_o2_cons/ui.ino
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
        DBG_PRINT("ui_state : ");
        DBG_PRINTLN(ui_state);
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
