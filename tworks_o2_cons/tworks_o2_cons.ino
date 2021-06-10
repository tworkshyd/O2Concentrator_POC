

// tworks_o2_cons.c

//#include <UniversalTimer.h>

#include "platform.h"
#include "display.h"
#include "o2_sensor.h"
#include "o2_cons.h"
#include "config.h"
#include "logs.h"
#include "db.h"
#include "ui.h"


// Sytem tick time
#define TICK_time (10)



unsigned char cycle;



void o2_cons_init (void);
void o2_main_task (void);



void setup (void) {

    Serial.begin (115200);
    DBG_PRINTLN ("Serial port initialized..!!");
    platform_init ();
    db_init ();
    ui_init ();
    ads_init ();
    o2_cons_init ();

}


void loop (void) {

    // 1. periodic tasks are called here
    if (f_msec) {
        f_msec = 0;
        // milli second tasks go here..

    }
    else if (f_10msec) {
        f_10msec = 0;
        // 10 milli second tasks go here..

    }
    else if (f_100msec) {
        f_100msec = 0;
        // 100 milli second tasks go here..
    }
    else if (f_1sec) {
        f_1sec = 0;
        // 1 second tasks go here..
        f_sec_logs_task = 1;
        f_sec_change_ui_task = 1;
        f_sec_change_o2_task = 1;
        f_sec_change_sensor_task = 1;

        o2_sensor_scan ();
        // read_pressure ();

        DBG_PRINT (".");

    }
    else if (f_1min) {
        f_1min = 0;
        // 1 minute tasks go here..

    }
    else if (f_1hr) {
        f_1hr = 0;
        // 1 hour tasks go here..

    }

    // 2. contineous tasks are called here
    else {
        o2_main_task ();
        ui_task_main ();
        logs_task ();
    }

}



void o2_cons_init (void)    {

    // compute slope and constant values
    sensor_zero_calibration ();

    //  SET DELAY TIMING HERE
    //**************************************************************************
    Production_Delay    = 4000;   //8 sec and 2 sec gave 73% | 8 sec and 4 sec gave 75%
    Flush_Delay         = 200;
    PreCharge_Delay     = 200;


    // STARTUP PURGE
    //digitalWrite(Sieve_A_Valve_Z1,      OPEN_VALVE);
    do_control (SIEVE_A_VALVE_CONTROL,    OPEN_VALVE);
    //digitalWrite(Sieve_B_Valve_Z2,      OPEN_VALVE);
    do_control (SIEVE_B_VALVE_CONTROL,    OPEN_VALVE);
    //digitalWrite(PreCharge_Valve_BCKF,  OPEN_VALVE);
    do_control (SIEVE_FLUSH_VLV_CNTRL,    OPEN_VALVE);
    new_delay_msecs (1000);


}



void o2_main_task (void)    {

    static unsigned long int time_tag;

    if (f_system_running != true) {
        return;
    }
    if (f_sec_change_o2_task) {
        f_sec_change_o2_task = 0;
        production_time_secs++;
    }

    if (time_elapsed (time_tag) < nb_delay)  {
        return;
    }

    // else
    time_tag = systemtick_msecs;

    DBG_PRINTLN ("calling PSA logic..");

    chine_new_PSA_logic();

    if (nb_delay != prev_nb_delay)  {
        prev_nb_delay = nb_delay;

        DBG_PRINT ("nb_delay : ");
        DBG_PRINTLN (nb_delay);
    }

}



void chine_PSA_logic (void)  {

    switch (cycle)
    {
        case 0:
            //CYCLE 1
            //**************************************************************************
            DBG_PRINTLN("Sieve A Charge / Sieve B Purge");
            //digitalWrite(Sieve_A_Valve_Z1,      OPEN_VALVE);
            do_control (SIEVE_A_VALVE_CONTROL,    OPEN_VALVE);
            //digitalWrite(Sieve_B_Valve_Z2,      CLOSE_VALVE);
            do_control (SIEVE_B_VALVE_CONTROL,    CLOSE_VALVE);
            nb_delay = Production_Delay;
            cycle++;
            break;
        case 1:
            //CYCLE 2
            //**************************************************************************
            DBG_PRINTLN("Sieve A Charge / Sieve B Purge / Flush/PreCharge");
            //digitalWrite(Sieve_A_Valve_Z1,      OPEN_VALVE );
            do_control (SIEVE_A_VALVE_CONTROL,    OPEN_VALVE);
            //digitalWrite(Sieve_B_Valve_Z2,      OPEN_VALVE );
            do_control (SIEVE_B_VALVE_CONTROL,    OPEN_VALVE);
            nb_delay = Flush_Delay;
            cycle++;
            break;
        case 2:
            //CYCLE 3
            //**************************************************************************
            DBG_PRINTLN("Sieve A Charge / Sieve B Charge / Flush/PreCharge");
            //digitalWrite(Sieve_A_Valve_Z1,      CLOSE_VALVE);
            do_control (SIEVE_A_VALVE_CONTROL,    CLOSE_VALVE);
            //digitalWrite(Sieve_B_Valve_Z2,      OPEN_VALVE );
            do_control (SIEVE_B_VALVE_CONTROL,    OPEN_VALVE);
            nb_delay = Production_Delay;
            cycle++;
            break;
        case 3:
            //CYCLE 1
            //**************************************************************************
            DBG_PRINTLN("Sieve A Charge / Sieve B Purge");
            //digitalWrite(Sieve_A_Valve_Z1,      OPEN_VALVE );
            do_control (SIEVE_A_VALVE_CONTROL,    OPEN_VALVE);
            //digitalWrite(Sieve_B_Valve_Z2,      OPEN_VALVE );
            do_control (SIEVE_B_VALVE_CONTROL,    OPEN_VALVE);
            nb_delay = Flush_Delay;
            cycle++;
            break;

        default:
            cycle = 0;
            break;
    }

}



