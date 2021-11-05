

// tworks_o2_cons.c

//#include <UniversalTimer.h>

//#include <extEEPROM.h>
//#include "RTCDS1307.h"


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


//extEEPROM eep(kbits_64, 1, 8);        // device size, number of devices, page size
//RTCDS1307 rtc (0x68);                 // address of RTC DS1307


unsigned char cycle;
unsigned char f_crn;
unsigned char f_trn;
unsigned char f_run_hours;


void o2_cons_init (void);
void o2_main_task (void);


void setup (void) {

    Serial.begin (115200);
    DBG_PRINTLN ("Serial port initialized..!!");
    platform_init ();
    db_init ();


    if (eeprom_init () == true) {
        f_eeprom_working = 1;
        // read eeprom and retrieve saved counters and system parameters
        eepread (EEPROM_RECORD_START, (byte*)&eep_record, EEPROM_RECORD_AREA_SIZE);

        // print retrieved record.. 
        DBG_PRINTLN ();
        DBG_PRINTLN ("EEprom retrieved record...");
        DBG_PRINT   ("eep_record.last_cycle_run_time_secs : ");
        DBG_PRINTLN (eep_record.last_cycle_run_time_secs);
        DBG_PRINT   ("eep_record.total_run_time_secs   : ");
        DBG_PRINTLN (eep_record.total_run_time_secs);

        // update system variables
        last_cycle_run_time_secs = eep_record.last_cycle_run_time_secs;
        total_run_time_secs      = eep_record.total_run_time_secs;
    }
    else {
        f_eeprom_working = 0;
      
        // load default parameters..
        byte *buff1, *buff2;

        buff1 = (byte*)&eep_record;
        buff2 = (byte*)&eep_record_default;
        for (int i = 0; i < EEPROM_RECORD_AREA_SIZE; i++)
        {
            buff1[i] = buff2[i];
        }

        DBG_PRINTLN ();
        DBG_PRINTLN ("EEprom not working...");
        DBG_PRINTLN ("Loaded factory defaults .. ");

    }



    //temp
    // test_ads1115 ();

//    // RTC DS1307 initialization
//    rtc.begin();
//    rtc.setDate(21, 9, 20);
//    rtc.setTime(21, 35, 55);
//
//    uint8_t eepStatus = eep.begin(eep.twiClock400kHz);   //go fast!
//    if (eepStatus) {
//      Serial.print(F("extEEPROM.begin() failed, status = "));
//      Serial.println(eepStatus);
//      while (1);
//    }

    // temp
    DBG_PRINTLN ("SIEVE_A_VALVE_CONTROL.!!");
    do_control (SIEVE_A_VALVE_CONTROL,    OPEN_VALVE);     
    delay (1000);    
    do_control (SIEVE_A_VALVE_CONTROL,    CLOSE_VALVE);     
    delay (1000);    
    DBG_PRINTLN ("SIEVE B VALVE_CONTROL.!!");
    do_control (SIEVE_B_VALVE_CONTROL,    OPEN_VALVE);
    delay (1000);    
   do_control (SIEVE_B_VALVE_CONTROL,    CLOSE_VALVE);
    delay (1000);    

    DBG_PRINTLN ("SIEVE_FLUSH_VLV_CNTRL..");
    do_control (SIEVE_FLUSH_VLV_CNTRL,    OPEN_VALVE);
    delay (1000);    
    do_control (SIEVE_FLUSH_VLV_CNTRL,    CLOSE_VALVE);
    delay (1000);    
    
//    eeprom_init ();
    // temp
    // eeptest ();
    //while (1)
    //{
    //  rtc_test ();
    //}
    
    o2_cons_init ();
    init_7segments ();

	  blank_7segments ();
       

    // temp
    test_7segments ();
    //test_neo_pixcell_leds ();

    
    display_o2 (00.0);
    display_total_run_hours (total_run_time_secs);    

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

    		if (f_system_running)	{
    			display_o2 (o2_concentration);  
    		}



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

    // 2. Continuous tasks are called here
    else {
        o2_main_task ();
        ui_task_main ();
        logs_task ();



  		if (f_system_running)	{
  			display_o2 (o2_concentration);
  			if (f_run_hours == 1) {
  				int secs = ( current_run_time_secs %  60);
  				int mins = ((current_run_time_secs % (60 * 60)) / 60);
  				int hrs  = ( current_run_time_secs / (60 * 60));
  				display_current_run_hours(hrs, mins);
  			}
  			else  {
  				int hrs = (total_run_time_secs / (60 * 60));
  				display_total_run_hours(hrs);
  			}
  		}
    }

	  init_7segments ();


}



void o2_cons_init (void)    {

    // compute slope and constant values
    sensor_zero_calibration ();


    // temp
//    DBG_PRINTLN ("Warning..: Hard coding slope and constant for Fio2");
//    o2_slope = 0.1734;
//    o2_const_val = 0.9393;
    
    DBG_PRINTLN ("");
    DBG_PRINT   ("o2_slope : ");
    DBG_PRINT   (o2_slope);
    DBG_PRINT   (", o2_const_val : ");
    DBG_PRINT   (o2_const_val);    


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

    // start with TRN display hence light-up TRN LED
    neo_pixcel_data (ALL_LEDs_OFF, 0);
    neo_pixcel_data (TRN_DISPLAY, 1); 

}



void o2_main_task (void)    {

    static unsigned long int time_tag;

    if (f_system_running != true) {
        return;
    }
    if (f_sec_change_o2_task) {
        f_sec_change_o2_task = 0;

        production_time_secs++;

        current_run_time_secs++;
        total_run_time_secs++;


      // display run hours, 45 seconds current run hours, 15 seconds total runhours
      int secs = ( current_run_time_secs %  60);
      int mins = ((current_run_time_secs % (60 * 60)) / 60);
      int hrs  = ( current_run_time_secs / (60 * 60));
         
      if ((current_run_time_secs % 15) == 0) {
        quadrant++;
        switch (quadrant) 
        {
            case 0:
            case 1:
            case 2:
              display_run_time(hrs, mins);
              neo_pixcel_data (ALL_LEDs_OFF, 0);
              delay(333);              
              neo_pixcel_data (CRN_DISPLAY, 1);              
              break;
           case 3:
              hrs = (total_run_time_secs / (60 * 60));
              display_run_hours(hrs);
              neo_pixcel_data (ALL_LEDs_OFF, 0);
              delay(333);
              neo_pixcel_data (TRN_DISPLAY,  1); 
              quadrant = 0;
              break;
        }
      }


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
