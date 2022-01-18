// logs.c

#include <CircularBuffer.h>

#include "db.h"
#include "logs.h"
#include "extEEPROM.h"




CircularBuffer <int, 400> buffer;
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

    // 1. print legend
    // sample output : 

    // On-time, Curr-RHs, PrdDly, FlshDly, PrechrgDly, o2_raw_ADC, o2_raw_mV, o2_concen,   tempr-snsr-1, tempr-snsr-2, pressures
    // 00:00:23 00:00:22,   5400,       0,        700,    822,      666.38,     18.45,        25.65,        24.50,       25.03

	// print label once every 10 (say) records
	#define		SKIP_COUNT		(10)
	if (skip_count++ >= SKIP_COUNT)	{
		skip_count = 0;
		DBG_PRINTLN ();
		DBG_PRINT ("On-time, Curr-RHs, PrdDly, FlshDly, PrechrgDly, o2_raw_ADC, o2_raw_mV, o2_concen,   tempr-snsr-1, tempr-snsr-2, pressures\r\n" );
	}
	
    // 1. time stamp
    sprintf(lcd_temp_string, "%02d:%02d:%02d ", systemtick_hrs, systemtick_mins, systemtick_secs);
    DBG_PRINT (lcd_temp_string);

    // 2. system run time
    int secs = ( current_run_time_secs %  60);
    int mins = ((current_run_time_secs % (60 * 60)) / 60);
    int hrs  = ( current_run_time_secs / (60 * 60));
    sprintf(lcd_temp_string, "%02d:%02d:%02d,  ", hrs, mins, secs);
    Serial.print (lcd_temp_string);

    // 3. production time, flush time n precharge time
    Serial.printf ("%5d,  ", Production_Delay);
    Serial.printf ("%6d,  ", Flush_Delay);
    Serial.printf ("%9d,  ", PreCharge_Delay);

    // 4. sieve A, B & back flush valve status
    //    sprintf(lcd_temp_string, "%d %d %d ", (do_byte & SIEVE_A_VALVE_CONTROL) != 0, (do_byte & SIEVE_B_VALVE_CONTROL) != 0, (do_byte & SIEVE_FLUSH_VLV_CNTRL) != 0);
    //    Serial.print (lcd_temp_string);

    // 5. O2 raw adc, mv, %
    //sprintf(lcd_temp_string, "%4d %lf %lf ", o2_raw_adc_count, m_raw_voltage, o2_concentration);
    //Serial.print (lcd_temp_string);
    Serial.printf ("%5d,      ", o2_raw_adc_count);
    Serial.print (o2_m_raw_voltage, 2);
    Serial.print (",     ");
    Serial.print (o2_concentration, 2);
    Serial.print (",    ");

    // 6. Temperature values
    Serial.print ("    ");
    Serial.print(tempr_value_1);
    Serial.print (",        ");
    Serial.print(tempr_value_2);
    Serial.print (",       ");
     
    // 7. Pressure 
    Serial.print(output_pressure, 2);
    Serial.println("\r\n");
    //Serial.println("\r\n");

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
// 		DBG_PRINT ("#");
// 		DBG_PRINT (record_count);
// 		DBG_PRINT (" ");
		sprintf(temp_string, "#%04d   ", record_count);
		DBG_PRINT (temp_string);;
		
		switch (one_record.rec_type)
		{
			case 	LOG_TIME_STAMP:
// 				DBG_PRINT (one_record.time_stamp.hh);	DBG_PRINT (":");
// 				DBG_PRINT (one_record.time_stamp.mm);	DBG_PRINT (":");
// 				DBG_PRINT (one_record.time_stamp.ss);	
				sprintf(temp_string, "%02d:%02d:%02d ", one_record.time_stamp.hh, \
									one_record.time_stamp.mm, one_record.time_stamp.ss);
				DBG_PRINT (temp_string);
				break;
			case 	LOG_SENSOR_DATA:
// 				DBG_PRINT (one_record.sensor_data.o2_p);	DBG_PRINT (", ");
// 				DBG_PRINT (one_record.sensor_data.press);	DBG_PRINT (", ");
// 				DBG_PRINT (one_record.sensor_data.tempr);	
				sprintf(temp_string, "%02d.%02d, %02d.%02d, %02d.%02d", \
							one_record.sensor_data.o2_p / 100, one_record.sensor_data.o2_p % 100, \
							one_record.sensor_data.press / 100, one_record.sensor_data.press % 100, \
							one_record.sensor_data.tempr / 100, one_record.sensor_data.tempr % 100);
				DBG_PRINT (temp_string);
				break;
			
		}
		DBG_PRINTLN ();
// 		DBG_PRINTLN (one_record.rec_type);
		
// 		DBG_PRINT   ("one_record.sensor_data.rec_type : ");
// 		DBG_PRINTLN (one_record.sensor_data.rec_type);
// 		DBG_PRINT   ("one_record.time_stamp.rec_type : ");
// 		DBG_PRINTLN (one_record.time_stamp.rec_type);
// 		DBG_PRINT   ("record_count : ");
// 		DBG_PRINTLN (record_count);
// 		DBG_PRINTLN ();

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

