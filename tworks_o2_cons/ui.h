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


enum UI_STATES_E  {

    UI_START,
    UI_CONFIG_MODE,
    UI_FACTORY_MODE,
    UI_CALIB_MODE,

    UI_SYS_ON_CHECK,
    UI_SYS_OFF_CHECK,
    
    UI_LAST,
    
};


void ui_init            (void);
void ui_task_main       (void);
void power_on_self_test (void);



#endif
