// logs.c


#include "db.h"
#include "logs.h"
#include "tempr_sensor.h"



uint8_t      alaram_byte;    // holds one bit for each alarm


void logs_task (void) {

    static int    log_period;
    static float  last_o2_concentration;

    float         change_in_value;


    // 1. periodic logging

    if (f_sec_logs_task)  {
        f_sec_logs_task = 0;

#ifdef  ENABLE_PERIODIC_LOGS
        log_period++;
        if (log_period >= SET_LOG_PERIOD_SECS)  {
            log_period = 0;
            log_dump ();
#endif
        }
    }

    // 2. event logging
#ifdef  ENABLE_EVENT_LOGS
    change_in_value = last_o2_concentration - o2_concentration;
    change_in_value = abs(change_in_value);
    if (change_in_value >= O2_VALUE_CHANGE_THRESHOLD)  {
        last_o2_concentration = o2_concentration;
        log_dump ();
    }
#endif

    // 3. debug prints
    // DBG_PRINT   ("nice to ");
    // DBG_PRINTLN (" see u here..!!");

}


void log_dump (void)  {

    // 1. print legend
    // sample output : 

    // On-time, Curr-RHs, PrdDly, FlshDly, PrechrgDly, o2_raw_ADC, o2_raw_mV, o2_concen,   tempr-snsr-1, tempr-snsr-2, pressures
    // 00:00:23 00:00:22,   5400,       0,        700,    822,      666.38,     18.45,        25.65,        24.50,       25.03


    DBG_PRINTLN ();
    DBG_PRINT ("On-time, Curr-RHs, PrdDly, FlshDly, PrechrgDly, o2_raw_ADC, o2_raw_mV, o2_concen,   tempr-snsr-1, tempr-snsr-2, pressures\r\n" );

    // 1. time stamp
    sprintf(lcd_temp_string, "%02d:%02d:%02d ", systemtick_hrs, systemtick_mins, systemtick_secs);
    DBG_PRINT (lcd_temp_string);

    // 2. system run time
    int secs = ( current_run_time_secs %  60);
    int mins = ((current_run_time_secs % (60 * 60)) / 60);
    int hrs  = ( current_run_time_secs / (60 * 60));
    sprintf(lcd_temp_string, "%02d:%02d:%02d,  ", hrs, mins, secs);
    Serial.print (lcd_temp_string);

    // 3. production time, flush time n precharge time
    Serial.printf ("%5d,  ", Production_Delay);
    Serial.printf ("%6d,  ", Flush_Delay);
    Serial.printf ("%9d,  ", PreCharge_Delay);

    // 4. sieve A, B & back flush valve status
    //    sprintf(lcd_temp_string, "%d %d %d ", (do_byte & SIEVE_A_VALVE_CONTROL) != 0, (do_byte & SIEVE_B_VALVE_CONTROL) != 0, (do_byte & SIEVE_FLUSH_VLV_CNTRL) != 0);
    //    Serial.print (lcd_temp_string);

    // 5. O2 raw adc, mv, %
    //sprintf(lcd_temp_string, "%4d %lf %lf ", o2_raw_adc_count, m_raw_voltage, o2_concentration);
    //Serial.print (lcd_temp_string);
    Serial.printf ("%5d,      ", o2_raw_adc_count);
    Serial.print (m_raw_voltage, 2);
    Serial.print (",     ");
    Serial.print (o2_concentration, 2);
    Serial.print (",    ");

    // 6. Temperature values
    Serial.print ("    ");
    Serial.print(tempr_value_1);
    Serial.print (",        ");
    Serial.print(tempr_value_2);
    Serial.print (",       ");
     
    // 7. Pressure 
    Serial.print(output_pressure, 2);
    Serial.println("\r\n");
    Serial.println("\r\n");

}



void alarms_task (void)    {

    if (f_sec_change_alarm_task)    {
        f_sec_change_alarm_task = 0;
    }
    else {
        return;
    }

    //DBG_PRINT  ("entry alarms_byte : ");
    //DBG_PRINTLN (alarms_byte);

    // comes here once in a secound only..

    // Check for alarms, then set / clear them.
    
    // 1. Low O2 concentration alarm
    if ( f_system_running   &&  (current_run_time_secs > SYSTEM_START_UP_PERIOD) )  {
        if (o2_concentration < O2_CONCENTRATION_LOW_THRHLD)   {
            alarms_byte |= O2C_ALARM_BIT;
            //DBG_PRINT  ("pt1 alarms_byte : ");
            //DBG_PRINTLN (alarms_byte);
            neo_pixel_control (LOW_O2C_ALARM,  ON_LED);  
            DBG_PRINTLN ("Low O2 concentration Alarm..!!!");

            beep_for (100);
            DBG_PRINT (".");
        }
        else if ( o2_concentration > O2_CONCENTRATION_LOW_THRHLD ) {    // no hysteresis here..
            // clear alarm 
            alarms_byte &= ~O2C_ALARM_BIT;
            neo_pixel_control (LOW_O2C_ALARM,  OFF_LED);  
            DBG_PRINTLN ("Clearing ...... Low O2 concentration Alarm..!!!");
        }
    }
    else {
        // nop
    }

//// temp for debugging - must be removed before build
//output_pressure = 5;
    
    // 2. Low Pressure alarm
    if (f_system_running   &&  (output_pressure < PRESSURE_VALUE_LOW_THRHLD) ) {
        if ( !(alarms_byte & PRESSURE_DROP_ALARM_BIT) )   {
            alarms_byte |= PRESSURE_DROP_ALARM_BIT;        
            //DBG_PRINT  ("pt2 alarms_byte : ");
            //DBG_PRINTLN (alarms_byte);
            neo_pixel_control (LOW_PRESSURE_ALARM, ON_LED);  
            DBG_PRINTLN ("Low Pressure Alarm..!!!");
        }
        else {  // temp for debugging
            beep_for (50);
            DBG_PRINT (",");
        }
    }    
    else if ( output_pressure > PRESSURE_VALUE_LOW_THRHLD ) {    // no hysteresis here..
        // clear alarm 
        alarms_byte &= ~PRESSURE_DROP_ALARM_BIT;
        // Auto clear neo-pixel LED as well?, if so uncomment below line
        // neo_pixel_control (LOW_PRESSURE_ALARM, OFF_LED);  
    }
    else {
        // nop
    }

    //// temp for debugging - must be removed before build
    //tempr_value = 115;
    
    // 3. High Temperature alarm
    if (f_system_running   &&  (tempr_value > TEMPERATURE_HIGH_THRHLD) ) {
        if ( !(alarms_byte & TEMPR_ALARM_BIT) )   {
            alarms_byte |= TEMPR_ALARM_BIT;           
            //DBG_PRINT  ("pt3 alarms_byte : ");
            //DBG_PRINTLN (alarms_byte);
            neo_pixel_control (HIGH_TEMPER_ALARM, ON_LED);
            DBG_PRINTLN ("High temperature Alarm..!!!");
            // SHUT - DOWN the system
                // todo
        }
        else {  // temp for debugging
            beep_for (150);
            DBG_PRINT (";");
        }
    }
    else if ( tempr_value < TEMPERATURE_HIGH_THRHLD ) {    // no hysteresis here..
        // clear alarm 
        alarms_byte &= ~TEMPR_ALARM_BIT;
        // Auto clear neo-pixel LED as well?, if so uncomment below line
        // neo_pixel_control (HIGH_TEMPER_ALARM, OFF_LED);  
    }
    else {
        // nop
    }
    //DBG_PRINT  ("exit alarms_byte : ");
    //DBG_PRINTLN (alarms_byte);

}
