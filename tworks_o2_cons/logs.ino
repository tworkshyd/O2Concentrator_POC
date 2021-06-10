// logs.c





void logs_task (void) {

    static int  log_period;



    // 1. periodic logging

    if (f_sec_logs_task)  {
        f_sec_logs_task = 0;
        log_period++;
        if (log_period >= SET_LOG_PERIOD_SECS)  {
            log_period = 0;
            PLOG_PRINT   ("iam ");
            PLOG_PRINTLN (" in..!!");
        }

        
    }

    // 2. event logging

    // 3. debug prints
    // DBG_PRINT   ("nice to ");
    // DBG_PRINTLN (" see u here..!!");



}


void log_dump (void)  {

    int secs = ( production_time_secs %  60);
    int mins = ((production_time_secs % (60 * 60)) / 60);
    int hrs  = ( production_time_secs / (60 * 60));
    sprintf(lcd_temp_string, "RUN TIME  %02d:%02d:%02d", hrs, mins, secs);
    EVNTLOG_PRINTLN(lcd_temp_string);
}
