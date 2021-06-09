// platform.c


#include "platform.h"
#include "display.h"



unsigned long int systemtick_msecs;


void timer_init (void)  {
  
    cli();//stop interrupts

    TCCR0A = 0; // set entire TCCR0A register to 0
    TCCR0B = 0; // same for TCCR0B
    TCNT0  = 0; // initialize counter value to 0

    // set timer0 interrupt at 1kHz == 1msecs
    // set compare match register for 1khz increments
    OCR0A = 249; // = (16*10^6) / (1000*64) - 1 (must be <256)
    
    // set timer0 interrupt at 2kHz == 500usecs
    // set compare match register for 2khz increments
    // OCR0A = 124;  // = (16*10^6) / (2000*64) - 1 (must be <256)
   
    // turn on CTC mode
    TCCR0A |= (1 << WGM01);
    // Set CS01 and CS00 bits for 64 prescaler
    TCCR0B |= (1 << CS01) | (1 << CS00);
    // enable timer compare interrupt
    TIMSK0 |= (1 << OCIE0A);
    
    sei();//allow interrupts
  
}

ISR (TIMER0_COMPA_vect) { // change the 0 to 1 for timer0
  
    //interrupt commands here
    systemtick_msecs++;

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
