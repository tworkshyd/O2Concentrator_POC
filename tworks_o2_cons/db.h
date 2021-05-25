// data_base.h

#ifndef _DATA_BASE_h
#define _DATA_BASE_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif


extern unsigned char   f_system_running;
extern unsigned char   f_sec_change_ui_task;
extern unsigned char   f_sec_change_o2_task;
extern unsigned char   f_sec_change_sensor_task;

extern float           o2_concentration;
extern float           output_pressure;
extern float           prev_o2_concentration;
extern float           prev_output_pressure;
extern unsigned int    system_runtime_secs;

void db_init (void);


#endif
