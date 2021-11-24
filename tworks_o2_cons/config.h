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
#define O2C_CAP_VALUE_MAX           (95.0)

// 5.
// Select by uncommeting the required events from below list
//  Note: these uncommented events print only if 'ENABLE_EVENT_LOGS' is enabled 
#define CHANGE_IN_O2_LEVEL    
#define CHANGE_IN_PRESSURE    
#define CHANGE_IN_TEMPERATURE    




// 6. Select appropriate H/W board revision  
/******************************************************************************/
    //  Rev 1.0 - having 3 boards(Display board, sensor board and control board), with control board MCU Atmega1284, dated 21/05/2021
#define HW_REV_1_0          (10)        // for rev 1.0
#define HW_REV_1_0_TXT      "Rev 1.0"

/******************************************************************************/
    
// Use appropriate selection below .. as per the board used..
#define HW_REVISION          HW_REV_1_0
#define HW_REVISION_TXT      HW_REV_1_0_TXT






// 7. Select appropriate version for .hex file generation
/******************************************************************************/
// Use appropriate selection below .. to generate .hex files .. as per the need..
// Requirement..
    //  M1 - v1.2 - no cap, 1st and 3rd relay reversal  --- O2 Sensor S1
    //  M2 - v1.0 - with cap                            --- O2 Sensor S3
    //  M3 - v1.1 - with cap                            --- O2 Sensor S2

#define FW_VERSION_1_0          (10)            // for v1.0
#define FW_VERSION_1_0_TXT      "Ver 1.0"       // for v1.0
#define FW_VERSION_1_1          (11)            // for v1.1
#define FW_VERSION_1_1_TXT      "Ver 1.1"       // for v1.1
#define FW_VERSION_1_2          (12)            // for v1.2
#define FW_VERSION_1_2_TXT      "Ver 1.2"       // for v1.2


    
/******************************************************************************/
#define     FW_VERSION          FW_VERSION_1_0
#define     FW_VERSION_TXT      FW_VERSION_1_0_TXT
/******************************************************************************/


//#if (FW_VERSION == FW_VERSION_1_0)
//    #define CAPP_AT_95_O2                   (1)   // O2c Capping enable / Disable selection
//    #define O2_SENSOR                       (3)   // O2 sensor selection among 3 
//    
//#elif (FW_VERSION == FW_VERSION_1_1)
//    #define CAPP_AT_95_O2                   (1)   // O2c Capping enable / Disable selection
//    #define O2_SENSOR                       (2)   // O2 sensor selection among 3 
//
//#elif (FW_VERSION == FW_VERSION_1_2)
//    #define CAPP_AT_95_O2                   (0)   // O2c Capping enable / Disable selection
//    #define O2_SENSOR                       (1)   // O2 sensor selection among 3 
//    #define ENABLE_USE_OF_RELAY_3_FOR_Z1    (1)   // enabling this results in '1st and 3rd relay reversal'
//
//#else
//    #define CAPP_AT_95_O2                   (1)   // O2c Capping enable / Disable selection
//    #define O2_SENSOR                       (3)   // O2 sensor selection among 3 
//
//#endif


// Select capping enable / disable
#define CAPP_AT_95_O2                   (1)   // O2c Capping enable / Disable selection


// Select O2 sensor (hard-coding calibration values)
#define O2_SENSOR                       (3)   // O2 sensor selection among 3 











#endif  // '#ifndef config_h' ends here..
