

// tworks_o2_cons.c

#include <UniversalTimer.h>

#include "platform.h"
#include "display.h"
#include "commn.h"
#include "o2_sensor.h"
#include "db.h"
#include "ui.h"


// Sytem tick time
#define TICK_time (10)


// moved from here to platform.h
/*
    // RELAY PIN ASSIGNMENT
    //**************************************************************************
    int Sieve_A_Valve_Z1     = RLY_3;    // Z1TSOL
    int Sieve_B_Valve_Z2     = RLY_2;    // Z2TSOL
    int PreCharge_Valve_BCKF = RLY_1;    // BACKFSOL
*/

// VARIABLE CREATION
//**************************************************************************
unsigned long Relay_Test_Delay;     // delay variable creation
unsigned long Startup_Purge_Delay;  // delay variable creation
unsigned long Production_Delay;     // delay variable creation
unsigned long Flush_Delay;          // delay variable creation
unsigned long PreCharge_Delay;      // delay variable creation
unsigned long wait_delay;

unsigned long nb_delay;
unsigned long prev_nb_delay;
unsigned char cycle;

void o2_cons_init (void);
void o2_main_task (void);


// Create a timer with 5 sec and non-repeating
UniversalTimer powerUpTimer(5000, false);

// Create a timer with 100ms and repeating
UniversalTimer system_tick_timer (TICK_time, true);



void setup (void) {

    Serial.begin(115200);
    Serial.println("Serial port initialized..!!");
    platform_init();
    db_init ();
    ui_init ();
    commn_init();
    ads_init ();
    o2_cons_init ();

    // start task timers
    powerUpTimer.start();
    system_tick_timer.start();


}


void loop (void) {

    static unsigned long time_tag;

    if (system_tick_timer.check())    {
        systemtick_msecs += TICK_time;
    }



    if (time_elapsed (time_tag) > 1000) {
        time_tag = systemtick_msecs;

        f_sec_change_ui_task = 1;
        f_sec_change_o2_task = 1;
        f_sec_change_sensor_task = 1;

        o2_sensor_scan ();
        read_pressure ();
        
        if (f_system_running == true) {
            system_runtime_secs++;
            o2_main_task ();
        }
        Serial.print(".");
    }



    ui_task_main ();
    


}



void o2_cons_init (void)    {

    // SET PIN MODE FOR PINS IN PROGRAM
    //**************************************************************************
    pinMode(Sieve_A_Valve_Z1,   OUTPUT);
    pinMode(Sieve_B_Valve_Z2,   OUTPUT);
    pinMode(PreCharge_Valve_BCKF, OUTPUT);

    //  SET DELAY TIMING HERE
    //**************************************************************************
    Relay_Test_Delay    = 1000;
    Startup_Purge_Delay = 1000;
    Production_Delay    = 4000;
    Flush_Delay         = 450;
    PreCharge_Delay     = 700;
    wait_delay          = 2000;

    // VALVE RELAY TEST SEQUENCE
    /*
        //**************************************************************************
        Serial.println("Relay Test Sequence");
        digitalWrite(Sieve_A_Valve_Z1,       HIGH); // Turn on relay
        delay(Relay_Test_Delay);
        digitalWrite(Sieve_B_Valve_Z2,       HIGH); // Turn on relay
        delay(Relay_Test_Delay);
        digitalWrite(PreCharge_Valve_BCKF,   HIGH); // Turn on relay
        delay(Relay_Test_Delay);
        Serial.println("Valve Relay Test Sequence Complete");
        delay(Relay_Test_Delay);
    */

    // STARTUP PURGE
    //**************************************************************************
    /*
        Serial.println("Relay Test Sequence");
        digitalWrite(Sieve_A_Valve_Z1,      HIGH); // Turn on relay
        digitalWrite(Sieve_B_Valve_Z2,      HIGH); // Turn on relay
        digitalWrite(PreCharge_Valve_BCKF,  HIGH); // Turn on relay
        delay(Startup_Purge_Delay);
    */
    // Open release valves.. to avoid blocking of compressor o/p during its test-run
    /*
        lcd.clear();
        lcd.setCursor(0, 1);
        //        "...................."
        lcd.print("System booting up..!");
        lcd.setCursor(0, 2);
        //        "...................."
        lcd.print("Opening releaseValvs");
    */
    beep_for (HIGH_BEEP);   // msecs
    digitalWrite(Sieve_A_Valve_Z1,      OPEN_VALVE);
    digitalWrite(Sieve_B_Valve_Z2,      OPEN_VALVE);
    delay (1000);

    // start task timers
    powerUpTimer.start();

}


