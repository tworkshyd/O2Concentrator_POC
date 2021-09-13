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




void logs_task (void);










#endif  // '#ifndef logs_h' ends here..
