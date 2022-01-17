// platform.c


#include "platform.h"
#include "extEEPROM.h"




// Function definations --------------------------------------------------

bool eeprom_init (void) {


    uint8_t eepStatus = eep.begin(eep.twiClock400kHz);   //go fast!
    if (eepStatus) {
      Serial.print(F("extEEPROM.begin() failed, status = "));
      Serial.println(eepStatus);
      // while (1);
      // todo - record this error ..
      return false;
      
    }

    // temp
    DBG_PRINTLN();
    DBG_PRINT ("extEEPROM_START_ADDRESS      : ");
    DBG_PRINTLN(extEEPROM_START_ADDRESS);
    DBG_PRINT ("extEEPROM_PAGE_SIZE_IN_BYTES : ");
    DBG_PRINTLN(extEEPROM_PAGE_SIZE_IN_BYTES);
    DBG_PRINT ("extEEPROM_SIZE_IN_BYTES      : ");
    DBG_PRINTLN(extEEPROM_SIZE_IN_BYTES);    
    DBG_PRINT ("extEEPROM_LAST_ADDRESS       : ");
    DBG_PRINTLN(extEEPROM_LAST_ADDRESS);

    DBG_PRINTLN();
    DBG_PRINT ("EEPROM_TEST_AREA_START       : ");
    DBG_PRINTLN(EEPROM_TEST_AREA_START);
    DBG_PRINT ("EEPROM_TEST_AREA_SIZE        : ");
    DBG_PRINTLN(EEPROM_TEST_AREA_SIZE);    
    DBG_PRINT ("EEPROM_TEST_AREA_END         : ");
    DBG_PRINTLN(EEPROM_TEST_AREA_END);

    DBG_PRINTLN();
    DBG_PRINT ("EEPROM_CALIB_START          : ");
    DBG_PRINTLN(EEPROM_CALIB_START);
    DBG_PRINT ("EEPROM_CALIB_AREA_SIZE      : ");
    DBG_PRINTLN(EEPROM_CALIB_AREA_SIZE);    
    DBG_PRINT ("EEPROM_CALIB_AREA_END       : ");
    DBG_PRINTLN(EEPROM_CALIB_AREA_END);
	
    DBG_PRINTLN();
    DBG_PRINT ("EEPROM_LOGS_START_ADDRESS   : ");
    DBG_PRINTLN(EEPROM_LOGS_START_ADDRESS);
    DBG_PRINT ("EEPROM_LOGS_END_ADDRESS     : ");
    DBG_PRINTLN(EEPROM_LOGS_END_ADDRESS);
    DBG_PRINT ("EEPROM_LOGS_RECORD_SIZE     : ");
    DBG_PRINTLN(EEPROM_LOGS_RECORD_SIZE);
    DBG_PRINT ("EEPROM_LOGS_TOTAL_COUNT     : ");
    DBG_PRINTLN(EEPROM_LOGS_TOTAL_COUNT);
    DBG_PRINT ("EEPROM_LOGS_AREA_END        : ");
    DBG_PRINTLN(EEPROM_LOGS_AREA_END);

    return true;
  
}


void eepwrite (unsigned int address, byte * buff_p, uint8_t n_bytes)  {

    unsigned int    len, partial_len, offset_bytes;
    
    if (buff_p == NULL  ||  address > extEEPROM_LAST_ADDRESS || n_bytes == 0) {
        return; 
    }

    offset_bytes = address % extEEPROM_PAGE_SIZE_IN_BYTES;
    partial_len  = extEEPROM_PAGE_SIZE_IN_BYTES - offset_bytes;  

    if (partial_len > n_bytes) {       
        partial_len = n_bytes;
    }

    if (partial_len)  {
        eep.write(address, buff_p, partial_len);
        n_bytes -= partial_len;
        address += partial_len;
        buff_p  += partial_len;
        _delay_us(5);
    }

    while (n_bytes) 
    {
        if (n_bytes > extEEPROM_PAGE_SIZE_IN_BYTES) {
            partial_len = extEEPROM_PAGE_SIZE_IN_BYTES;
        }
        else {
            partial_len = n_bytes;
        }
        eep.write(address, buff_p, partial_len);
        n_bytes -= partial_len;
        address += partial_len;    
        buff_p  += partial_len;
        _delay_us(5);
    }
    
  
}


void eepread (unsigned int address, byte * buff_p, uint8_t n_bytes)  {
    
    if (buff_p == NULL  ||  address > extEEPROM_LAST_ADDRESS || n_bytes == 0) {
        return; 
    }

    eep.read(address, buff_p, n_bytes);

}


bool compare_buff (byte * buff_a, byte * buff_b, int  len)  {

    if (buff_a == NULL || buff_b == NULL || len == 0) {
      return false;
    }

    // else

    while (len)  
    {
        len--;
        if (*buff_a != *buff_b) {
            return false;
        }
        buff_a++;
        buff_b++;
    }

    return true;
}


    
void eeptest (void) {
    
    uint64_t		i, address;
    byte			write_buff[EEPROM_TEST_BUFFER_SIZE];
    byte			read_buff[EEPROM_TEST_BUFFER_SIZE];
    uint64_t		buff_size = EEPROM_TEST_BUFFER_SIZE;
    int				success = 0;
    int				fail = 0;


    DBG_PRINTLN();
    DBG_PRINTLN ("external EEprom test started..");

    DBG_PRINTLN ("test buffer..");
    for (i = 0; i < EEPROM_TEST_BUFFER_SIZE; i++)
    {
        write_buff[i] = i + '3';   
        DBG_PRINT (write_buff[i]);   
        DBG_PRINT (" ");
    }    

    DBG_PRINTLN ();
    
    DBG_PRINT ("Writing test buffer..");
	int		iteration = 0;
    for (address = extEEPROM_START_ADDRESS; address < extEEPROM_LAST_ADDRESS; address += EEPROM_TEST_BUFFER_SIZE)
    {
        eepwrite (address, write_buff, EEPROM_TEST_BUFFER_SIZE);
        DBG_PRINTLN (iteration); 
		iteration++;
    }

    DBG_PRINT ("Reading test buffer..");
	iteration = 0;
    for (address = extEEPROM_START_ADDRESS; address < extEEPROM_LAST_ADDRESS; address += EEPROM_TEST_BUFFER_SIZE)
    {
        eepread (address, read_buff, EEPROM_TEST_BUFFER_SIZE);
        DBG_PRINT (read_buff[iteration % EEPROM_TEST_BUFFER_SIZE]); // just a sample element printed
        DBG_PRINT (" ");   
		iteration++;

        if (compare_buff (read_buff, write_buff, EEPROM_TEST_BUFFER_SIZE) == true)  {
            success++;
        }
        else {
            fail++;
        }
    }
    DBG_PRINTLN ();   

    DBG_PRINT ("Success : ");
    DBG_PRINTLN (success);
    DBG_PRINT ("Fail    : ");
    DBG_PRINTLN (fail);
      
}


void save_run_hrs_n_calib_constants (void) {

	eep_record.last_cycle_run_time_secs = last_cycle_run_time_secs;
	eep_record.total_run_time_secs      = total_run_time_secs;
	
	// write on to eeprom
	if (f_eeprom_working) {
		eepwrite (EEPROM_CALIB_START, (byte*)&eep_record, EEPROM_CALIB_AREA_SIZE);
	}
	
}




/////////////////// scrap area /////////////////////////////////
