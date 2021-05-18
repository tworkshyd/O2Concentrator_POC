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


void ui_task_main (void)    {

    static int     button_press_stability;

    buttonState = digitalRead(buttonPin);


    if (buttonState == HIGH)   {
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
            }
            else {
                if (button_presed == true)  {
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
            lcd.setCursor(0, 2);
            lcd.print("Compressor - ON   ");
            BUUZZER_CNTRL (ON);
            digitalWrite(PreCharge_Valve_BCKF,      LOW);       // on
            delay(5000);
            BUUZZER_CNTRL (OFF);
            lcd.setCursor(0, 2);
            lcd.print("Compressor - OFF  ");
            digitalWrite(PreCharge_Valve_BCKF,      HIGH);      // off
            delay (1000);
            
            lcd.clear();
            lcd.setCursor(0, 3);
            lcd.print("Testing over...!! ");
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
                f_start = false;
                Serial.println("Stop Button Pressed!");
                lcd.setCursor(0, 3);
                lcd.print("Stop Button Pressed ");
                BUUZZER_CNTRL (ON);
                COMPRSSR_CNTRL (ON);
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
