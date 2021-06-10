// logs.c





void logs_task (void) {

    static int  log_period;

    // 1. periodic logging
    log_period++;
    if (log_period >= SET_LOG_PERIOD_SECS)  {
        log_period = 0;
        PLOG_PRINT   ("iam ");
        PLOG_PRINTLN (" in..!!");

    }


    // 2. event logging
    int secs = ( system_runtime_secs %  60);
    int mins = ((system_runtime_secs % (60 * 60)) / 60);
    int hrs  = ( system_runtime_secs / (60 * 60));
    sprintf(lcd_temp_string, "RUN TIME  %02d:%02d:%02d", hrs, mins, secs);
    EVNTLOG_PRINTLN(lcd_temp_string);


    // 3. debug prints
    // DBG_PRINT   ("nice to ");
    // DBG_PRINTLN (" see u here..!!");



}


void log_dump (void)  {

  
}