void chine_new_PSA_logic (void)  {

    switch (cycle)
    {
        case 0:
            //CYCLE 1
            //**************************************************************************
            DBG_PRINTLN("Sieve A Charge / Sieve B Purge");
            //digitalWrite(Sieve_A_Valve_Z1,      OPEN_VALVE );
            do_control (SIEVE_A_VALVE_CONTROL,    OPEN_VALVE);
            //digitalWrite(Sieve_B_Valve_Z2,      CLOSE_VALVE);
            do_control (SIEVE_B_VALVE_CONTROL,    CLOSE_VALVE);
            //digitalWrite(PreCharge_Valve_BCKF,  CLOSE_VALVE);
            do_control (SIEVE_FLUSH_VLV_CNTRL,    CLOSE_VALVE);
            nb_delay = Production_Delay;
            cycle++;
            break;
        case 1:
            //CYCLE 2
            //**************************************************************************
            DBG_PRINTLN("Sieve A Charge / Sieve B Purge");
            //digitalWrite(Sieve_A_Valve_Z1,      OPEN_VALVE );
            do_control (SIEVE_A_VALVE_CONTROL,    OPEN_VALVE);
            //digitalWrite(Sieve_B_Valve_Z2,      CLOSE_VALVE);
            do_control (SIEVE_B_VALVE_CONTROL,    CLOSE_VALVE);
            //digitalWrite(PreCharge_Valve_BCKF,  OPEN_VALVE );
            do_control (SIEVE_FLUSH_VLV_CNTRL,    OPEN_VALVE);
            nb_delay = Flush_Delay;
            cycle++;
            break;
        case 2:
            //CYCLE 3
            //**************************************************************************
            DBG_PRINTLN("Sieve A Charge / Sieve B Purge / Flush/PreCharge");
            //digitalWrite(Sieve_A_Valve_Z1,      OPEN_VALVE );
            do_control (SIEVE_A_VALVE_CONTROL,    OPEN_VALVE);
            //digitalWrite(Sieve_B_Valve_Z2,      OPEN_VALVE );
            do_control (SIEVE_B_VALVE_CONTROL,    OPEN_VALVE);
            //digitalWrite(PreCharge_Valve_BCKF,  OPEN_VALVE );
            do_control (SIEVE_FLUSH_VLV_CNTRL,    OPEN_VALVE);
            nb_delay = PreCharge_Delay;
            cycle++;
            break;
        case 3:
            //CYCLE 4
            //**************************************************************************
            DBG_PRINTLN("Sieve A Charge / Sieve B Charge / Flush/PreCharge");
            //digitalWrite(Sieve_A_Valve_Z1,      CLOSE_VALVE);
            do_control (SIEVE_A_VALVE_CONTROL,    CLOSE_VALVE);
            //digitalWrite(Sieve_B_Valve_Z2,      OPEN_VALVE );
            do_control (SIEVE_B_VALVE_CONTROL,    OPEN_VALVE);
            //digitalWrite(PreCharge_Valve_BCKF,  CLOSE_VALVE);
            do_control (SIEVE_FLUSH_VLV_CNTRL,    CLOSE_VALVE);
            nb_delay = Production_Delay;
            cycle++;
            break;

        case 4:
            //CYCLE 5
            //**************************************************************************
            DBG_PRINTLN("Sieve A Charge / Sieve B Charge / Flush/PreCharge");
            //digitalWrite(Sieve_A_Valve_Z1,      CLOSE_VALVE);
            do_control (SIEVE_A_VALVE_CONTROL,    CLOSE_VALVE);
            //digitalWrite(Sieve_B_Valve_Z2,      OPEN_VALVE );
            do_control (SIEVE_B_VALVE_CONTROL,    OPEN_VALVE);
            //digitalWrite(PreCharge_Valve_BCKF,  OPEN_VALVE );
            do_control (SIEVE_FLUSH_VLV_CNTRL,    OPEN_VALVE);
            nb_delay = Flush_Delay;
            cycle++;
            break;

        case 5:
            //CYCLE 6
            //**************************************************************************
            DBG_PRINTLN("Sieve A Charge / Sieve B Purge");
            //digitalWrite(Sieve_A_Valve_Z1,      OPEN_VALVE);
            do_control (SIEVE_A_VALVE_CONTROL,    OPEN_VALVE);
            //digitalWrite(Sieve_B_Valve_Z2,      OPEN_VALVE);
            do_control (SIEVE_B_VALVE_CONTROL,    OPEN_VALVE);
            //digitalWrite(PreCharge_Valve_BCKF,  OPEN_VALVE);
            do_control (SIEVE_FLUSH_VLV_CNTRL,    OPEN_VALVE);
            nb_delay = PreCharge_Delay;
            cycle++;
            break;
        default:
            cycle = 0;
            nb_delay = 0;
            break;
    }

}
