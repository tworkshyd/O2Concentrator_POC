// logs.c


#include "logs.h"


void logs_task (void) {

    static int  log_period;



    // 1. periodic logging

    if (f_sec_logs_task)  {
        f_sec_logs_task = 0;
        log_period++;
        if (log_period >= SET_LOG_PERIOD_SECS)  {
            log_period = 0;
#ifdef  ENABLE_PERIODIC_LOGS
            log_dump ();
#endif
        }
    }

    // 2. event logging
    // todo

    // 3. debug prints
    // DBG_PRINT   ("nice to ");
    // DBG_PRINTLN (" see u here..!!");



}


void log_dump (void)  {

    // 1. time stamp
    sprintf(lcd_temp_string, "%02d:%02d:%02d ", systemtick_hrs, systemtick_mins, systemtick_secs);
    Serial.print (lcd_temp_string);

    // 2. system run time
    int secs = ( production_time_secs %  60);
    int mins = ((production_time_secs % (60 * 60)) / 60);
    int hrs  = ( production_time_secs / (60 * 60));
    sprintf(lcd_temp_string, "%02d:%02d:%02d ", hrs, mins, secs);
    Serial.print (lcd_temp_string);

    // 3. production time, flush time n precharge time
    Serial.print (Production_Delay);
    Serial.print (" ");
    Serial.print (Flush_Delay);
    Serial.print (" ");
    Serial.print (PreCharge_Delay);
    Serial.print (" ");

    // 4. sieve A, B & back fluxh vavle status
    sprintf(lcd_temp_string, "%d %d %d ", (do_byte & SIEVE_A_VALVE_CONTROL) != 0, (do_byte & SIEVE_B_VALVE_CONTROL) != 0, (do_byte & SIEVE_FLUSH_VLV_CNTRL) != 0);
    Serial.print (lcd_temp_string);

   // 5. O2 raw adc, mv, %
    Serial.print(o2_raw_adc_count);
    Serial.print (" ");
    Serial.print(m_raw_voltage, 2);
    Serial.print (" ");
    Serial.print(o2_concentration, 2);
    Serial.print (" ");

   // 6. O2 cons ACD, room temp, RH
    sprintf(lcd_temp_string, "%d %d %d ", 1, 2, 3);
    Serial.println (lcd_temp_string);


}
