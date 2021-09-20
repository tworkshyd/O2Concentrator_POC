// config.h

#ifndef config_h
#define config_h




// 1. Periodic logs
#define ENABLE_PERIODIC_LOGS   
#define SET_LOG_PERIOD_SECS         (2)

// 2. Event logs..
//#define ENABLE_EVENT_LOGS    
#define O2_VALUE_CHANGE_THRESHOLD   (0.3)    

// 3. Debug logs
#define ENABLE_DEBUG_PRINTS

// Select by uncommeting the required events from below list
//  Note: these uncommented events print only if 'ENABLE_EVENT_LOGS' is enabled 
#define CHANGE_IN_O2_LEVEL    
#define CHANGE_IN_PRESSURE    
#define CHANGE_IN_TEMPERATURE    


// void display_current_run_time_1  (uint16_t hours, uint16_t mins);	// with decimal point after hours
// void display_current_run_time_2  (uint16_t hours, uint16_t mins);	// without decimal point and one digit gap between hours & minutes
// void display_current_run_time_3  (uint16_t hours, uint16_t mins);	// with colon between run hours and minutes by flipping digit2 (from right) of row2

//#define display_current_run_time	display_current_run_time_1
#define display_current_run_time	display_current_run_time_2
//#define display_current_run_time	display_current_run_time_3












#endif  // '#ifndef config_h' ends here..
