// db.c -- data base .c 

#include "db.h"

unsigned char   f_system_running;

unsigned char   f_sec_logs_task;
unsigned char   f_sec_change_ui_task;
unsigned char   f_sec_change_o2_task;
unsigned char   f_sec_change_sensor_task;

float           o2_concentration = 93.50;
float           prev_o2_concentration;
float           output_pressure  = 25.03;
float           prev_output_pressure  = 25;
unsigned int    system_runtime_secs;
unsigned int    prev_system_runtime_secs;




void db_init (void) {

    f_system_running = 0;
    
}
