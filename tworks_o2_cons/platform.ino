// platform.c


#include "platform.h"
#include "display.h"
#include "db.h"

// Note: timer 0 is not working with I2C peripheral, hence use Timer 1 / 2 for systick


void config_timer1  (void)  {

    // Timer 1
    TCCR1A = 0; // set entire TCCR0A register to 0
    TCCR1B = 0; // same for TCCR0B
    TCNT1  = 0; // initialize counter value to 0

    // set timer1 interrupt at 1kHz == 1msecs
    // set compare match register for 1khz increments
    OCR1A = 249; // = (16*10^6) / (1000*64) - 1 (must be < 66535)

    // set timer1 interrupt at 2kHz == 500usecs
    // set compare match register for 2khz increments
    // OCR1A = 124;  // = (16*10^6) / (2000*64) - 1 (must be < 66535)

    // turn on CTC mode
    TCCR1B |= (1 << WGM12);
    // Set CS12, CS11 and CS10 bits for 64 prescaler
    TCCR1B |= (1 << CS11) | (1 << CS10);

    // enable timer compare interrupt
    TIMSK1 |= (1 << OCIE1A);

}


void timer_init (void)  {

    cli();//stop interrupts
    config_timer1 ();
    sei();//allow interrupts

    DBG_PRINTLN ("Timer initialized..");
}

ISR (TIMER1_COMPA_vect) { // change the 0 to 1 for timer0

    // interrupt commands here
    systemtick_msecs++;

    f_msec = 1;
    if ( (systemtick_msecs % 10) == 0)  {
        f_10msec = 1;
        if ( (systemtick_msecs % 100) == 0)  {
            f_100msec = 1;
            if ( (systemtick_msecs % 1000) == 0)  {
                f_1sec = 1;
                systemtick_secs++;
                if (systemtick_secs >= 60)  {
                    systemtick_secs = 0;
                    f_1min = 1;
                    systemtick_mins++;
                    if (systemtick_mins >= 60) {
                        systemtick_mins = 0;
                        f_1hr = 1;
                        systemtick_hrs++;
                    }
                }
            }
        }
    }
}


void new_delay_msecs (unsigned int  time_delay) {

    unsigned long int   time_tag;


    //DBG_PRINT ("/");
    time_tag = systemtick_msecs;
    while (time_elapsed (time_tag) < time_delay)
    {
        // nop
    }
}


void platform_init (void) {

    timer_init ();

    // set up the LCD's number of columns and rows:
    lcd.begin(LCD_COLS, LCD_ROWS);

    // pin mode setting
    pinMode(RLY_1,            OUTPUT);
    pinMode(RLY_2,            OUTPUT);
    pinMode(RLY_3,            OUTPUT);
    pinMode(RLY_4,            OUTPUT);   
    pinMode(DDIR,             OUTPUT);
    digitalWrite(DDIR,        HIGH);

    pinMode(buzzr_cntrl_pin,  OUTPUT);
    pinMode(compr_cntrl_pin,  OUTPUT);
    pinMode(startSwitchPin,   INPUT );
    pinMode(startSwitchPin,   INPUT_PULLUP);
    pinMode(alarmClearButton, INPUT );
    pinMode(alarmClearButton, INPUT_PULLUP);
    
    // default pin-state
    digitalWrite(buzzr_cntrl_pin,   LOW);
    digitalWrite(compr_cntrl_pin,   LOW );

    // Serial 7 segment interface
    pinMode(dataPin_7segment,       OUTPUT);
    pinMode(clckPin_7segment,       OUTPUT);
    pinMode(loadPin_7segment,       OUTPUT);

    digitalWrite(dataPin_7segment,  LOW );
    digitalWrite(clckPin_7segment,  LOW );
    digitalWrite(loadPin_7segment,  LOW );

    // neo-pixel leds interface
    pinMode(miso_neo_data1,         OUTPUT);
    pinMode(pd6_neo_data2,          OUTPUT);
    pinMode(pd7_neo_data3,          OUTPUT);

    digitalWrite(miso_neo_data1,    LOW );
    digitalWrite(pd6_neo_data2,     LOW );
    digitalWrite(pd7_neo_data3,     LOW );

    DBG_PRINTLN("GPIO init done..");

}


