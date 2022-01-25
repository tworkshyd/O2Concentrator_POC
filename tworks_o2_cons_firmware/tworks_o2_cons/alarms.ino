// logs.c

#include <CircularBuffer.h>

#include "db.h"
#include "alarms.h"
#include "tempr_sensor.h"



uint8_t      alaram_byte;    // holds one bit for each alarm
bool         f_critical_alarms;
bool         f_start_alarm_beeps;








void alarms_task (void)    {

    static uint8_t  low_fio2_alarm_dly;
    static uint8_t  low_pressure_alarm_dly;
    static uint8_t  high_temperature_alarm_dly;

    if (f_sec_change_alarm_task)    {
        f_sec_change_alarm_task = 0;
    }
    else {
        return;
    }


    // comes here once in a second only..
	if (alarm_clear_button_pressed)	{
		alarm_clear_button_pressed = false;
		
		low_fio2_alarm_dly = 0;
		low_pressure_alarm_dly = 0;
		high_temperature_alarm_dly = 0;
		
		alarms_byte = 0;
		f_start_alarm_beeps = 0;
		neo_pixel_control (LOW_O2C_ALARM, OFF_LED);
		neo_pixel_control (LOW_PRESSURE_ALARM, OFF_LED);
		neo_pixel_control (HIGH_TEMPER_ALARM, OFF_LED);
		DBG_PRINTLN ();
		DBG_PRINT   (__FILE__);
		DBG_PRINTLN (" : Alarm Clear Button Pressed..");
	}

	if (alarms_byte != 0)	{
		// some alarms present .. pls print them..
		DBG_PRINTLN ();
		if (alarms_byte & O2C_ALARM_BIT)	{
			DBG_PRINTLN ("Alarm : LOW O2 concentration ..!!");	
		}
		if (alarms_byte & TEMPR_ALARM_BIT)	{
			DBG_PRINTLN ("Alarm : High Temperature ..!!");
		}
		if (alarms_byte & PRESSURE_DROP_ALARM_BIT)	{
			DBG_PRINTLN ("Alarm : LOW Pressure ..!!");
		}			
	}


    // Check for alarms, then set / clear them.
    
    // 1. Low O2 concentration alarm
    if ( f_system_running   &&  (current_run_time_secs > SYSTEM_START_UP_PERIOD) )  {
        if (o2_concentration < O2_CONCENTRATION_LOW_THRHLD)   {
            
            if (low_fio2_alarm_dly < TIME_DELAY_BEFORE_LOW_O2_ALARM_ASSERTION) {
                low_fio2_alarm_dly++;
				DBG_PRINTLN ();
				DBG_PRINT ("!!!!!!! Warning : Low O2 concentration Alarm.. ticking  ");
				DBG_PRINTLN (TIME_DELAY_BEFORE_LOW_O2_ALARM_ASSERTION - low_fio2_alarm_dly);
            }
            else {
                alarms_byte |= O2C_ALARM_BIT;
                neo_pixel_control (LOW_O2C_ALARM,  ON_LED);
                f_start_alarm_beeps = 1;
				DBG_PRINTLN ();
                DBG_PRINTLN ("Low O2 concentration Alarm Triggered..!!!");
            }
        }
        else if ( o2_concentration > O2_CONCENTRATION_LOW_THRHLD ) {    // no hysteresis here..
            // clear alarm 
            if (low_fio2_alarm_dly) {
                low_fio2_alarm_dly--;
				DBG_PRINTLN ();
				DBG_PRINT   ("Alarm Clearing : Low O2 concentration Alarm.. ticking ... ");
				DBG_PRINTLN (low_fio2_alarm_dly);
            }
            else {
                alarms_byte &= ~O2C_ALARM_BIT;
                neo_pixel_control (LOW_O2C_ALARM,  OFF_LED);  
            }
        }
    }
    else {
        // nop
    }


    // 2. Low Pressure alarm
    if ( f_system_running )  {
        if ( (output_pressure < (PRESSURE_VALUE_LOW_THRHLD - 0.5)) )   {
            if (low_pressure_alarm_dly < TIME_DELAY_BEFORE_LOW_PRESSRUE_ALARM_ASSERTION) {
                low_pressure_alarm_dly++;
				DBG_PRINTLN ();
				DBG_PRINT   ("!!!!!!! Warning : Low Pressure Alarm.. ticking  ");
				DBG_PRINTLN (TIME_DELAY_BEFORE_LOW_PRESSRUE_ALARM_ASSERTION - low_pressure_alarm_dly);
            }
            else {
                alarms_byte |= PRESSURE_DROP_ALARM_BIT;        
                neo_pixel_control (LOW_PRESSURE_ALARM, ON_LED);  
                f_start_alarm_beeps = 1;
				DBG_PRINTLN ();
                DBG_PRINTLN ("Low Pressure Alarm.. Triggered !!!");			
            }
        }
        
		else if ( output_pressure > PRESSURE_VALUE_LOW_THRHLD ) {    // no hysteresis here..
			// clear alarm 
			if (low_pressure_alarm_dly) {
				low_pressure_alarm_dly--;
				DBG_PRINTLN ();
				DBG_PRINT   ("Alarm Clearing : Low Pressure Alarm.. ticking ...");
				DBG_PRINTLN (low_pressure_alarm_dly);
			}
			else {
				alarms_byte &= ~PRESSURE_DROP_ALARM_BIT;
				// Auto clear neo-pixel LED as well?, if so uncomment below line
				neo_pixel_control (LOW_PRESSURE_ALARM, OFF_LED);  
			}
		}
	}
    else {
        // nop
    }

    
    // 3. High Temperature alarm
	// Note: Practically observed that if temperature sensor is not connected the temperature value is reaching 150.0 constant
	//		Hence taking this as a condition for checking temperature sensor absence and discarding its value if condition check
	float temp_tempr_value_1 = tempr_value_1;
	float temp_tempr_value_2 = tempr_value_2;
	if (tempr_value_1 >= INVALID_TEMPERATURE_VALUE)	{
		temp_tempr_value_1 = TEMPERATURE_HIGH_THRHLD - 1.0;		// just one deg. less to avoid alarm
	}
	if (tempr_value_2 >= INVALID_TEMPERATURE_VALUE)	{
		temp_tempr_value_2 = TEMPERATURE_HIGH_THRHLD - 1.0;		// just one deg. less to avoid alarm
	}
	
    if ( f_system_running )  {
        if ( (temp_tempr_value_1 > TEMPERATURE_HIGH_THRHLD)	 ||  (temp_tempr_value_2 > TEMPERATURE_HIGH_THRHLD) )	{
            if (high_temperature_alarm_dly < TIME_DELAY_BEFORE_HIGH_TEMPR_ALARM_ASSERTION)  {
                high_temperature_alarm_dly++;
				DBG_PRINTLN ();
				DBG_PRINT   ("!!!!!!! Warning : High Temperature Alarm.. ticking  ");
				DBG_PRINTLN (TIME_DELAY_BEFORE_HIGH_TEMPR_ALARM_ASSERTION - high_temperature_alarm_dly);
            }
            else {
                alarms_byte |= TEMPR_ALARM_BIT;           
                neo_pixel_control (HIGH_TEMPER_ALARM, ON_LED);
                f_start_alarm_beeps = 1;
				DBG_PRINTLN ();
                DBG_PRINTLN ("High temperature Alarm Triggered..!!!\n");
                // SHUT - DOWN the system
                    // todo
            }
        }
    
		// else if ( tempr_value_1 < TEMPERATURE_HIGH_THRHLD ) {    // no hysteresis here..
		else if ( temp_tempr_value_1 < TEMPERATURE_HIGH_THRHLD  &&  temp_tempr_value_2 < TEMPERATURE_HIGH_THRHLD ) {    // no hysteresis here..
			// clear alarm 
			if (high_temperature_alarm_dly) {
				high_temperature_alarm_dly--;
				DBG_PRINTLN ();
				DBG_PRINT   ("Alarm Clearing : High Temperature Alarm.. ticking ...");
				DBG_PRINT   (high_temperature_alarm_dly);
			}
			else {
				alarms_byte &= ~TEMPR_ALARM_BIT;
				// Auto clear neo-pixel LED as well?, if so uncomment below line
				neo_pixel_control (HIGH_TEMPER_ALARM, OFF_LED);  
			}
		}
    }
    else {
        // nop
    }


    // Shutdown criteria
    if (alarms_byte & CRITICAL_ALARMS)  {
        f_critical_alarms = 1;
    }
    else {
        f_critical_alarms = 0;
    }

    if (f_start_alarm_beeps)   {
        beep_for (100);
    }

    
}


// EoF -------------