void oxikit_PSA_logic (void)  {

    switch (cycle)
    {
        case 0:
            //CYCLE 1
            //**************************************************************************
            Serial.println("Sieve A Charge / Sieve B Purge");
            digitalWrite(Sieve_A_Valve_Z1,      OPEN_VALVE);  // LOW);   // HIGH);
            digitalWrite(Sieve_B_Valve_Z2,      CLOSE_VALVE); // HIGH);  // LOW);
            digitalWrite(PreCharge_Valve_BCKF,  CLOSE_VALVE); // HIGH);  // LOW);
            // delay(Production_Delay);
            nb_delay = Production_Delay;
            cycle++;
            break;
        case 1:
            //CYCLE 2
            //**************************************************************************
            Serial.println("Sieve A Charge / Sieve B Purge / Flush/PreCharge");
            digitalWrite(Sieve_A_Valve_Z1,      OPEN_VALVE );  // LOW);   // HIGH);
            digitalWrite(Sieve_B_Valve_Z2,      CLOSE_VALVE);  // HIGH);  // LOW);
            digitalWrite(PreCharge_Valve_BCKF,  OPEN_VALVE );  // LOW);   // HIGH);
            // delay(Flush_Delay) ;
            nb_delay = Flush_Delay;
            cycle++;
            break;
        case 2:
            //CYCLE 3
            //**************************************************************************
            Serial.println("Sieve A Charge / Sieve B Charge / Flush/PreCharge");
            digitalWrite(Sieve_A_Valve_Z1,      OPEN_VALVE);  // LOW);   // HIGH);
            digitalWrite(Sieve_B_Valve_Z2,      OPEN_VALVE);  // LOW);   // HIGH);
            digitalWrite(PreCharge_Valve_BCKF,  OPEN_VALVE);  // LOW);   // HIGH);
            // delay(PreCharge_Delay);
            nb_delay = PreCharge_Delay;
            cycle++;
            break;
        case 3:
            //CYCLE 4
            //**************************************************************************
            Serial.println("Sieve A Purge / Sieve B Charge");
            digitalWrite(Sieve_A_Valve_Z1,      CLOSE_VALVE);  // HIGH);  // LOW);
            digitalWrite(Sieve_B_Valve_Z2,      OPEN_VALVE );  // LOW);   // HIGH);
            digitalWrite(PreCharge_Valve_BCKF,  CLOSE_VALVE);  // HIGH);  // LOW);
            // delay(Production_Delay);
            nb_delay = Production_Delay;
            cycle++;
            break;
        case 4:
            //CYCLE 5
            //**************************************************************************
            Serial.println("Sieve A Purge / Sieve B Charge / Flush/PreCharge");
            digitalWrite(Sieve_A_Valve_Z1,      CLOSE_VALVE);  // HIGH);  // LOW);
            digitalWrite(Sieve_B_Valve_Z2,      OPEN_VALVE );  // LOW);   // HIGH);
            digitalWrite(PreCharge_Valve_BCKF,  OPEN_VALVE );  // LOW);   // HIGH);
            // delay(Flush_Delay);
            nb_delay = Flush_Delay;
            cycle++;
            break;
        case 5:
            //CYCLE 6
            //**************************************************************************
            Serial.println("Sieve A Charge / Sieve B Charge / Flush/PreCharge");
            digitalWrite(Sieve_A_Valve_Z1,      OPEN_VALVE);  // LOW);   // HIGH);
            digitalWrite(Sieve_B_Valve_Z2,      OPEN_VALVE);  // LOW);   // HIGH);
            digitalWrite(PreCharge_Valve_BCKF,  OPEN_VALVE);  // LOW);   // HIGH);
            // delay(PreCharge_Delay) ;
            nb_delay = PreCharge_Delay;
            cycle = 0;
            break;
        default:
            cycle = 0;
            break;
    }

}


