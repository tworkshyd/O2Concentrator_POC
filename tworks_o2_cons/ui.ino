// ui.c


#include <string.h>

#include "platform.h"
#include "ui.h"
#include "db.h"


static int  ui_state, prev_ui_state;
static int  f_state_changed;


int buttonState = 0;         // variable for reading the pushbutton status
int button_pressed;
int button_press_count;
int button_press_duration;


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

    BUUZZER_CNTRL (ON);
    delay(msecs);
    BUUZZER_CNTRL (OFF);

}


void multi_beeps (int count) {

    int   i;
    if (count > 10) {
        count = 10;
    }

    for (i = 0; i < count; i++)
    {
        beep_for (50);
        delay(100);
    }

}



void ui_task_main (void)    {

    static int            button_debounce_delay;
    static unsigned long  time_tag;
    static unsigned int   state_time;
    char                  str_temp[6];
    char                  str_temp2[6];

    buttonState = digitalRead(buttonPin);

    if (f_sec_change_ui_task) {
        f_sec_change_ui_task = 0;
        state_time++;
    }

    if (buttonState == BUTTON_ACTIVE) {   // press detection
        button_debounce_delay++;
        if (button_debounce_delay >= BUTTON_DEBOUNCE_DLY)   {
            button_debounce_delay = 0;
            button_pressed = true;
        }
        // temp
        Serial.print  ("button_debounce_delay : ");
        Serial.println(button_debounce_delay);
    }
    else {  // release detection
        if (button_debounce_delay)
            button_debounce_delay--;
        if (button_debounce_delay == 0)    {
            if (button_pressed) {
                button_pressed = false;
                button_press_count++;
                time_tag = systemtick_msecs;
                // temp
                Serial.print  ("button_press_count : ");
                Serial.println(button_press_count);
            }
        }

    }

    //    if (button_debounce_delay >= BUTTON_DEBOUNCE_DLY)   {
    //        button_debounce_delay = 0;
    //    }


    switch (ui_state)
    {
        case UI_START:
            // if (powerUpTimer.check())   {
            if (state_time >= 5) {
                ui_state = UI_SYS_INIT;
                lcd.setCursor(0, 3);
                button_press_count = 0;
                lcd.print("Press ButtonToStart!");
            }
            else if (time_elapsed (time_tag) > 1500) {
                if (button_press_count >= CALIBRATION_MODE_ENTRY_CHECK)  {
                    button_press_count = 0;
                    ui_state = UI_CALIB_MODE;
                }
                else if (button_press_count >= FACTORY_MODE_ENTRY_CHECK)  {
                    button_press_count = 0;
                    ui_state = UI_FACTORY_MODE;
                }
                if (button_press_count >= CONFIG_MODE_ENTRY_CHECK)  {
                    button_press_count = 0;
                    ui_state = UI_CONFIG_MODE;
                }
            }
            else {
                if (f_state_changed)  {
                    ui_print_welcome ();
                }
            }
            break;
        case UI_CALIB_MODE:
            Serial.println("Entered Calibration Mode..");
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Calibration Mode..");
            lcd.setCursor(0, 1);
            lcd.print("O2 sensorCalibration");
            multi_beeps (5);

            ui_state = UI_SYS_INIT;
            break;
        case UI_FACTORY_MODE:
            Serial.println("Factory Mode..");
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Factory Mode..");

            multi_beeps (3);

            // Power On self test - on demand
            power_on_self_test ();
            ui_state = UI_SYS_INIT;
            break;

        case UI_CONFIG_MODE:
            Serial.println("Entered Configuration Mode..");
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Configuration Mode..");
            lcd.setCursor(0, 1);

            multi_beeps (2);
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
            if (button_pressed == true)  {
                button_pressed = false;
                f_system_running = true;

                Serial.println("Start Button Pressed..!");
                lcd.setCursor(0, 3);
                lcd.print("Start Button Pressed");
                beep_for (SYS_ON_BEEP);   // msecs

                COMPRSSR_CNTRL (ON);
                delay (1000);

                lcd.setCursor(0, 3);
                lcd.print("O2 Cons. Starting... ");
                delay (1000);

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
                sprintf(lcd_temp_string, "O2 CONC   PRESSURE ");
                Serial.println(lcd_temp_string);
                //        "...................."
                lcd.print(lcd_temp_string);
            }

            // LCD Line 2
            // O2 concentration / output pressure
            if (prev_o2_concentration != o2_concentration || prev_output_pressure != output_pressure)  {
                prev_o2_concentration  = o2_concentration;
                prev_output_pressure = output_pressure;
                // sprintf(lcd_temp_string, "%f \%%   %2d psi", o2_concentration, output_pressure);
                // Serial.println(lcd_temp_string);
                /* 4 is mininum width, 2 is precision; float value is copied onto str_temp*/
                dtostrf(o2_concentration, 4, 2, str_temp);
                
                
                dtostrf(output_pressure, 4, 2, str_temp2);
                // sprintf(lcd_temp_string, "%f \%%   %2d psi", o2_concentration, output_pressure);
                sprintf(lcd_temp_string, "%s %%   %s psi", str_temp, str_temp2);
                               
                Serial.println(lcd_temp_string);
                lcd.setCursor(0, 1);
                lcd.print(lcd_temp_string);
            }

            // LCD Line 3
            // lcd.setCursor(0, 2);
            // blank for now

            // LCD Line 4
            if (prev_system_runtime_secs ^ system_runtime_secs) {
                prev_system_runtime_secs = system_runtime_secs;

                int secs = ( system_runtime_secs %  60);
                int mins = ((system_runtime_secs % (60 * 60)) / 60);
                int hrs  = ( system_runtime_secs / (60 * 60));
                sprintf(lcd_temp_string, "RUN TIME  %02d:%02d:%02d", hrs, mins, secs);
                Serial.println(lcd_temp_string);
                lcd.setCursor(0, 3);
                lcd.print(lcd_temp_string);
            }

            // System OFF check
            if (button_pressed == true)  {
                ui_state = UI_SYS_OFF_CHECK;
            }
            else {
                // no state change
            }
            break;

        case UI_SYS_OFF_CHECK:
            // System OFF check
            if (button_pressed == true)  {
                button_pressed = false;
                f_system_running = false;
                Serial.println("Stop Button Pressed!");
                lcd.setCursor(0, 3);
                lcd.print("Stop Button Pressed ");
                COMPRSSR_CNTRL (OFF);
                beep_for (SYS_OFF_BEEP);   // msecs

                delay (1000);
                lcd.setCursor(0, 3);
                lcd.print("O2 Cons. Stopping.. ");
                delay (1000);

                lcd.setCursor(0, 3);
                lcd.print("O2 Cons. Stopped..! ");
                ui_state = UI_LAST;
            }
            else {
                // back to run state..
                ui_state = UI_SYS_RUNNING;
            }
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
        Serial.print("ui_state : ");
        Serial.println(ui_state);
    }
}




