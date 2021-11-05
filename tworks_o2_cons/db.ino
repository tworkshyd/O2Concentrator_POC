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



uint8_t         start_switch_pressed;
uint8_t         alarm_clear_button_pressed;



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


uint8_t         start_switch_pressed;
uint8_t         alarm_clear_button_pressed;


uint8_t year, month, weekday, day, hour, minute, second;
bool    period = 0;
String  m[12] = {"January", "February", "March", "April", "May", "June", "July", "August", "Septumber", "October", "November", "December"};
String  w[7]  = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};



void db_init (void) {

    f_system_running = 0;
    
}
