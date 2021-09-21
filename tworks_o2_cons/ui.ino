// ui.c


#include <string.h>

#include "platform.h"
#include "logs.h"
#include "ui.h"
#include "db.h"


static int  ui_state, prev_ui_state;
static int  f_state_changed;




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

/* User Interface..
 *  
 *  (') --> short press
 *  (*) ---> Long press
 *  
 *  after power ON .. 
 *   |
 *   |<----------- 5 seconds quiescent period --------->|         .------- <------<------<------<------<------<------.
 *     |   |   |                                        |         |                                                  ^
 *     |   |   |                                        |--> Long press  (*): "START"  ----> short press : "STOP" ---'
 *     |   |   |                                        |
 *     |   |   |                                        |
 *     |   |   |                                        
 *     |   |  (')(') (twice) ---> Config mode 
 *     |   |                         |
 *     |   |                        (*) 
 *     |   |                         |
 *     |   |                    Reset Timers --> (') --->>>
 *     |   |                         |
 *     |   |                        (*)
 *     |   |                         |
 *     |   |                       [sure] -----> (') --->>>           
 *     |   |                         |
 *     |   |                        (*)          
 *     |   |                         |
 *     |   |                     [confirm]  ---> (') --->>>        
 *     |   |
 *     |   |
 *     |  (')(')(') (thrice) ---> Factory mode
 *     |                              |
 *     |                             (*)
 *     |                              |
 *     |                             SA-Valve -----> (') -----> SB-Valve ----> (') ----> BKP-Valve ----> (') ----> Compressor ---> (')----->>>
 *     |                              |
 *     |                             (*)                          < do >                  < do >                     < do >
 *     |                              |
 *     |                             OPEN ---> (') ---> CLOSE ---> (') -->.
 *     |                              |                                   |
 *     |                              '------- <------------ <------------'
 *     |
 *     |
 *    (')(')(')(')(') (5 times) --> Calibration mode
 *                                    |
 *                                   (*)
 *                                    |
 *                                   O2 Sensor -----> (') -----> Press-sensor ----->>>
 *                                    |
 *                                   (*)                          < do >                  
 *                                    |
 *                                    |------- <------------ <------- <------------.
 *                                    |                                            |
 *                                    0% ---> (') ---> 21% ---> (') ---> 100% ---->'
 *                                    |            |    |            |    |        |  
 *                                   (*)           ^   (*)           ^   (*)       ^
 *                                    |            |    |            |    |        |
 *                                  [Save] --->----'  [Save] --->----'  [Save] -->-'
 *                                  
 *                                                                       
 */
 

void ui_task_main (void)    {
   
    //static unsigned long  time_tag;
    static unsigned int   state_time;
    char                  str_temp[6];
    char                  str_temp2[6];

    //buttonState = digitalRead(buttonPin);

    if (f_sec_change_ui_task) {
        f_sec_change_ui_task = 0;
        state_time++;
    }

    switch (ui_state)
    {
        case UI_START:
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
            if (start_switch_pressed == true)  {
                f_system_running = true;

                DBG_PRINTLN("Start Button Pressed..!");
                lcd.setCursor(0, 3);
                lcd.print("Start Button Pressed");
                beep_for (SYS_ON_BEEP);   // msecs

                COMPRSSR_CNTRL (COMPRSSR_ON);
                // new_delay_msecs (1000);

                lcd.setCursor(0, 3);
                lcd.print("O2 Cons. Starting... ");
                // new_delay_msecs (1000);

                ui_state = UI_SYS_RUNNING;
            }
            break;


        case UI_SYS_RUNNING:
            // system running
            // System OFF check
            if (start_switch_pressed == false)  {
                f_system_running = false;
                ui_state = UI_SYS_SHUT_OFF;
            }
            else {
                // no state change
            }
            break;

        case UI_SYS_SHUT_OFF:
            // System OFF 
            COMPRSSR_CNTRL (COMPRSSR_OFF);
            beep_for (SYS_OFF_BEEP);   // msecs
            new_delay_msecs (1000);
            lcd.setCursor(0, 3);
            lcd.print("O2 Cons. Stopping.. ");
            new_delay_msecs (1000);
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
        DBG_PRINT("ui_state : ");
        DBG_PRINTLN(ui_state);
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
    new_delay_msecs (5000);

    lcd.setCursor(0, 2);
    lcd.print("Z1TSOL - CLOSE  ");
    digitalWrite(Sieve_A_Valve_Z1,      CLOSE_VALVE);
    beep_for (LOW_BEEP);   // msecs

    new_delay_msecs (5000);


    // 2. Relay Z2TSOL
    lcd.setCursor(0, 2);
    lcd.print("Z2TSOL - OPEN   ");
    digitalWrite(Sieve_B_Valve_Z2,      OPEN_VALVE);
    beep_for (HIGH_BEEP);   // msecs
    new_delay_msecs (5000);
    lcd.setCursor(0, 2);
    lcd.print("Z2TSOL - CLOSE  ");
    digitalWrite(Sieve_B_Valve_Z2,      CLOSE_VALVE);
    beep_for (LOW_BEEP);   // msecs
    new_delay_msecs (2000);

    // 3. Relay BCKFSOL
    lcd.setCursor(0, 2);
    lcd.print("BCKFSOL - OPEN  ");
    digitalWrite(PreCharge_Valve_BCKF,  OPEN_VALVE);
    beep_for (HIGH_BEEP);   // msecs
    new_delay_msecs (5000);

    lcd.setCursor(0, 2);
    lcd.print("BCKFSOL - CLOSE ");
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
    digitalWrite(Sieve_A_Valve_Z1,      OPEN_VALVE);
    digitalWrite(Sieve_B_Valve_Z2,      OPEN_VALVE);
    new_delay_msecs (1000);

    lcd.setCursor(0, 2);
    lcd.print("Compressor - ON     ");
    digitalWrite(PreCharge_Valve_BCKF,  OPEN_VALVE);
    beep_for (HIGH_BEEP);   // msecs
    new_delay_msecs (5000);
    COMPRSSR_CNTRL (COMPRSSR_ON);

    BUUZZER_CNTRL (BUZZ_OFF);
    COMPRSSR_CNTRL (COMPRSSR_OFF);
    lcd.setCursor(0, 2);
    lcd.print("Compressor - OFF    ");
    digitalWrite(PreCharge_Valve_BCKF,  CLOSE_VALVE);
    beep_for (LOW_BEEP);   // msecs
    new_delay_msecs (1000);

    lcd.clear();
    lcd.setCursor(0, 3);
    //  "1.3.5.7.9.........20"
    lcd.print("Testing over...!!   ");
    new_delay_msecs (2000);

}
