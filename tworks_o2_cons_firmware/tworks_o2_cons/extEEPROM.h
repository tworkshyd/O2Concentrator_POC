// platform.h

#ifndef _extEEPROM_h
#define _extEEPROM_h



#include "config.h"






// Function declarations -----------------------------------------------------
bool eeprom_init  (void);
void eepwrite     (unsigned int address, byte * buff_p, uint8_t n_bytes);
void eepread      (unsigned int address, byte * buff_p, uint8_t n_bytes);
void eeptest      (void);
void save_record  (void);



#endif
