// ui.c


#include "platform.h"
#include "ui.h"
#include "db.h"


static int  ui_state;

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



#define BUTTON_PRESS_STABILITY_DLY  (100)  // in msecs


void ui_task_main (void)    {

    static int     button_press_stability;

    buttonState = digitalRead(buttonPin);



    if (buttonState == LOW) {   // press detection
        button_press_stability++;
        if (button_press_stability >= BUTTON_PRESS_STABILITY_DLY)   {
            button_press_stability = 0;
            button_pressed = true;
        }
        // temp
        Serial.println(button_press_stability);
    }
    else {  // release detection
        if (button_press_stability)
            button_press_stability--;
        if (button_press_stability == 0)    {
            button_pressed = false;
            button_press_count++;
            button_press_duration = button_press_count * BUTTON_PRESS_STABILITY_DLY;
        }
        // temp
        Serial.print  ("button_press_count : ");
        Serial.println(button_press_count);
    }

    if (button_press_stability >= BUTTON_PRESS_STABILITY_DLY)   {
        button_press_stability = 0;
    }

    switch (ui_state)
    {
        case UI_START:
            if (powerUpTimer.check())   {
                ui_state = UI_SYS_ON_CHECK;
                lcd.setCursor(0, 3);
                lcd.print("Press ButtonToStart!");
            }
            else {
                if (button_pressed == true)  {
                    button_pressed = false;
                    ui_state = UI_FACTORY_MODE;
                }
            }
            break;
        case UI_FACTORY_MODE:
            Serial.println("Start Button Pressed..!");
            Serial.println("Factory Mode..");

            if (button_pressed == true)  {
                button_pressed = false;
                ui_state = UI_CALIB_MODE;
            }

            // Power On self test - on demand
            power_on_self_test ();

            lcd.clear();
            ui_print_welcome ();
            lcd.setCursor(0, 3);
            lcd.print("Press ButtonToStart!");
            ui_state = UI_SYS_ON_CHECK;
            break;

        case UI_CALIB_MODE:

            break;

        case UI_SYS_ON_CHECK:
            // System ON check
            if (button_pressed == true)  {
                button_pressed = false;
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
                ui_state = UI_SYS_OFF_CHECK;
            }
            break;

        case UI_SYS_OFF_CHECK:
            // System OFF check
            if (button_pressed == true)  {
                button_pressed = false;
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
                ui_state = UI_LAST;
            }
            break;
        default:
        case UI_LAST:
            ui_state = UI_START;
            break;
    }

}




void power_on_self_test (void) {

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Factory Mode..");
    lcd.setCursor(0, 1);
    lcd.print("Relay Test..");
    delay (2000);

    // 1. Relay Z1TSOL
    lcd.setCursor(0, 2);
    lcd.print("Z1TSOL - ON   ");
    digitalWrite(Sieve_A_Valve_Z1,      LOW);       // on
    BUUZZER_CNTRL (ON);
    delay(2000);
    lcd.setCursor(0, 2);
    lcd.print("Z1TSOL - OFF");
    BUUZZER_CNTRL (OFF);
    digitalWrite(Sieve_A_Valve_Z1,      HIGH);      // off
    delay(2000);

    // 2. Relay Z2TSOL
    lcd.setCursor(0, 2);
    lcd.print("Z2TSOL - ON   ");
    BUUZZER_CNTRL (ON);
    digitalWrite(Sieve_B_Valve_Z2,      LOW);       // on
    delay(2000);
    BUUZZER_CNTRL (OFF);
    lcd.setCursor(0, 2);
    lcd.print("Z2TSOL - OFF   ");
    digitalWrite(Sieve_B_Valve_Z2,      HIGH);      // off
    delay(2000);

    // 3. Relay BCKFSOL
    lcd.setCursor(0, 2);
    lcd.print("BCKFSOL - ON   ");
    BUUZZER_CNTRL (ON);
    digitalWrite(PreCharge_Valve_BCKF,      LOW);       // on
    delay(2000);
    BUUZZER_CNTRL (OFF);
    lcd.setCursor(0, 2);
    lcd.print("BCKFSOL - OFF  ");
    digitalWrite(PreCharge_Valve_BCKF,      HIGH);      // off
    delay(2000);

    //lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print("Compressor Test..");

    // 4. Compressor check
    // Open release valves.. to avoid blocking of compressor o/p during its test-run
    lcd.setCursor(0, 2);
    lcd.print("Opening releaseValvs");
    delay (1000);
    digitalWrite(Sieve_A_Valve_Z1,      LOW);      // on
    digitalWrite(Sieve_B_Valve_Z2,      LOW);      // on
    lcd.setCursor(0, 2);
    lcd.print("Compressor - ON     ");
    BUUZZER_CNTRL (ON);
    COMPRSSR_CNTRL (ON);
    digitalWrite(PreCharge_Valve_BCKF,      LOW);       // on
    delay(5000);
    BUUZZER_CNTRL (OFF);
    COMPRSSR_CNTRL (OFF);
    lcd.setCursor(0, 2);
    lcd.print("Compressor - OFF    ");
    digitalWrite(PreCharge_Valve_BCKF,      HIGH);      // off
    delay (1000);

    lcd.clear();
    lcd.setCursor(0, 3);
    //  "1.3.5.7.9.........20"
    lcd.print("Testing over...!!   ");
    delay(2000);

}
