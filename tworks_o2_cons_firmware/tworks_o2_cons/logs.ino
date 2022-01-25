// logs.c

#include <CircularBuffer.h>

#include "db.h"
#include "logs.h"
#include "extEEPROM.h"


CircularBuffer <float, O2_MOVING_AVG_COUNT> cbuf;


int record_no = 0;



void log_init (void)	{
	
	update_log_rcord_head_ptr (EEPROM_LOGS_START_ADDRESS);
	
}


void logs_task (void) {

    static int    log_period;
    static float  last_o2_concentration;

    float         change_in_value;


    // 1. periodic logging

    if (f_sec_logs_task)  {
        f_sec_logs_task = 0;

#ifdef  ENABLE_PERIODIC_LOGS
        log_period++;
        if (log_period >= SET_LOG_PERIOD_SECS)  {
            log_period = 0;
            log_print_on_terminal ();
#endif
        }
    }

    // 2. event logging
#ifdef  ENABLE_EVENT_LOGS
    change_in_value = last_o2_concentration - o2_concentration;
    change_in_value = abs(change_in_value);
    if (change_in_value >= O2_VALUE_CHANGE_THRESHOLD)  {
        last_o2_concentration = o2_concentration;
        log_print_on_terminal ();
    }
#endif

    // 3. debug prints
    // DBG_PRINT   ("nice to ");
    // DBG_PRINTLN (" see u here..!!");

}



void log_print_on_terminal (void)  {

	static uint8_t		skip_count = 0;
	char				temp_buffer[100];
	char				temp1[10];
	char				temp2[10];
	char				temp3[10];

    // sample output : 
				//  On-time  Curr-RHs    PrdDly  FlshDly  PrechrgDly   o2_ADC  o2_mV    %o2  avgO2%     tempr-1  tempr-2    pressure
				//  00:00:23 00:00:22     5400       0        700         822  666.38  18.45  18.45      25.65     24.50      25.03

    // 1. print legend once every (say) 10 records
	#define		SKIP_COUNT		(10)
	if (skip_count >= SKIP_COUNT || skip_count == 0)	{
		skip_count = 0;
		DBG_PRINTLN ();
		DBG_PRINT ("On-time  Curr-RHs    PrdDly  FlshDly  PrechrgDly   o2_ADC  o2_mV    %o2  avgO2%     tempr-1  tempr-2    pressure\r\n" );
				//  On-time  Curr-RHs    PrdDly  FlshDly  PrechrgDly   o2_ADC  o2_mV    %o2  avgO2%     tempr-1  tempr-2    pressure
                //  00:00:23 00:00:22     5400       0        700         822  666.38  18.45  18.45      25.65     24.50      25.03

	}
	skip_count++;

	
    // 2. time stamp
    sprintf(lcd_temp_string, "%02d:%02d:%02d ", systemtick_hrs, systemtick_mins, systemtick_secs);
    DBG_PRINT (lcd_temp_string);

    // 3. system run time
    int secs = ( current_run_time_secs %  60);
    int mins = ((current_run_time_secs % (60 * 60)) / 60);
    int hrs  = ( current_run_time_secs / (60 * 60));
    sprintf(lcd_temp_string, "%02d:%02d:%02d  ", hrs, mins, secs);
    DBG_PRINT (lcd_temp_string);

    // 4. production time, flush time n pre-charge time
	sprintf (temp_buffer, "  %5d   %5d      %5d", Production_Delay, Flush_Delay, PreCharge_Delay);
	DBG_PRINT (temp_buffer);

    // 5. O2 raw adc, mv, %
		// '2nd param' is minimum width, '3rd param' is precision; float value is copied onto '4th param' 
	dtostrf(o2_m_raw_voltage, 6, 2, temp1);
	dtostrf(o2_concentration, 5, 2, temp2);
	dtostrf(o2_moving_avg,    5, 2, temp3);
	sprintf (temp_buffer, "      %5d  %s  %s  %s", o2_raw_adc_count, temp1, temp2, temp3);
	DBG_PRINT (temp_buffer);

    // 6. Temperature n pressure values
	dtostrf(tempr_value_1,   5, 2, temp1);
	dtostrf(tempr_value_2,   5, 2, temp2);
	dtostrf(output_pressure, 5, 2, temp3);
	sprintf (temp_buffer, "      %s     %s      %s", temp1, temp2, temp3);
	DBG_PRINT   (temp_buffer);
    DBG_PRINTLN ();
     
}


