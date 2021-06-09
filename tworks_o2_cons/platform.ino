// platform.c


#include "platform.h"
#include "display.h"



volatile unsigned long int systemtick_msecs;

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

    Serial.println ("Timer initialized..");
}

ISR (TIMER1_COMPA_vect) { // change the 0 to 1 for timer0

    // interrupt commands here
    systemtick_msecs++;

}


void new_delay_msecs (unsigned int  time_delay) {

    unsigned long int   time_tag;


    Serial.print ("/");
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
    pinMode(RLY_1,          OUTPUT);
    pinMode(RLY_2,          OUTPUT);
    pinMode(RLY_3,          OUTPUT);
    // pinMode(RLY_4,       OUTPUT);

    pinMode(BUZZR,          OUTPUT);
    pinMode(COMPRSSR,       OUTPUT);
    pinMode(buttonPin,      INPUT );
    pinMode(buttonPin, INPUT_PULLUP);

    // default pin-state
    digitalWrite(RLY_1,     HIGH);
    digitalWrite(RLY_2,     HIGH);
    digitalWrite(RLY_3,     HIGH);
    // igitalWrite(RLY_4,   HIGH);

    digitalWrite(BUZZR,     HIGH);
    digitalWrite(COMPRSSR,  LOW );

    Serial.println("GPIO init done..");


}


unsigned long int time_elapsed (unsigned long int time_delay)  {

    return systemtick_msecs - time_delay;

}
