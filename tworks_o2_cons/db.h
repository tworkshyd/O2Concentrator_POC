// data_base.h

#ifndef _DATA_BASE_h
#define _DATA_BASE_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif




// external EEPROM memory address range and memory map
#define   extEEPROM_START_ADDRESS         (0x00)

#define   extEEPROM_SIZE_IN_BITS          (8142L * 8L)
#define   extEEPROM_PAGE_SIZE_IN_BYTES    (32L)
#define   extEEPROM_SIZE_IN_BYTES         (extEEPROM_SIZE_IN_BITS / 8)

#define   extEEPROM_LAST_ADDRESS          (extEEPROM_START_ADDRESS + extEEPROM_SIZE_IN_BYTES)






// time keepers
extern volatile unsigned long int systemtick_msecs;
extern volatile unsigned char     systemtick_secs;
extern volatile unsigned char     systemtick_mins;
extern volatile unsigned char     systemtick_hrs;

extern volatile bool  f_msec, f_10msec, f_100msec;
extern volatile bool  f_1sec, f_1min, f_1hr;

extern bool           f_system_running;
extern bool           f_sec_logs_task;
extern bool           f_sec_change_ui_task;
extern bool           f_sec_change_o2_task;
extern bool           f_sec_change_sensor_task;

extern float           o2_concentration;
extern float           output_pressure;
extern float           prev_o2_concentration;
extern float           prev_output_pressure;

extern unsigned int    current_run_time_secs;
extern unsigned int    prev_current_run_time_secs;
extern unsigned long   total_run_time_secs;


extern unsigned long int Production_Delay;     // delay variable creation
extern volatile unsigned long int Flush_Delay;          // delay variable creation
extern unsigned long int PreCharge_Delay;      // delay variable creation

extern unsigned long int nb_delay;
extern unsigned long int prev_nb_delay;
extern unsigned char     do_byte;              // holds all digital outputs current status

extern volatile int16_t  o2_raw_adc_count;
extern volatile float    o2_slope;
extern volatile float    o2_const_val;
extern volatile float    m_raw_voltage;

extern uint8_t   start_switch_pressed;
extern uint8_t   alarm_clear_button_pressed;

void db_init (void);


#endif