void power_on_self_test (void) {

    lcd.setCursor(0, 1);
    lcd.print("Relay Tests..");

    // 1. Relay Z1TSOL
    lcd.setCursor(0, 2);
    lcd.print("Z1TSOL - OPEN   ");
    digitalWrite(Sieve_A_Valve_Z1,      OPEN_VALVE);
    beep_for (HIGH_BEEP);   // msecs
    delay(5000);

    lcd.setCursor(0, 2);
    lcd.print("Z1TSOL - CLOSE  ");
    digitalWrite(Sieve_A_Valve_Z1,      CLOSE_VALVE);
    beep_for (LOW_BEEP);   // msecs

    delay(5000);


    // 2. Relay Z2TSOL
    lcd.setCursor(0, 2);
    lcd.print("Z2TSOL - OPEN   ");
    digitalWrite(Sieve_B_Valve_Z2,      OPEN_VALVE);
    beep_for (HIGH_BEEP);   // msecs
    delay(5000);
    lcd.setCursor(0, 2);
    lcd.print("Z2TSOL - CLOSE  ");
    digitalWrite(Sieve_B_Valve_Z2,      CLOSE_VALVE);
    beep_for (LOW_BEEP);   // msecs
    delay(2000);

    // 3. Relay BCKFSOL
    lcd.setCursor(0, 2);
    lcd.print("BCKFSOL - OPEN  ");
    digitalWrite(PreCharge_Valve_BCKF,  OPEN_VALVE);
    beep_for (HIGH_BEEP);   // msecs
    delay(5000);

    lcd.setCursor(0, 2);
    lcd.print("BCKFSOL - CLOSE ");
    digitalWrite(PreCharge_Valve_BCKF,  CLOSE_VALVE);
    beep_for (LOW_BEEP);   // msecs
    delay(2000);

    //lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print("Compressor Test..");

    // 4. Compressor check
    // Open release valves.. to avoid blocking of compressor o/p during its test-run
    lcd.setCursor(0, 2);
    //        "...................."
    lcd.print("Opening releaseValvs");
    beep_for (HIGH_BEEP);   // msecs
    digitalWrite(Sieve_A_Valve_Z1,      OPEN_VALVE);
    digitalWrite(Sieve_B_Valve_Z2,      OPEN_VALVE);
    delay (1000);

    lcd.setCursor(0, 2);
    lcd.print("Compressor - ON     ");
    digitalWrite(PreCharge_Valve_BCKF,  OPEN_VALVE);
    beep_for (HIGH_BEEP);   // msecs
    delay(5000);
    COMPRSSR_CNTRL (ON);

    BUUZZER_CNTRL (OFF);
    COMPRSSR_CNTRL (OFF);
    lcd.setCursor(0, 2);
    lcd.print("Compressor - OFF    ");
    digitalWrite(PreCharge_Valve_BCKF,  CLOSE_VALVE);
    beep_for (LOW_BEEP);   // msecs
    delay (1000);

    lcd.clear();
    lcd.setCursor(0, 3);
    //  "1.3.5.7.9.........20"
    lcd.print("Testing over...!!   ");
    delay(2000);

}
