

// tworks_o2_cons.c

//#include <UniversalTimer.h>
#include <extEEPROM.h>
#include "tempr_sensor.h"
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

extEEPROM eep(kbits_64, 1, 8);         // device size, number of devices, page size

unsigned char cycle;
unsigned char f_crn;
unsigned char f_trn;
unsigned char f_run_hours;

void o2_cons_init (void);
void o2_main_task (void);



void setup (void) {

    // Serial port initialization
    Serial.begin (115200);
    
    DBG_PRINTLN ();
    DBG_PRINTLN ("Tworks Foundation, Hyderabad, Telangana, Inida\n");
    DBG_PRINTLN ("Oxygen Concentrator Project\n");
    DBG_PRINTLN ("Harware Board Revision : " HW_REVISION_TXT);
    DBG_PRINTLN ("Firmware Version       : " FW_VERSION_TXT);
    DBG_PRINT   ("Build Time             : ");
    DBG_PRINT   (__DATE__);
    DBG_PRINT   (", ");
    DBG_PRINTLN (__TIME__);
    DBG_PRINTLN ();


	//temp
	//test_gpios ();


    platform_init ();
    ads_init ();
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
        DBG_PRINT   ("eep_record.total_run_time_secs      : ");
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

    
    o2_cons_init ();
    init_7segments ();
    blank_7segments ();
           
    ui_init ();
	
    // temp test area ---------------------
    // eeptest ();    
    // test_ads1115 ();
    // test_7segments ();
// 	while (1)
// 	{
// 		DBG_PRINTLN ("7-segments display test...");
// 		test_7segments ();
// 		DBG_PRINTLN ("7.");
// 		
// 	}

    // ------------------------------------
	
	
	

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
        update_neo_pixel_leds ();      
    }
    else if (f_1sec) {
        f_1sec = 0;
        // 1 second tasks go here..
        f_sec_logs_task = 1;
        f_sec_change_ui_task = 1;
        f_sec_change_o2_task = 1;
        f_sec_change_sensor_task = 1;
        f_sec_change_alarm_task = 1;

        o2_sensor_scan ();
        tempr_sensor_scan ();
        read_pressure ();
    
// 		if (f_system_running)	{
// 			display_o2 (o2_concentration);  
// 		}
       
        // DBG_PRINT (".");
    }
    else if (f_1min) {
        f_1min = 0;
        // 1 minute tasks go here..

        if (f_system_running) {
            save_record ();
        }
        
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
        alarms_task ();
        display_task ();
        
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
    do_control (SIEVE_A_VALVE_CONTROL,    CLOSE_VALVE);
    do_control (SIEVE_B_VALVE_CONTROL,    CLOSE_VALVE);
    do_control (SIEVE_FLUSH_VLV_CNTRL,    CLOSE_VALVE);
    new_delay_msecs (500);


}



void o2_main_task (void)    {

    static unsigned long int time_tag;
    static uint8_t           quadrant;
    static uint8_t           once_done;
    

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
        int secs = ( current_run_time_secs %  60);
        int mins = ((current_run_time_secs % (60 * 60)) / 60);
        int hrs  = ( current_run_time_secs / (60 * 60));
           
//         if ((current_run_time_secs % 15) == 0) {
//             quadrant++;
//             switch (quadrant) 
//             {
//                 case 0:
//                 case 1:
//                 case 2:
DBG_PRINTLN ("calling display_current_run_hours()..");
                  display_current_run_hours(hrs, mins);
                  f_crn = 1;
                  f_trn = 0;
		          f_run_hours = 1;
//                   break;
//                case 3:
//                   hrs = (total_run_time_secs / (60 * 60));
//                   display_total_run_hours(hrs);
//                   quadrant = 0;
//                   f_crn = 0;
//                   f_trn = 1;  
// 				  f_run_hours = 0;           
//                   break;
//             }
//         }
    }
    
    if (time_elapsed (time_tag) < nb_delay)  {
        return;
    }

    // else
    time_tag = systemtick_msecs;

    //DBG_PRINTLN ("calling PSA logic..");

    //tworks3_PSA_logic();
    tworks2_PSA_logic();


    if (nb_delay != prev_nb_delay)  {
        prev_nb_delay = nb_delay;

    }

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





// eof -----------------------
/*
 *         if (f_run_hours == 1) {
            DBG_PRINTLN("pt.1........");
            neo_pixel_control (NEO_PXL_TOTAL_RUN_TIME, ON_LED);  
            delay(300);
            DBG_PRINTLN("............");
            neo_pixel_control (NEO_PXL_CURR_RUN_TIME,  OFF_LED);             
            DBG_PRINTLN("............");
        }
        else  {
            DBG_PRINTLN("pt.2------------");
            neo_pixel_control (NEO_PXL_TOTAL_RUN_TIME, ON_LED);  
            delay(300);
            DBG_PRINTLN("----------------");
            
            neo_pixel_control (NEO_PXL_CURR_RUN_TIME,  OFF_LED);  
            DBG_PRINTLN("----------------");
        }     


        
 */
