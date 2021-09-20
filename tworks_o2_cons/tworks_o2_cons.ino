



// tworks_o2_cons.c

//#include <UniversalTimer.h>
#include <extEEPROM.h>

#include "o2_sensor.h"
#include "platform.h"
#include "o2_cons.h"
#include "display.h"
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
    ads_init ();
    db_init ();
    //temp
    // test_ads1115 ();
    o2_cons_init ();
    init_7segments ();

    // temp
    test_7segments ();
    
    display_o2 (00.0);
    display_total_run_hours (total_run_time_secs);    
    ui_init ();

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
        // test_neo_pixcell_leds ();
        display_o2 (o2_concentration);       
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

/*
    // Following timing settings used with 3 valves (2  - 3/2, 1 - 2/2) circuit; 6 step cycle;
    Production_Delay    = 5000;
    Flush_Delay         = 400;
    PreCharge_Delay     = 700;
*/

    // Following timing settings used with 2 valves, 1 orifice circuit; 4 step cycle;
    Production_Delay    = 5400;
    PreCharge_Delay     = 700; 


    // STARTUP PURGE
    //digitalWrite(Sieve_A_Valve_Z1,      OPEN_VALVE);
    do_control (SIEVE_A_VALVE_CONTROL,    CLOSE_VALVE);
    //digitalWrite(Sieve_B_Valve_Z2,      OPEN_VALVE);
    do_control (SIEVE_B_VALVE_CONTROL,    CLOSE_VALVE);
    //digitalWrite(PreCharge_Valve_BCKF,  OPEN_VALVE);
    // do_control (SIEVE_FLUSH_VLV_CNTRL, CLOSE_VALVE);
    do_control (SIEVE_FLUSH_VLV_CNTRL,    CLOSE_VALVE);
    new_delay_msecs (500);


}



void o2_main_task (void)    {

    static unsigned long int time_tag;
    static uint8_t           quadrant;
    static uint8_t           once_done;
    
    int secs, mins, hrs;
    
    if (f_system_running != true) {
        once_done = 0;
        return;
    }
    else {
        if (!once_done) {
          once_done = 1;
          COMPRSSR_CNTRL (COMPRSSR_ON);
        }
    }
    
    if (f_sec_change_o2_task) {
        f_sec_change_o2_task = 0;
        current_run_time_secs++;
        total_run_time_secs++;


      // display run hours, 45 seconds current run hours, 15 seconds total runhours
      secs = ( current_run_time_secs %  60);
      mins = ((current_run_time_secs % (60 * 60)) / 60);
      hrs  = ( current_run_time_secs / (60 * 60));
         
      if ((current_run_time_secs % 15) == 0) {
      // if ((current_run_time_secs % 5) == 0) {   // temp to speed up the testing
          quadrant++;
          if (quadrant > 3) {
              quadrant = 0;
          }
      }

      lc.shutdown (0, false);
      /* Set the brightness to a medium values */
      lc.setIntensity (0, _7_SEGMENT_INTENSITY);  // 0x0 TO 0xF
      /* and clear the display */
      lc.clearDisplay (0);
	  
	  display_o2 (o2_concentration);  
      
        switch (quadrant) 
        {
            case 0:
            case 1:
            case 2:
              display_current_run_time(hrs, mins);
              neo_pixcel_data (TRN_DISPLAY, 0); 
              neo_pixcel_data (CRN_DISPLAY, 1);
              break;
           case 3:
              hrs = (total_run_time_secs / (60 * 60));
              display_total_run_hours(hrs);       
              neo_pixcel_data (TRN_DISPLAY, 1); 
              neo_pixcel_data (CRN_DISPLAY, 0);   
              break;
        }
      
    }
    
    if (time_elapsed (time_tag) < nb_delay)  {
        return;
    }

    // else
    time_tag = systemtick_msecs;

    DBG_PRINTLN ("calling PSA logic..");

    //tworks3_PSA_logic();
    tworks2_PSA_logic();


    if (nb_delay != prev_nb_delay)  {
        prev_nb_delay = nb_delay;

        DBG_PRINT ("nb_delay : ");
        DBG_PRINTLN (nb_delay);
    }

    // repeating display refresh after valves operation..
//    switch (quadrant) 
//    {
//        case 0:
//        case 1:
//        case 2:
//          display_current_run_time(hrs, mins);
//          break;
//       case 3:
//          hrs = (total_run_time_secs / (60 * 60));
//          display_total_run_hours(hrs);
//          quadrant = 0;
//          break;
//    }
}



