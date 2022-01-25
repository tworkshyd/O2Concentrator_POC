// data_base.h

#ifndef _DATA_BASE_h
#define _DATA_BASE_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif





// Structure declarations
typedef struct eep_record_t {

    // 1. Run hour counters 
    unsigned int    last_cycle_run_time_secs;
    unsigned long   total_run_time_secs;  

    // 2. Calibration constants
    float           O2_slope;
    float           O2_constant;
    float           pressure_slope;
    float           pressure_constant;
    float           tempr1_slope;
    float           tempr1_constant;
    float           tempr2_slope;
    float           tempr2_constant;
    
} EEP_RECORD_T;




// variable declarations
extern bool          f_eeprom_working;
extern EEP_RECORD_T  eep_record;
extern EEP_RECORD_T  eep_record_default;




// time keepers
extern volatile unsigned long int systemtick_msecs;
extern volatile unsigned char     systemtick_secs;
extern volatile unsigned char     systemtick_mins;
extern volatile unsigned char     systemtick_hrs;

extern volatile bool  f_msec, f_10msec, f_100msec;
extern volatile bool  f_1sec, f_1min, f_1hr;

extern bool           f_system_running;
extern bool           f_sec_disp_task;
extern bool           f_sec_logs_task;
extern bool           f_sec_change_ui_task;
extern bool           f_sec_change_o2_task;
extern bool           f_sec_change_sensor_task;
extern bool           f_sec_change_alarm_task;

// alarms
extern uint8_t			alarms_byte;     // can hold upto 8 alarms, 1 bit for each

extern float             o2_concentration;
extern float			 o2_moving_avg;
extern float             output_pressure;
extern float			 prev_o2_concentration;
extern float			 prev_output_pressure;
extern volatile int16_t  pressure_raw_adc_count;
extern volatile float    pressureZero;
extern volatile float    pressureMax;
extern volatile float    pressuretransducermaxPSI;
extern volatile float    pressure_raw_m_voltage;



extern unsigned int    current_run_time_secs;
extern unsigned int    prev_current_run_time_secs;
extern unsigned int    last_cycle_run_time_secs;
extern unsigned long   total_run_time_secs;


extern unsigned long int Production_Delay;				// delay variable creation
extern volatile unsigned long int Flush_Delay;          // delay variable creation
extern unsigned long int PreCharge_Delay;				// delay variable creation

extern unsigned long int nb_delay;
extern unsigned long int prev_nb_delay;
extern unsigned char     do_byte;						// holds all digital outputs current status

extern volatile int16_t  o2_raw_adc_count;
extern volatile float    o2_slope;
extern volatile float    o2_const_val;
extern volatile float    o2_m_raw_voltage;

extern uint8_t   start_switch_pressed;
extern uint8_t   alarm_clear_button_pressed;

extern unsigned char    f_run_hours;
extern unsigned char    f_display_crn;
extern uint8_t          neo_pixel_leds_byte;			// holds led light-up info, 1 - on, 0 - off from LSB as alarm-1
extern uint8_t          prev_neo_pixel_leds;


// Function declarations
void db_init (void);


#endif
