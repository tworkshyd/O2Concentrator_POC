// ui.c


#include "platform.h"
#include "ui.h"
#include "db.h"


static int  ui_state;

int buttonState = 0;         // variable for reading the pushbutton status
int button_presed;


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


void ui_task_main (void)    {

    static int     button_press_stability;

    buttonState = digitalRead(buttonPin);


    if (buttonState == LOW) {   // HIGH)   {
        button_press_stability++;
        if (button_press_stability >= 1000)   {
            button_press_stability = 0;
            button_presed = true;
        }
        // temp
        Serial.println(button_press_stability);
    }
    else {
        if (button_press_stability)
            button_press_stability--;
        if (button_press_stability == 0)    {
            button_presed = false;
        }
    }

    if (button_press_stability >= 1000)   {
        button_press_stability = 0;
    }

    switch (ui_state)
    {
        case 0:
            if (powerUpTimer.check())   {
                ui_state += 2;
                lcd.setCursor(0, 3);
                lcd.print("Press ButtonToStart!");
            }
            else {
                if (button_presed == true)  {
                    button_presed = false;
                    ui_state++;
                }
            }
            break;
        case 1:
            // Power On self test - on demand

            Serial.println("Start Button Pressed..!");
            Serial.println("Factory Mode..");

            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Factory Mode..");
            lcd.setCursor(0, 1);
            lcd.print("Relay Test..");
            delay (2000);


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
            lcd.clear();
            ui_print_welcome ();
            lcd.setCursor(0, 3);
            lcd.print("Press ButtonToStart!");
            ui_state++;
            break;
        case 2:
            // System ON check
            if (button_presed == true)  {
                button_presed = false;
                f_start = true;
                Serial.println("Start Button Pressed..!");
                lcd.setCursor(0, 3);
                lcd.print("Start Button Pressed");
                BUUZZER_CNTRL (ON);
                COMPRSSR_CNTRL (ON);
                delay (1500);
                BUUZZER_CNTRL (OFF);
                lcd.setCursor(0, 3);
                lcd.print("O2 Cons. Running... ");
                ui_state++;
            }
            break;

        case 3:
            // System OFF check
            if (button_presed == true)  {
                button_presed = false;
                f_start = false;
                Serial.println("Stop Button Pressed!");
                lcd.setCursor(0, 3);
                lcd.print("Stop Button Pressed ");
                BUUZZER_CNTRL (ON);
                COMPRSSR_CNTRL (OFF);
                delay (1000);
                lcd.setCursor(0, 3);
                lcd.print("O2 Cons. Stopping.. ");
                delay (1000);
                BUUZZER_CNTRL (OFF);
                lcd.setCursor(0, 3);
                lcd.print("O2 Cons. Stopped..! ");
                ui_state++;
            }
            break;
        case 4:
            ui_state = 2;
            break;
        default:
            ui_state = 2;
            break;
    }

}
