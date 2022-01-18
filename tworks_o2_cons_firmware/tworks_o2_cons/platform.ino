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
#if   (HW_REVISION == HW_REV_1_0)
    pinMode(RLY_4,            OUTPUT);
#elif (HW_REVISION == HW_REV_2_0)
	// nop
#else
	// nop
#endif
	
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


void test_gpios (void) {

// 	timer_init ();

	// set up the LCD's number of columns and rows:
	lcd.begin(LCD_COLS, LCD_ROWS);

	DBG_PRINTLN("GPIO testing...");

	// pin mode setting
	pinMode (RLY_1,            OUTPUT);
	DBG_PRINTLN ("RLY_1 = HIGH");
	digitalWrite (RLY_1,        HIGH);
	delay(1300);
	DBG_PRINTLN ("RLY_1 = low");
	digitalWrite (RLY_1,        LOW);
	delay(1300);
	
	pinMode (RLY_2,            OUTPUT);
	DBG_PRINTLN ("RLY_2 = HIGH");
	digitalWrite (RLY_2,        HIGH);
	delay(1300);
	DBG_PRINTLN ("RLY_2 = low");
	digitalWrite (RLY_2,        LOW);
	delay(1300);

	pinMode (RLY_3,            OUTPUT);
	DBG_PRINTLN ("RLY_3 = HIGH");
	digitalWrite (RLY_3,        HIGH);
	delay(1300);
	DBG_PRINTLN ("RLY_3 = low");
	digitalWrite (RLY_3,        LOW);
	delay(1300);

	#if   (HW_REVISION == HW_REV_1_0)
		pinMode (RLY_4,            OUTPUT);
		DBG_PRINTLN ("RLY_4 = HIGH");
		digitalWrite (RLY_4,        HIGH);
		delay(1300);
		DBG_PRINTLN ("RLY_4 = low");
		digitalWrite (RLY_4,        LOW);
		delay(1300);
	#elif (HW_REVISION == HW_REV_2_0)
		// nop
	#else
		// nop
	#endif
	
	pinMode (DDIR,            OUTPUT);
	DBG_PRINTLN ("DDIR = HIGH");
	digitalWrite (DDIR,        HIGH);
	delay(1300);
	DBG_PRINTLN ("DDIR = low");
	digitalWrite (DDIR,        LOW);
	delay(1300);

	DBG_PRINTLN ("setting to default state.");
	DBG_PRINTLN ("DDIR = HIGH");
	digitalWrite(DDIR,        HIGH);

	pinMode (buzzr_cntrl_pin,  OUTPUT);
	DBG_PRINTLN ("buzzr_cntrl_pin = HIGH");
	digitalWrite (buzzr_cntrl_pin,        HIGH);
	delay(1300);
	DBG_PRINTLN ("buzzr_cntrl_pin = low");
	digitalWrite (buzzr_cntrl_pin,        LOW);
	delay(1300);

	pinMode(compr_cntrl_pin,             OUTPUT);
	DBG_PRINTLN ("compr_cntrl_pin = HIGH");
	digitalWrite (compr_cntrl_pin,        HIGH);
	delay(1300);
	DBG_PRINTLN ("compr_cntrl_pin = low");
	digitalWrite (compr_cntrl_pin,        LOW);
	delay(1300);

	
	// default pin-state
	digitalWrite(buzzr_cntrl_pin,   LOW);
	DBG_PRINTLN ("buzzr_cntrl_pin = HIGH");
	digitalWrite (buzzr_cntrl_pin,        HIGH);
	delay(1300);
	DBG_PRINTLN ("buzzr_cntrl_pin = low");
	digitalWrite (buzzr_cntrl_pin,        LOW);
	delay(1300);


	// Serial 7 segment interface
	pinMode(dataPin_7segment,       OUTPUT);
	DBG_PRINTLN ("compr_cntrl_pin = HIGH");
	digitalWrite (compr_cntrl_pin,        HIGH);
	delay(1300);
	DBG_PRINTLN ("compr_cntrl_pin = low");
	digitalWrite (compr_cntrl_pin,        LOW);
	delay(1300);

	pinMode(clckPin_7segment,       OUTPUT);
	DBG_PRINTLN ("compr_cntrl_pin = HIGH");
	digitalWrite (compr_cntrl_pin,        HIGH);
	delay(1300);
	DBG_PRINTLN ("compr_cntrl_pin = low");
	digitalWrite (compr_cntrl_pin,        LOW);
	delay(1300);

	pinMode(loadPin_7segment,       OUTPUT);
	DBG_PRINTLN ("compr_cntrl_pin = HIGH");
	digitalWrite (compr_cntrl_pin,        HIGH);
	delay(1300);
	DBG_PRINTLN ("compr_cntrl_pin = low");
	digitalWrite (compr_cntrl_pin,        LOW);
	delay(1300);


	DBG_PRINTLN ("setting to default state...");
	digitalWrite(dataPin_7segment,  LOW );
	digitalWrite(clckPin_7segment,  LOW );
	digitalWrite(loadPin_7segment,  LOW );

	// neo-pixel leds interface
	pinMode(miso_neo_data1,         OUTPUT);
	DBG_PRINTLN ("miso_neo_data1 = HIGH");
	digitalWrite (miso_neo_data1,        HIGH);
	delay(1300);
	DBG_PRINTLN ("miso_neo_data1 = low");
	digitalWrite (miso_neo_data1,        LOW);
	delay(1300);

	pinMode(pd6_neo_data2,          OUTPUT);
	DBG_PRINTLN ("pd6_neo_data2 = HIGH");
	digitalWrite (pd6_neo_data2,        HIGH);
	delay(1300);
	DBG_PRINTLN ("pd6_neo_data2 = low");
	digitalWrite (pd6_neo_data2,        LOW);
	delay(1300);

	pinMode(pd7_neo_data3,          OUTPUT);
	DBG_PRINTLN ("pd7_neo_data3 = HIGH");
	digitalWrite (pd7_neo_data3,       HIGH);
	delay(1300);
	DBG_PRINTLN ("pd7_neo_data3 = low");
	digitalWrite (pd7_neo_data3,        LOW);
	delay(1300);


	DBG_PRINTLN ("setting to default state..");
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


}



unsigned long int time_elapsed (unsigned long int time_delay)  {

    return systemtick_msecs - time_delay;

}





/////////////////// scrap area /////////////////////////////////
