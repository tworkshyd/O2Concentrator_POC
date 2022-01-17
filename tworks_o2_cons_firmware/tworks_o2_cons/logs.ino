// logs.c

#include <CircularBuffer.h>

#include "db.h"
#include "logs.h"




CircularBuffer <int, 400> buffer;



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
            log_serial_dump ();
#endif
        }
    }

    // 2. event logging
#ifdef  ENABLE_EVENT_LOGS
    change_in_value = last_o2_concentration - o2_concentration;
    change_in_value = abs(change_in_value);
    if (change_in_value >= O2_VALUE_CHANGE_THRESHOLD)  {
        last_o2_concentration = o2_concentration;
        log_serial_dump ();
    }
#endif

    // 3. debug prints
    // DBG_PRINT   ("nice to ");
    // DBG_PRINTLN (" see u here..!!");

}


void log_serial_dump (void)  {

	static uint8_t		skip_count = 0;

    // 1. print legend
    // sample output : 

    // On-time, Curr-RHs, PrdDly, FlshDly, PrechrgDly, o2_raw_ADC, o2_raw_mV, o2_concen,   tempr-snsr-1, tempr-snsr-2, pressures
    // 00:00:23 00:00:22,   5400,       0,        700,    822,      666.38,     18.45,        25.65,        24.50,       25.03

	// print label once every 10 (say) records
	#define		SKIP_COUNT		(10)
	if (skip_count++ >= SKIP_COUNT)	{
		skip_count = 0;
		DBG_PRINTLN ();
		DBG_PRINT ("On-time, Curr-RHs, PrdDly, FlshDly, PrechrgDly, o2_raw_ADC, o2_raw_mV, o2_concen,   tempr-snsr-1, tempr-snsr-2, pressures\r\n" );
	}
	
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
    Serial.print (o2_m_raw_voltage, 2);
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
    //Serial.println("\r\n");

}


void logs_store     (void)	{
	
	
	
}


void logs_retrive   (void)	{
	
	
	
}




// EoF -------------

