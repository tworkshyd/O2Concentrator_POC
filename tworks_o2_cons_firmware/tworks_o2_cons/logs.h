// logs.h

#ifndef logs_h
#define logs_h


#include "config.h"

// 1. Periodic logs
#ifdef  ENABLE_PERIODIC_LOGS
#define PLOG_PRINTLN     Serial.println
#define PLOG_PRINT       Serial.print
#else
#define PLOG_PRINTLN     // Serial.println
#define PLOG_PRINT       // Serial.print
#endif

// 2. Event logs
#ifdef  ENABLE_EVENT_LOGS
#define EVNTLOG_PRINTLN Serial.println
#define EVNTLOG_PRINT   Serial.print
#else
#define EVNTLOG_PRINTLN //  Serial.println
#define EVNTLOG_PRINT   //  Serial.print
#endif


// 3. Debug logs
#ifdef  ENABLE_DEBUG_PRINTS
#define DBG_PRINTLN     Serial.println
#define DBG_PRINT       Serial.print
#else
#define DBG_PRINTLN     //  Serial.println
#define DBG_PRINT       //  Serial.print
#endif


// Enum ----------------------------------------------------------------------
typedef enum log_type_e	{
	
	LOG_SENSOR_DATA = 0x03,
	LOG_TIME_STAMP
	
} LOG_TYPE_E;


// Structure declarations ----------------------------------------------------

// Periodicity - Capture log every time the 4 step cycle ends.

/*
 Parameter			Remarks
	HH				RTC, 24 hour time
	MM				RTC, 24 hour time
	SS				RTC, 24 hour time
	O2 %			Int
	P1				Pressure in psi, float
	T1				Int/Byte
*/

typedef struct time_stamp_t	{
	
	LOG_TYPE_E	rec_type;
	uint8_t		ss;
	uint8_t		mm;
	uint8_t		hh;
	uint8_t		dd;
	uint8_t		YM;	// Year & month (one nibble for each)
	
} TIME_STAMP_T;

typedef struct sensor_data_t	{
	
	LOG_TYPE_E	rec_type;
	uint8_t		ss;
	// Note : All decimal values are stored in integer format after multiplying by 100, to save on memory
	//			(to avoid expensive 8 bytes floating point values), just divide by 100 to get actual value with
	//			2 digits precision
	uint16_t	o2_p;	// 93.23 %, 2.2 digits		
	uint16_t	press;	// 06.31 psi, 2.2 digits
	uint16_t	tempr;	// 45.57 Deg C, 2.2 digits
	
} SENSOR_DATA_T;



// Note: below union is used to ease handing of different type of records.
typedef union log_record_t	{
	
	LOG_TYPE_E		rec_type;
	TIME_STAMP_T	time_stamp;
	SENSOR_DATA_T	sensor_data;
	
} LOG_RECORD_U;






// Function declarations -----------------------------------------------------
void log_init		(void);
void logs_task      (void);
void log_serial_dump(void);
void logs_store     (void);
void logs_retrive   (void);









#endif  // '#ifndef logs_h' ends here..
