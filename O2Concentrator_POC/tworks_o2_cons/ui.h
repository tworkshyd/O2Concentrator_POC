// ui.h

#ifndef _UI_h
#define _UI_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif


//#define ON  (true)
//#define OFF (false)


#define BUTTON_DEBOUNCE_DLY ( 55)    // in msecs


#define HIGH_BEEP           (200)
#define LOW_BEEP            ( 50)
#define SYS_ON_BEEP         (555)
#define SYS_OFF_BEEP        (111)


// Entry Check
#define CALIBRATION_MODE_ENTRY_CHECK      (5)
#define FACTORY_MODE_ENTRY_CHECK          (3)
#define CONFIG_MODE_ENTRY_CHECK           (1)



enum UI_STATES_E  {

    UI_START,         // 0
    UI_CALIB_MODE,    // 1
    UI_FACTORY_MODE,  // 2
    UI_CONFIG_MODE,   // 3

    UI_SYS_INIT,      // 4
    UI_SYS_ON_CHECK,  // 5
    UI_SYS_RUNNING,   // 6
    UI_SYS_OFF_CHECK, // 7

    UI_LAST,          // 8

};


void ui_init            (void);
void ui_task_main       (void);
void power_on_self_test (void);
void beep_for       (int msecs);



#endif
