// logs.h

#ifndef		alarms_h
	#define alarms_h


#include "config.h"



#define O2C_ALARM_BIT           (0x01)
#define TEMPR_ALARM_BIT         (0x02)
#define PRESSURE_DROP_ALARM_BIT (0x04)

// critical Alarms will switch off the system, please make a selection by ORing the appropriate ones in below define
#define CRITICAL_ALARMS         (TEMPR_ALARM_BIT | PRESSURE_DROP_ALARM_BIT)

#define SYSTEM_START_UP_PERIOD							(120)       // delay in seconds, before actually checking for O2 Concentration after power-on
#define TIME_DELAY_BEFORE_LOW_O2_ALARM_ASSERTION		(55)        // delay in seconds,
#define TIME_DELAY_BEFORE_HIGH_TEMPR_ALARM_ASSERTION	(15)        // delay in seconds,
#define TIME_DELAY_BEFORE_LOW_PRESSRUE_ALARM_ASSERTION  (25)        // delay in seconds,







extern uint8_t      alarms_byte;            // holds one bit for each alarm
extern bool         f_critical_alarms;
extern bool         f_start_alarm_beeps;



void alarms_task    (void);









#endif  // '#ifndef logs_h' ends here..
