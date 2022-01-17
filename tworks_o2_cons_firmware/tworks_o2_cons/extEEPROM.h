// platform.h

#ifndef _extEEPROM_h
#define _extEEPROM_h



#include "config.h"


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





// Function declarations -----------------------------------------------------
bool eeprom_init  (void);
void eepwrite     (unsigned int address, byte * buff_p, uint8_t n_bytes);
void eepread      (unsigned int address, byte * buff_p, uint8_t n_bytes);
void eeptest      (void);
void save_record  (void);



#endif