void tworks_PSA_logic (void)  {

    switch (cycle)
    {
        // -------------------------- Tank - 1 -----------------------------------------
        case 0:
            //CYCLE 1
            //**************************************************************************
            Serial.println("Sieve A Charge / Sieve B Purge");
            digitalWrite(Sieve_A_Valve_Z1,      OPEN_VALVE);
            digitalWrite(Sieve_B_Valve_Z2,      CLOSE_VALVE);
            digitalWrite(PreCharge_Valve_BCKF,  CLOSE_VALVE);
            // delay(Production_Delay);
            nb_delay = Production_Delay;
            cycle++;
            break;
        case 1:
            //CYCLE 2
            //**************************************************************************
            Serial.println("Sieve A Charge / Sieve B Purge / Flush/PreCharge");
            digitalWrite(Sieve_A_Valve_Z1,      OPEN_VALVE );
            digitalWrite(Sieve_B_Valve_Z2,      CLOSE_VALVE);
            digitalWrite(PreCharge_Valve_BCKF,  OPEN_VALVE );
            // delay(Flush_Delay) ;
            nb_delay = Flush_Delay;
            cycle++;
            break;
        case 2:
            //CYCLE 3
            //**************************************************************************
            Serial.println("Sieve A Charge / Sieve B Charge / Flush/PreCharge");
            digitalWrite(Sieve_A_Valve_Z1,      OPEN_VALVE);
            digitalWrite(Sieve_B_Valve_Z2,      OPEN_VALVE);
            digitalWrite(PreCharge_Valve_BCKF,  OPEN_VALVE);
            // delay(PreCharge_Delay);
            nb_delay = PreCharge_Delay;
            cycle++;
            break;
        case 3:
            //CYCLE 4
            //**************************************************************************
            Serial.println("Sieve A Purge / Sieve B Charge");
            digitalWrite(Sieve_A_Valve_Z1,      CLOSE_VALVE);
            digitalWrite(Sieve_B_Valve_Z2,      CLOSE_VALVE);
            digitalWrite(PreCharge_Valve_BCKF,  CLOSE_VALVE);
            // delay(Production_Delay);
            nb_delay = wait_delay;
            cycle++;
            break;


        // -------------------------- Tank - 2 -----------------------------------------
        case 4:
            //CYCLE 1
            //**************************************************************************
            Serial.println("Sieve A Charge / Sieve B Purge");
            digitalWrite(Sieve_A_Valve_Z1,      CLOSE_VALVE);
            digitalWrite(Sieve_B_Valve_Z2,      OPEN_VALVE);
            digitalWrite(PreCharge_Valve_BCKF,  CLOSE_VALVE);
            // delay(Production_Delay);
            nb_delay = Production_Delay;
            cycle++;
            break;
        case 5:
            //CYCLE 2
            //**************************************************************************
            Serial.println("Sieve A Charge / Sieve B Purge / Flush/PreCharge");
            digitalWrite(Sieve_A_Valve_Z1,      CLOSE_VALVE );
            digitalWrite(Sieve_B_Valve_Z2,      OPEN_VALVE);
            digitalWrite(PreCharge_Valve_BCKF,  OPEN_VALVE );
            // delay(Flush_Delay) ;
            nb_delay = Flush_Delay;
            cycle++;
            break;
        case 6:
            //CYCLE 3
            //**************************************************************************
            Serial.println("Sieve A Charge / Sieve B Charge / Flush/PreCharge");
            digitalWrite(Sieve_A_Valve_Z1,      OPEN_VALVE);
            digitalWrite(Sieve_B_Valve_Z2,      OPEN_VALVE);
            digitalWrite(PreCharge_Valve_BCKF,  OPEN_VALVE);
            // delay(PreCharge_Delay);
            nb_delay = PreCharge_Delay;
            cycle++;
            break;
        case 7:
            //CYCLE 4
            //**************************************************************************
            Serial.println("Sieve A Purge / Sieve B Charge");
            digitalWrite(Sieve_A_Valve_Z1,      CLOSE_VALVE);
            digitalWrite(Sieve_B_Valve_Z2,      CLOSE_VALVE);
            digitalWrite(PreCharge_Valve_BCKF,  CLOSE_VALVE);
            // delay(Production_Delay);
            nb_delay = wait_delay;
            cycle++;
            break;


        default:
            cycle = 0;
            break;
    }

}



void o2_main_task (void)    {

    static unsigned long  time_tag;

    if (time_elapsed (time_tag) < nb_delay)  {
        return;
    }

    // else
    time_tag = systemtick_msecs;

    // oxikit_PSA_logic ();
    tworks_PSA_logic ();

    if (nb_delay != prev_nb_delay)  {
        prev_nb_delay = nb_delay;

        Serial.print ("nb_delay : ");
        Serial.println (nb_delay);
    }



}
