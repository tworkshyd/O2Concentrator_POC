// config.h

#ifndef config_h
#define config_h


/* A. Hardware Boards --------------------------------------------------------
 	Revision log -------------------------------------------------------------
 ---------------------------------------------------------------------------*/
//  Rev 1.0 - having 3 boards(Display board, sensor board and control board), 
//			with control board MCU Atmega1284, dated 21/05/2021
#define HW_REV_1_0          (10)        // for rev 1.0
#define HW_REV_1_0_TXT      "Rev 1.0"

//  Rev 2.0 - having 2 boards(Display board, and control board, with control 
//			board MCU Atmega1284, dated 25/10/2021
#define HW_REV_2_0          (20)        // for rev 2.0
#define HW_REV_2_0_TXT      "Rev 2.0"
// ---------------------------------------------------------------------------

/* ---------------------------------------------------------------------------
 * Select appropriate H/W board revision, as per the board used..
 * ------------------------------------------------------------------------ */
#define		HW_REVISION          HW_REV_1_0
#define		HW_REVISION_TXT      HW_REV_1_0_TXT
// ---------------------------------------------------------------------------




/* B. Firmware version management --------------------------------------------
 	versions log -------------------------------------------------------------
 ---------------------------------------------------------------------------*/
//  v1.0 - Rev1.0 Boards + without alarms and hard coded calibration values
//  v1.1 - Rev1.0 Boards + with alarms and hard coded calibration values
#define FW_VERSION_1_0          (10)            // for v1.0
#define FW_VERSION_1_0_TXT      "Ver 1.0"       
#define FW_VERSION_1_1          (11)            // for v1.1
#define FW_VERSION_1_1_TXT      "Ver 1.1"      
// ---------------------------------------------------------------------------

/* ---------------------------------------------------------------------------
 * Select appropriate F/W version.
 * ------------------------------------------------------------------------ */
#define     FW_VERSION          FW_VERSION_1_1
#define     FW_VERSION_TXT      FW_VERSION_1_1_TXT
// ---------------------------------------------------------------------------




// ===========================================================================
// ============================= User settable ===============================
// ===========================================================================


/* ===========================================================================
 * 1. Enable / disable O2 concentration value capping
 * ======================================================================== */
// i. Uncomment below line to enable O2c Capping
#define CAPP_AT_95_O2        (1)   
// ii. Choose capping value
#define O2C_CAP_VALUE_MAX    (95.0)
// ---------------------------------------------------------------------------


/* ===========================================================================
 * 2. Select Alarms trigger thresholds and delay before assertion
 * ======================================================================== */
// i. O2 Concentration
#define O2_CONCENTRATION_LOW_THRHLD         (82.0)		// in '%'

// ii. Pressure value
#define PRESSURE_VALUE_LOW_THRHLD           (6.0)		// in psi units

// iii. Temperature value
#define TEMPERATURE_HIGH_THRHLD             (100.0)		// in Centigrade units
// ---------------------------------------------------------------------------


/* ===========================================================================
 * 3. Alarm LEDs position selection (Alarms to LEDs mapping)
 * ======================================================================== */
#define     UNUSED_ALARM_1          NEO_PXL_ALARM_1
#define     LOW_O2C_ALARM           NEO_PXL_ALARM_2
#define     LOW_PRESSURE_ALARM      NEO_PXL_ALARM_3
#define     HIGH_TEMPER_ALARM       NEO_PXL_ALARM_4
#define     UNUSED_ALARM_2          NEO_PXL_ALARM_5
// ---------------------------------------------------------------------------



/* ===========================================================================
 * 4. Logging parameters selection (on serial port)
 * ======================================================================== */
// i. Periodic logs
#define ENABLE_PERIODIC_LOGS
#define SET_LOG_PERIOD_SECS         (2)

// ii. Select by uncommenting the required events from below list
//  Note: these uncommented events print only if 'ENABLE_EVENT_LOGS' is enabled
#define CHANGE_IN_O2_LEVEL
#define CHANGE_IN_PRESSURE
#define CHANGE_IN_TEMPERATURE

// iii. Event logs..
//#define ENABLE_EVENT_LOGS
#define O2_VALUE_CHANGE_THRESHOLD   (0.3)

// iv. Debug prints enable / disable
#define ENABLE_DEBUG_PRINTS

// ---------------------------------------------------------------------------




#endif  // '#ifndef config_h' ends here..