void tworks3_PSA_logic (void)  {

    switch (cycle)
    {
        case 0:
            //CYCLE 1
            //**************************************************************************
            do_control (SIEVE_A_VALVE_CONTROL,    OPEN_VALVE);         
            do_control (SIEVE_B_VALVE_CONTROL,    CLOSE_VALVE);
            do_control (SIEVE_FLUSH_VLV_CNTRL,    CLOSE_VALVE);
            nb_delay = Production_Delay;
            cycle++;
            break;
        case 1:
            //CYCLE 2
            //**************************************************************************
            do_control (SIEVE_A_VALVE_CONTROL,    OPEN_VALVE);         
            do_control (SIEVE_B_VALVE_CONTROL,    CLOSE_VALVE);
            do_control (SIEVE_FLUSH_VLV_CNTRL,    OPEN_VALVE);
            nb_delay = Flush_Delay;
            cycle++;
            break;
        case 2:
            //CYCLE 3
            //**************************************************************************
            do_control (SIEVE_A_VALVE_CONTROL,    OPEN_VALVE);         
            do_control (SIEVE_B_VALVE_CONTROL,    OPEN_VALVE);
            do_control (SIEVE_FLUSH_VLV_CNTRL,    OPEN_VALVE);
            nb_delay = PreCharge_Delay;
            cycle++;
            break;
        case 3:
            //CYCLE 4
            //**************************************************************************
            do_control (SIEVE_A_VALVE_CONTROL,    CLOSE_VALVE);         
            do_control (SIEVE_B_VALVE_CONTROL,    OPEN_VALVE);
            do_control (SIEVE_FLUSH_VLV_CNTRL,    CLOSE_VALVE);
            nb_delay = Production_Delay;
            cycle++;
            break;

        case 4:
            //CYCLE 5
            //**************************************************************************
            do_control (SIEVE_A_VALVE_CONTROL,    CLOSE_VALVE);         
            do_control (SIEVE_B_VALVE_CONTROL,    OPEN_VALVE);
            do_control (SIEVE_FLUSH_VLV_CNTRL,    OPEN_VALVE);
            nb_delay = Flush_Delay;
            cycle++;
            break;

        case 5:
            //CYCLE 6
            //**************************************************************************
            do_control (SIEVE_A_VALVE_CONTROL,    OPEN_VALVE);         
            do_control (SIEVE_B_VALVE_CONTROL,    OPEN_VALVE);
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


void tworks2_values_to_default_postion (void)  {


    //**************************************************************************
    do_control (SIEVE_A_VALVE_CONTROL,    CLOSE_VALVE);         
    do_control (SIEVE_B_VALVE_CONTROL,    CLOSE_VALVE); 


}

            
void tworks2_PSA_logic (void)  {

    switch (cycle)
    {
        case 0:
            //CYCLE 1
            //**************************************************************************
            do_control (SIEVE_A_VALVE_CONTROL,    OPEN_VALVE);         
            do_control (SIEVE_B_VALVE_CONTROL,    CLOSE_VALVE); 
            nb_delay = Production_Delay;
            cycle++;
            break;
      
        case 1:
            //CYCLE 3
            //**************************************************************************
            do_control (SIEVE_A_VALVE_CONTROL,    OPEN_VALVE);         
            do_control (SIEVE_B_VALVE_CONTROL,    OPEN_VALVE);
            nb_delay = PreCharge_Delay;
            cycle++;
            break;
            
        case 2:
            //CYCLE 4
            //**************************************************************************
            do_control (SIEVE_A_VALVE_CONTROL,    CLOSE_VALVE);         
            do_control (SIEVE_B_VALVE_CONTROL,    OPEN_VALVE);
            nb_delay = Production_Delay;
            cycle++;
            break;

        case 3:
            //CYCLE 6
            //**************************************************************************
            do_control (SIEVE_A_VALVE_CONTROL,    OPEN_VALVE);         
            do_control (SIEVE_B_VALVE_CONTROL,    OPEN_VALVE);
            nb_delay = PreCharge_Delay;
            cycle++;
            break;
            
        default:
            cycle = 0;
            nb_delay = 0;
            break;
    }

}
