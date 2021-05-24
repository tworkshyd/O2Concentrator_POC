// db.c -- data base .c 

#include "db.h"

unsigned char   f_system_running;

unsigned int    o2_concentration = 93;
unsigned int    prev_o2_concentration;
unsigned int    output_pressure  = 25;
unsigned int    prev_output_pressure  = 25;
unsigned int    system_runtime_secs;
unsigned int    prev_system_runtime_secs;




void db_init (void) {

    f_system_running = 0;
    
}
