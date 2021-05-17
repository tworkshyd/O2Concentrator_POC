// ui.c


#include "platform.h"
#include "ui.h"
#include "db.h"


static int  ui_state;

int buttonState = 0;         // variable for reading the pushbutton status
int button_presed;




void ui_init (void) {

    // Print a message to the LCD.
    lcd.setCursor(0, 0);
    lcd.print(" Welcome to Tworks");
    lcd.setCursor(0, 1);
    lcd.print("     Hyderabad");
    lcd.setCursor(0, 2);
    lcd.print("Oxygen Concentrator!");

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
            // System ON check
            if (button_presed == true)  {
                f_start = true;
                Serial.println("Start Button Pressed..!");
                lcd.setCursor(0, 3);
                lcd.print("Start Button Pressed");
                BUUZZER_CNTRL (ON);
                COMPRSSR_CNTRL (ON);
                delay (2000);
                BUUZZER_CNTRL (OFF);
                lcd.setCursor(0, 3);
                lcd.print("O2 Cons. Running... ");
                ui_state++;
            }
            break;

        case 1:
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
        case 2:
            ui_state = 0;
            break;
        default:
            ui_state = 0;
            break;
    }



}
