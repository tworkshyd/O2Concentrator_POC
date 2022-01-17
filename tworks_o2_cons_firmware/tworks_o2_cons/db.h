// data_base.h

#ifndef _DATA_BASE_h
#define _DATA_BASE_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif


/* ---------------------------------------------------------------------------
 * Select EEPROM device appropriately
 * ------------------------------------------------------------------------ */
// #define extEEPROM_PART_NO_AT24C		( 32)	//  32-Kbit ( 4,096 x 8)
// #define extEEPROM_PART_NO_AT24C		( 64)	//  64-Kbit ( 8,192 x 8)
// #define extEEPROM_PART_NO_AT24C		(128)	// 128-Kbit (16,384 x 8)
   #define extEEPROM_PART_NO_AT24C		(256)	// 256-Kbit (32,768 x 8)
// ---------------------------------------------------------------------------
	

// external EEPROM memory address range and memory map, as per the selected device
#define   extEEPROM_NO_OF_DEVICES             (1)		

#if (extEEPROM_PART_NO_AT24C == 32)	
	#define   extEEPROM_SIZE_IN_BITS          kbits_32		// (4096L * 8L)
	#define   extEEPROM_PAGE_SIZE_IN_BYTES    (32L)

#elif (extEEPROM_PART_NO_AT24C == 64)	
	#define   extEEPROM_SIZE_IN_BITS          kbits_64		// (8192L * 8L)
	#define   extEEPROM_PAGE_SIZE_IN_BYTES    (32L)

#elif (extEEPROM_PART_NO_AT24C == 128)
	#define   extEEPROM_SIZE_IN_BITS          kbits_128		// (16384L * 8L)
	#define   extEEPROM_PAGE_SIZE_IN_BYTES    (64L)

#elif (extEEPROM_PART_NO_AT24C == 256)
	#define   extEEPROM_SIZE_IN_BITS          kbits_256		// (32768L * 8L)
	#define   extEEPROM_PAGE_SIZE_IN_BYTES    (64L)

#else

	#error "Please select EEPROM device used..!!!"
#endif



#define   extEEPROM_START_ADDRESS         (0x00L)
#define   extEEPROM_SIZE_IN_BYTES         ((extEEPROM_SIZE_IN_BITS * 1024L) / 8L)
#define   extEEPROM_LAST_ADDRESS          (extEEPROM_START_ADDRESS + extEEPROM_SIZE_IN_BYTES)


// Memory address allocations
#define   EEPROM_TEST_AREA_START          (extEEPROM_START_ADDRESS)
#define   EEPROM_TEST_AREA_SIZE           (extEEPROM_PAGE_SIZE_IN_BYTES)
#define   EEPROM_TEST_AREA_END            (EEPROM_TEST_AREA_START + EEPROM_TEST_AREA_SIZE)
#define   EEPROM_TEST_BUFFER_SIZE         (33)

#define   EEPROM_RECORD_START             (EEPROM_TEST_AREA_END)
#define   EEPROM_RECORD_AREA_SIZE         (sizeof(EEP_RECORD_T))
#define   EEPROM_RECORD_AREA_END          (EEPROM_RECORD_START + EEPROM_RECORD_AREA_SIZE)

// ... 




// Structure declarations
typedef struct eep_record_t {

    // 1. Run hour counters 
    unsigned int    last_cycle_run_time_secs;
    unsigned long   total_run_time_secs;  

    // 2. Calibration constants
    float           O2_slope;
    float           O2_constant;
    
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
