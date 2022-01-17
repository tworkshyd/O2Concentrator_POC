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

struct time_stamp_t	{
	
	uint8_t		rec_type;
	uint8_t		ss;
	uint8_t		mm;
	uint8_t		hh;
	uint8_t		dd;
	uint8_t		YM;	// Year & month (one nibble for each)
	
};

struct sensor_data_t	{
	
	uint8_t		rec_type;
	uint8_t		ss;
	// Note : All decimal values are stored in integer format after multiplying by 100, to save on memory
	//			(to avoid expensive 8 bytes floating point values), just divide by 100 to get actual value with
	//			2 digits precision
	uint16_t	o2_p;	// 93.23 %, 2.2 digits		
	uint16_t	press;	// 06.31 psi, 2.2 digits
	uint16_t	tempr;	// 45.57 Deg C, 2.2 digits
	
};



// Note: below union is used to ease handing of different type of records.
union log_record_t	{
	
	uint8_t					rec_type;
	struct time_stamp_t		time_stamp;
	struct sensor_data_t	sensor_data;
	
};





// Function declarations -----------------------------------------------------

void logs_task      (void);
void log_serial_dump(void);
void logs_store     (void);
void logs_retrive   (void);









#endif  // '#ifndef logs_h' ends here..
