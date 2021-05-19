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



void ui_init        (void);
void ui_task_main   (void);




#endif
