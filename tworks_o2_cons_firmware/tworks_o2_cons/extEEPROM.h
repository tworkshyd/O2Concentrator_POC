// platform.h

#ifndef _extEEPROM_h
#define _extEEPROM_h



#include "config.h"
#include "logs.h"


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




// Memory address allocations : For test buffer
#define   EEPROM_TEST_AREA_START          (extEEPROM_START_ADDRESS)
#define   EEPROM_TEST_AREA_SIZE           (extEEPROM_PAGE_SIZE_IN_BYTES)
#define   EEPROM_TEST_AREA_END            (EEPROM_TEST_AREA_START + EEPROM_TEST_AREA_SIZE)
#define   EEPROM_TEST_BUFFER_SIZE         (extEEPROM_PAGE_SIZE_IN_BYTES)

// Memory address allocations : for Run hours and calibration data 
#define   EEPROM_CALIB_START             (EEPROM_TEST_AREA_END)
#define   EEPROM_CALIB_AREA_SIZE         (sizeof(EEP_RECORD_T))
#define   EEPROM_CALIB_AREA_END          (EEPROM_CALIB_START + EEPROM_CALIB_AREA_SIZE)

// Memory address allocations : for log records
#define   EEPROM_LOGS_START_ADDRESS      (EEPROM_CALIB_AREA_END)
#define   EEPROM_LOGS_END_ADDRESS        (extEEPROM_LAST_ADDRESS)
#define   EEPROM_LOGS_AREA_SIZE_IN_BYTES (EEPROM_LOGS_END_ADDRESS - EEPROM_LOGS_START_ADDRESS)
#define   EEPROM_LOGS_RECORD_SIZE        (sizeof(LOG_RECORD_U))
#define   EEPROM_LOGS_TOTAL_COUNT        ((EEPROM_LOGS_END_ADDRESS - EEPROM_LOGS_START_ADDRESS) / EEPROM_LOGS_RECORD_SIZE)
#define   EEPROM_LOGS_AREA_END			 (EEPROM_LOGS_START_ADDRESS + (EEPROM_LOGS_TOTAL_COUNT * EEPROM_LOGS_RECORD_SIZE))



extern uint16_t	eep_log_next_record_address;




// Function declarations -----------------------------------------------------
bool eeprom_init  (void);
void eepwrite     (unsigned int address, byte * buff_p, uint16_t n_bytes);
void eepread      (unsigned int address, byte * buff_p, uint16_t n_bytes);
void eepfill	  (unsigned int address, uint8_t data, uint16_t n_bytes);
void eeptest      (void);
void save_run_hrs_n_calib_constants (void);
void update_log_rcord_head_ptr (uint16_t eep_address);
void push_log_to_eeprom (LOG_RECORD_U * log_p);
LOG_RECORD_U * pull_log_from_eeprom (void);


#endif
