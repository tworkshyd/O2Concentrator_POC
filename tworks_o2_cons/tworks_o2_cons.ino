

// tworks_o2_cons.c

#include <UniversalTimer.h>

#include "platform.h"
#include "display.h"
#include "db.h"
#include "ui.h"



// RELAY PIN ASSIGNMENT
//**************************************************************************
int Sieve_A_Valve   = RLY_2;
int Sieve_B_Valve   = RLY_3;
int PreCharge_Valve = RLY_1;


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


// Create a timer with 100ms and repeating
UniversalTimer delay100msTimer(100, true);

// Create a timer with variable timing and non-repeating
UniversalTimer o2main_task_vDlyTmr(1000, true);


void setup() {

    Serial.begin(115200);
    Serial.println("Serial port initialized..!!");
    platform_init();
    db_init ();
    ui_init ();
    o2_cons_init ();

    

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
    pinMode(Sieve_A_Valve,   OUTPUT);
    pinMode(Sieve_B_Valve,   OUTPUT);
    pinMode(PreCharge_Valve, OUTPUT);

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
    digitalWrite(Sieve_A_Valve,     HIGH); // Turn on relay
    delay(Relay_Test_Delay);
    digitalWrite(Sieve_B_Valve,     HIGH); // Turn on relay
    delay(Relay_Test_Delay);
    digitalWrite(PreCharge_Valve,   HIGH); // Turn on relay
    delay(Relay_Test_Delay);
    Serial.println("Valve Relay Test Sequence Complete");
    delay(Relay_Test_Delay);

    // STARTUP PURGE
    //**************************************************************************
    Serial.println("Relay Test Sequence");
    digitalWrite(Sieve_A_Valve,   HIGH); // Turn on relay
    digitalWrite(Sieve_B_Valve,   HIGH); // Turn on relay
    digitalWrite(PreCharge_Valve, HIGH); // Turn on relay
    delay(Startup_Purge_Delay);


    // start task timers
    o2main_task_vDlyTmr.start();

}


void o2_main_task (void)    {

    if (o2main_task_vDlyTmr.check())    {

        switch (cycle)
        {
            case 0:
                //CYCLE 1
                //**************************************************************************
                Serial.println("Sieve A Charge / Sieve B Purge");
                digitalWrite(Sieve_A_Valve,   HIGH);
                digitalWrite(Sieve_B_Valve,   LOW);
                digitalWrite(PreCharge_Valve, LOW);
                // delay(Production_Delay);
                nb_delay = Production_Delay;
                cycle++;
                break;
            case 1:
                //CYCLE 2
                //**************************************************************************
                Serial.println("Sieve A Charge / Sieve B Purge / Flush/PreCharge");
                digitalWrite(Sieve_A_Valve,   HIGH);
                digitalWrite(Sieve_B_Valve,   LOW);
                digitalWrite(PreCharge_Valve, HIGH);
                // delay(Flush_Delay) ;
                nb_delay = Flush_Delay;
                cycle++;
                break;
            case 2:
                //CYCLE 3
                //**************************************************************************
                Serial.println("Sieve A Charge / Sieve B Charge / Flush/PreCharge");
                digitalWrite(Sieve_A_Valve,   HIGH);
                digitalWrite(Sieve_B_Valve,   HIGH);
                digitalWrite(PreCharge_Valve, HIGH);
                // delay(PreCharge_Delay);
                nb_delay = PreCharge_Delay;
                cycle++;
                break;
            case 3:
                //CYCLE 4
                //**************************************************************************
                Serial.println("Sieve A Purge / Sieve B Charge");
                digitalWrite(Sieve_A_Valve,   LOW);
                digitalWrite(Sieve_B_Valve,   HIGH);
                digitalWrite(PreCharge_Valve, LOW);
                // delay(Production_Delay);
                nb_delay = Production_Delay;
                cycle++;
                break;
            case 4:
                //CYCLE 5
                //**************************************************************************
                Serial.println("Sieve A Purge / Sieve B Charge / Flush/PreCharge");
                digitalWrite(Sieve_A_Valve,   LOW);
                digitalWrite(Sieve_B_Valve,   HIGH);
                digitalWrite(PreCharge_Valve, HIGH);
                // delay(Flush_Delay);
                nb_delay = Flush_Delay;
                cycle++;
                break;
            case 5:
                //CYCLE 6
                //**************************************************************************
                Serial.println("Sieve A Charge / Sieve B Charge / Flush/PreCharge");
                digitalWrite(Sieve_A_Valve,   HIGH);
                digitalWrite(Sieve_B_Valve,   HIGH);
                digitalWrite(PreCharge_Valve, HIGH);
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
