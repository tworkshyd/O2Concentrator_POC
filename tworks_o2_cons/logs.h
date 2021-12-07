// logs.h

#ifndef logs_h
#define logs_h


#include "config.h"

// 1. Periodic logs
#ifdef  ENABLE_PERIODIC_LOGS
#define PLOG_PRINTLN     Serial.println
#define PLOG_PRINT       Serial.print
#else
#define PLOG_PRINTLN     // Serial.println
#define PLOG_PRINT       // Serial.print
#endif

// 2. Event logs
#ifdef  ENABLE_EVENT_LOGS
#define EVNTLOG_PRINTLN Serial.println
#define EVNTLOG_PRINT   Serial.print
#else
#define EVNTLOG_PRINTLN //  Serial.println
#define EVNTLOG_PRINT   //  Serial.print
#endif


// 3. Debug logs
#ifdef  ENABLE_DEBUG_PRINTS
#define DBG_PRINTLN     Serial.println
#define DBG_PRINT       Serial.print
#else
#define DBG_PRINTLN     //  Serial.println
#define DBG_PRINT       //  Serial.print
#endif



    
#define O2C_ALARM_BIT           (0x01)
#define TEMPR_ALARM_BIT         (0x02)
#define PRESSURE_DROP_ALARM_BIT (0x04)

#define CRITICAL_ALARMS         (TEMPR_ALARM_BIT | PRESSURE_DROP_ALARM_BIT)

#define SYSTEM_START_UP_PERIOD              (120)       // delay in seconds, before actually checking for O2 Concentration after power-on
#define TIME_DELAY_BEFORE_ALARM_ASSERTION   (25)        // delay in seconds,



extern uint8_t      alarms_byte;            // holds one bit for each alarm
extern bool         f_critical_alarms;
extern bool         f_start_alarm_beeps;





void logs_task      (void);
void alarms_task    (void);









#endif  // '#ifndef logs_h' ends here..
