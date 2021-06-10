// o2_cons.h


#ifndef   o2_cons_h
  #define o2_cons_h




enum DO_CONTROLS_E {

    SIEVE_A_VALVE_CONTROL          = 0x01,
    SIEVE_B_VALVE_CONTROL          = 0x02,
    SIEVE_FLUSH_VLV_CNTRL          = 0x04,
    COMPRESSOR_CONTROL             = 0x08,
    
    BUZZER_CONTROL                 = 0x10,    
    _7SEG_DATA_CONTROL             = 0x20,
    _7SEG_CLCK_CONTROL             = 0x40,
    _7SEG_CS_CONTROL               = 0x80,

    INVALID_DO_ID                  = 0xFF
  
};


bool do_control (DO_CONTROLS_E do_id, bool bit_value);












#endif  // '#ifndef   o2_cons_h' ends ...
