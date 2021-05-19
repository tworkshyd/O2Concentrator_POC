

// tworks_o2_cons.c

#include <UniversalTimer.h>

#include "platform.h"
#include "display.h"
#include "commn.h"
#include "db.h"
#include "ui.h"


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

unsigned long nb_delay;
unsigned char cycle;

void o2_cons_init (void);
void o2_main_task (void);


// Create a timer with 5 sec and non-repeating
UniversalTimer powerUpTimer(5000, false);

// Create a timer with 100ms and repeating
// UniversalTimer delay100msTimer(100, true);

// Create a timer with variable timing and non-repeating
UniversalTimer o2main_task_vDlyTmr(1000, true);


void setup() {

    Serial.begin(115200);
    Serial.println("Serial port initialized..!!");
    platform_init();
    db_init ();
    ui_init ();
    commn_init();
    o2_cons_init ();

    // start task timers
    powerUpTimer.start();


}


void loop() {



    ui_task_main ();

    if (f_start == true) {
        o2_main_task ();
    }

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

    // VALVE RELAY TEST SEQUENCE
    //**************************************************************************
    Serial.println("Relay Test Sequence");
    digitalWrite(Sieve_A_Valve_Z1,     HIGH); // Turn on relay
    delay(Relay_Test_Delay);
    digitalWrite(Sieve_B_Valve_Z2,     HIGH); // Turn on relay
    delay(Relay_Test_Delay);
    digitalWrite(PreCharge_Valve_BCKF,   HIGH); // Turn on relay
    delay(Relay_Test_Delay);
    Serial.println("Valve Relay Test Sequence Complete");
    delay(Relay_Test_Delay);

    // STARTUP PURGE
    //**************************************************************************
    Serial.println("Relay Test Sequence");
    digitalWrite(Sieve_A_Valve_Z1,   HIGH); // Turn on relay
    digitalWrite(Sieve_B_Valve_Z2,   HIGH); // Turn on relay
    digitalWrite(PreCharge_Valve_BCKF, HIGH); // Turn on relay
    delay(Startup_Purge_Delay);


    // start task timers
    o2main_task_vDlyTmr.start();
    powerUpTimer.start();

}


void o2_main_task (void)    {

    if (o2main_task_vDlyTmr.check())    {

        switch (cycle)
        {
            case 0:
                //CYCLE 1
                //**************************************************************************
                Serial.println("Sieve A Charge / Sieve B Purge");
                digitalWrite(Sieve_A_Valve_Z1,      LOW);   // HIGH);
                digitalWrite(Sieve_B_Valve_Z2,      HIGH);  // LOW);
                digitalWrite(PreCharge_Valve_BCKF,  HIGH);  // LOW);
                // delay(Production_Delay);
                nb_delay = Production_Delay;
                cycle++;
                break;
            case 1:
                //CYCLE 2
                //**************************************************************************
                Serial.println("Sieve A Charge / Sieve B Purge / Flush/PreCharge");
                digitalWrite(Sieve_A_Valve_Z1,      LOW);   // HIGH);
                digitalWrite(Sieve_B_Valve_Z2,      HIGH);  // LOW);
                digitalWrite(PreCharge_Valve_BCKF,  LOW);   // HIGH);
                // delay(Flush_Delay) ;
                nb_delay = Flush_Delay;
                cycle++;
                break;
            case 2:
                //CYCLE 3
                //**************************************************************************
                Serial.println("Sieve A Charge / Sieve B Charge / Flush/PreCharge");
                digitalWrite(Sieve_A_Valve_Z1,      LOW);   // HIGH);
                digitalWrite(Sieve_B_Valve_Z2,      LOW);   // HIGH);
                digitalWrite(PreCharge_Valve_BCKF,  LOW);   // HIGH);
                // delay(PreCharge_Delay);
                nb_delay = PreCharge_Delay;
                cycle++;
                break;
            case 3:
                //CYCLE 4
                //**************************************************************************
                Serial.println("Sieve A Purge / Sieve B Charge");
                digitalWrite(Sieve_A_Valve_Z1,      HIGH);  // LOW);
                digitalWrite(Sieve_B_Valve_Z2,      LOW);   // HIGH);
                digitalWrite(PreCharge_Valve_BCKF,  HIGH);  // LOW);
                // delay(Production_Delay);
                nb_delay = Production_Delay;
                cycle++;
                break;
            case 4:
                //CYCLE 5
                //**************************************************************************
                Serial.println("Sieve A Purge / Sieve B Charge / Flush/PreCharge");
                digitalWrite(Sieve_A_Valve_Z1,      HIGH);  // LOW);
                digitalWrite(Sieve_B_Valve_Z2,      LOW);   // HIGH);
                digitalWrite(PreCharge_Valve_BCKF,  LOW);   // HIGH);
                // delay(Flush_Delay);
                nb_delay = Flush_Delay;
                cycle++;
                break;
            case 5:
                //CYCLE 6
                //**************************************************************************
                Serial.println("Sieve A Charge / Sieve B Charge / Flush/PreCharge");
                digitalWrite(Sieve_A_Valve_Z1,      LOW);   // HIGH);
                digitalWrite(Sieve_B_Valve_Z2,      LOW);   // HIGH);
                digitalWrite(PreCharge_Valve_BCKF,  LOW);   // HIGH);
                // delay(PreCharge_Delay) ;
                nb_delay = PreCharge_Delay;
                cycle = 0;
                break;
            default:
                cycle = 0;
                break;
        }
    }

}
