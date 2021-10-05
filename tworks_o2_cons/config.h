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

// 4. O2C cap-value setting
#define O2C_CAP_VALUE_MAX   (95.0)

// 5.
// Select by uncommeting the required events from below list
//  Note: these uncommented events print only if 'ENABLE_EVENT_LOGS' is enabled 
#define CHANGE_IN_O2_LEVEL    
#define CHANGE_IN_PRESSURE    
#define CHANGE_IN_TEMPERATURE    


// 6. O2c Capping enable / Disable selection
#define CAPP_AT_95_O2 (1)

// 7. Relay selection for Z1 seive
#define ENABLE_USE_OF_RELAY_3_FOR_Z1    (1)   // enabling this results in '1st and 3rd relay reversal'


// Use appropriate selection above .. to generate .hex files .. as per the need..
//M3 - v1.1 - no cap                                --- O2 Sensor S3
//M2 - v1.0 - with cap                              --- O2 Sensor S2
//M1 - v1.2 - with cap, 1st and 3rd relay reversal  --- O2 Sensor S1













#endif  // '#ifndef config_h' ends here..
