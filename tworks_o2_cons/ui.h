// ui.h

#ifndef _UI_h
#define _UI_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif


#define ON  (true)
#define OFF (false)

#define HIGH_BEEP (150)
#define LOW_BEEP  ( 50)



void ui_init        (void);
void ui_task_main   (void);
void beep_for       (int msecs);



#endif
