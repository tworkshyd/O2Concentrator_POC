// data_base.h

#ifndef _DATA_BASE_h
#define _DATA_BASE_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif


extern unsigned char   f_system_running;

extern unsigned int    o2_concentration;
extern unsigned int    output_pressure;
extern unsigned int    system_runtime_secs;

void db_init (void);


#endif