void log_serial_dump (void)	{
			
	DBG_PRINTLN ("Printing all collected logs..");
    DBG_PRINTLN ();

	static uint8_t		skip_count = 0;
	char	temp_string[100];

	unsigned int	address;
	unsigned int	record_count;
	LOG_RECORD_U	one_record;
	
	record_count = 1;	// for do while()
	update_log_rcord_head_ptr (EEPROM_LOGS_START_ADDRESS);
	
	// temp
	pull_log_from_eeprom (&one_record);
	DBG_PRINTLN ("---------------------------");
	DBG_PRINTLN ("RecNo.   %O2    psi    *C");
	DBG_PRINTLN ("---------------------------");
	//            #0000   07.14, 00.44, 20.56
	// one_record.time_stamp.mm, one_record.time_stamp.ss);
	
	
	do 
	{
		pull_log_from_eeprom (&one_record);
		sprintf(temp_string, "#%04d   ", record_count);
		DBG_PRINT (temp_string);;
		
		switch (one_record.rec_type)
		{
			case 	LOG_TIME_STAMP:
				sprintf(temp_string, "%02d:%02d:%02d ", one_record.time_stamp.hh, \
									one_record.time_stamp.mm, one_record.time_stamp.ss);
				DBG_PRINT (temp_string);
				break;
			case 	LOG_SENSOR_DATA:
				sprintf(temp_string, "%02d.%02d, %02d.%02d, %02d.%02d", \
							one_record.sensor_data.o2_p / 100, one_record.sensor_data.o2_p % 100, \
							one_record.sensor_data.press / 100, one_record.sensor_data.press % 100, \
							one_record.sensor_data.tempr / 100, one_record.sensor_data.tempr % 100);
				DBG_PRINT (temp_string);
				break;
			
		}
		DBG_PRINTLN ();

		record_count++;
	} while (record_count < (EEPROM_LOGS_TOTAL_COUNT - 1));		
	// 	 } while (one_record.sensor_data.rec_type != 0xFF   &&  record_count < EEPROM_LOGS_TOTAL_COUNT);
	
	DBG_PRINTLN ("---------------------------");
	DBG_PRINTLN ();
	
	DBG_PRINT  ("Total records printed.. : ");
	DBG_PRINTLN (record_count);

	// please reset the log pointer to its starting position
	update_log_rcord_head_ptr(EEPROM_LOGS_START_ADDRESS);
	// todo
		// have two different pointers one for read and the other for write.
	
}




void logs_store     (void)	{
	
	static LOG_RECORD_U		log_data;
	
	
	/* 
		Note: To store more records on eeprom memory .. the time stamp will be store 
			  only once after 'TS_AFTER_N_RECORDS' records.
	*/
	
	#define TS_AFTER_N_RECORDS	(10)	// introduce on time stamp record after these many data records.
		
		
	
	if ((record_no % TS_AFTER_N_RECORDS) == 0  ||  (record_no == 0) )	{
		
		// log time stamp
		log_data.time_stamp.rec_type  = LOG_TIME_STAMP;
		log_data.time_stamp.ss    = systemtick_secs % 60;
		log_data.time_stamp.mm    = systemtick_mins % 60;
		log_data.time_stamp.hh    = systemtick_hrs;
		// todo
		// log_data.time_stamp.dd    = ;
		// log_data.time_stamp.YM    = ;
		
		// store into eeprom
		DBG_PRINT  ("logging time stamp  : ");
		push_log_to_eeprom (&log_data);
		record_no++;
		DBG_PRINTLN (record_no);
	}
	
	log_data.sensor_data.rec_type  = LOG_SENSOR_DATA;
	log_data.sensor_data.o2_p      = (uint16_t)(o2_concentration * 100);
	log_data.sensor_data.press     = (uint16_t)(output_pressure  * 100);
	log_data.sensor_data.tempr     = (uint16_t)(tempr_value_1    * 100);
	
	// store into eeprom
	DBG_PRINT   ("logging sensor data : ");
	push_log_to_eeprom (&log_data);
	record_no++;
	DBG_PRINTLN (record_no);
	
}






// EoF -------------