bool do_control (DO_CONTROLS_E do_id, bool bit_value) {


    // DBG_PRINT ("do_id : ");
    // DBG_PRINTLN (do_id);

    switch (do_id)
    {
        case SIEVE_A_VALVE_CONTROL:
            digitalWrite(Sieve_A_Valve_Z1,      bit_value);
            break;
        case SIEVE_B_VALVE_CONTROL:
            digitalWrite(Sieve_B_Valve_Z2,      bit_value);
            break;
        case SIEVE_FLUSH_VLV_CNTRL:
            digitalWrite(PreCharge_Valve_BCKF,  bit_value);
            break;
        case COMPRESSOR_CONTROL:
            digitalWrite(compr_cntrl_pin,       bit_value);
            break;
        case BUZZER_CONTROL:
            digitalWrite(buzzr_cntrl_pin,       !bit_value);
            break;
        case _7SEG_DATA_CONTROL:
        case _7SEG_CLCK_CONTROL:
        case _7SEG_CS_CONTROL:
            break;
        default:
            do_id = INVALID_DO_ID;
            break;
    }


    if (do_id != INVALID_DO_ID)  {
        if (bit_value)  {
            do_byte |= do_id;
        }
        else {
            do_byte &= ~do_id;
        }
    }

    // temp
    // Serial.println(bit_value); Serial.println(do_id); Serial.println(do_byte);

    // DBG_PRINT ("do_byte : ");
    // DBG_PRINTLN(do_byte);

}



unsigned long int time_elapsed (unsigned long int time_delay)  {

    return systemtick_msecs - time_delay;

}




////////////////// external eeprom driver //////////////////
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
    DBG_PRINT ("extEEPROM_SIZE_IN_BITS       : ");
    DBG_PRINTLN(extEEPROM_SIZE_IN_BITS);
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
    DBG_PRINT ("EEPROM_RECORD_START          : ");
    DBG_PRINTLN(EEPROM_RECORD_START);
    DBG_PRINT ("EEPROM_RECORD_AREA_SIZE      : ");
    DBG_PRINTLN(EEPROM_RECORD_AREA_SIZE);    
    DBG_PRINT ("EEPROM_RECORD_AREA_END       : ");
    DBG_PRINTLN(EEPROM_RECORD_AREA_END);

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
    
    int       i, address;
    byte      write_buff[EEPROM_TEST_BUFFER_SIZE];
    byte      read_buff[EEPROM_TEST_BUFFER_SIZE];
    int       buff_size = EEPROM_TEST_BUFFER_SIZE;
    int       success = 0;
    int       fail = 0;


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
    for (address = extEEPROM_START_ADDRESS; address < extEEPROM_LAST_ADDRESS; address += EEPROM_TEST_BUFFER_SIZE)
    {
        eepwrite (address, write_buff, EEPROM_TEST_BUFFER_SIZE);
    }

    DBG_PRINT ("Reading test buffer..");
    for (address = extEEPROM_START_ADDRESS; address < extEEPROM_LAST_ADDRESS; address += EEPROM_TEST_BUFFER_SIZE)
    {
        eepread (address, read_buff, EEPROM_TEST_BUFFER_SIZE);
        DBG_PRINT (read_buff[5]); // just a sample element printed
        DBG_PRINT (" ");   

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


void save_record (void) {

    eep_record.last_cycle_run_time_secs = last_cycle_run_time_secs;
    eep_record.total_run_time_secs      = total_run_time_secs;
    
    // write on to eeprom
    if (f_eeprom_working) {
      eepwrite (EEPROM_RECORD_START, (byte*)&eep_record, EEPROM_RECORD_AREA_SIZE);
    }
            
}

/////////////////// scrap area /////////////////////////////////
/*
    // temp : test for LCD pins
    while (1)
    {
        digitalWrite(dataPin_7segment,   LOW );
        digitalWrite(clckPin_7segment,   LOW );
        digitalWrite(loadPin_7segment,   LOW );
        delay(1000);
        digitalWrite(dataPin_7segment,   HIGH );
        digitalWrite(clckPin_7segment,   HIGH );
        digitalWrite(loadPin_7segment,   HIGH );
        delay(1000);
        
     }
*/
