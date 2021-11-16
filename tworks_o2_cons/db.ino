// db.c -- data base .c 

#include "db.h"


// variable definations
bool f_eeprom_working;
EEP_RECORD_T  eep_record;
EEP_RECORD_T  eep_record_default = {
    
    // 1. Run hour countes 
    0,  // unsigned int    current_run_time_secs;
    0,  // unsigned long   total_run_time_secs;  

    // todo
      //    // 2. Calibration constants
      //    float           O2_slope;
      //    float           O2_constant;
      
};


// time keepers
volatile unsigned long int    systemtick_msecs;
volatile unsigned char        systemtick_secs;
volatile unsigned char        systemtick_mins;
volatile unsigned char        systemtick_hrs;

bool volatile   f_msec, f_10msec, f_100msec;
bool volatile   f_1sec, f_1min, f_1hr;

bool            f_system_running;
bool            f_sec_logs_task;
bool            f_sec_change_ui_task;
bool            f_sec_change_o2_task;
bool            f_sec_change_sensor_task;
bool            f_sec_change_alarm_task;


// alarms
uint8_t         alarms_byte;     // can hold upto 8 alarms, 1 bit for each


float           o2_concentration = 93.50;
float           prev_o2_concentration;
float           output_pressure  = 25.03;
float           prev_output_pressure  = 25;

unsigned int    current_run_time_secs;
unsigned int    prev_current_run_time_secs;
unsigned int    last_cycle_run_time_secs;
unsigned long   total_run_time_secs;


unsigned long int Production_Delay;     // delay variable creation
volatile unsigned long int Flush_Delay; // delay variable creation
unsigned long int PreCharge_Delay;      // delay variable creation

unsigned long int nb_delay;
unsigned long int prev_nb_delay;
unsigned char     do_byte;              // holds all digital outputs current status

volatile int16_t  o2_raw_adc_count;
volatile float    o2_slope;
volatile float    o2_const_val;
volatile float    m_raw_voltage;


uint8_t   start_switch_pressed;
uint8_t   alarm_clear_button_pressed;

unsigned char   f_display_crn;
uint8_t         neo_pixel_leds_byte;   // holds led light-up info, 1 - on, 0 - off from LSB as alarm-1
uint8_t         prev_neo_pixel_leds;

void db_init (void) {

    f_system_running = 0;
    
}
