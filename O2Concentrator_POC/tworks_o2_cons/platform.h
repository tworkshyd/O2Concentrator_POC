// platform.h

#ifndef _PLATFORM_h
#define _PLATFORM_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif



/*
    1. 1st row : From Port map O2 concentrator document &

    2. 2nd row : From Aurdino Nano - pinout diagram
        "https://components101.com/microcontrollers/arduino-nano"
    3. As per O2 concentrator circuit

    ========================================================================================================
    PortB - PB7         PB6         PB5         PB4         PB3             PB2         PB1             PB0
        -           -           13/SCK      12/MISO     11/PWM/MOSI     10/PWM/#SS  9/MISO/INT1     8/
    --------------------------------------------------------------------------------------------------------
        -           -           Low-Buz     DB7         DB6             DB5         DB4             En
    ========================================================================================================

    ========================================================================================================
    PortC - PC7     PC6         PC5             PC4             PC3         PC2        PC1          PC0
        -       -/RESET     19/A5/ADC5/SCL  18/A4/ADC4/SDA  17/A3/ADC3  16/A2/ADC2 15/A1/ADC1   14/A0/ADC0
    --------------------------------------------------------------------------------------------------------
        ASpare4 ASpare3     SCL             SDA             Aspare2     Push_Button LM35Out     Psns1
    ========================================================================================================

    ========================================================================================================
    PortD - PD7     PD6         PD5         PD4     PD3         PD2         PD1         PD0
        7/      6/PWM       5/PWM       4/      3/PWM/INT1  2/INT0      1/TXD       0/RXD
    --------------------------------------------------------------------------------------------------------
        RS      Comp-On     BckFSol     ZT2Sol  ZT1Sol      DDIR        TXD         RXD
    ========================================================================================================
*/




// 1. LCD display port mappings
#define RS		        (7)
#define EN		        (8)
#define D4		        (9)
#define D5		        (10)
#define D6		        (11)
#define D7		        (12)

// 2. Relay Controls
#define RLY_1           (5)
#define RLY_2           (4)
#define RLY_3           (3)
//#define RLY_4         ()
//#define DDIR          ()

// Alias names
#define BCK_F_SOLINOID  RLY_1
#define ZT2_SOLINOID    RLY_2
#define ZT1_SOLINOID    RLY_3


// 3. Buzzer
#define buzzr_cntrl_pin           (13)

// 4. Compressor Control
#define compr_cntrl_pin (6)

// 5. Sensor inputs
#define Prss_sns_1_adc  (A0)
#define LM35_Out_adc    (A1)

// 6. Button input
#define buttonPin       (A2)    // (2)

// Serial 7-segment display signals
#define dataPin         (2)    // 
#define clckPin         (A3)    // 
#define csPin           (A6)    // 




// RELAY PIN ASSIGNMENT
//**************************************************************************
#define  Sieve_A_Valve_Z1       RLY_3       // Z1TSOL
#define  Sieve_B_Valve_Z2       RLY_2       // Z2TSOL
#define  PreCharge_Valve_BCKF   RLY_1       // BACKFSOL


#define BUTTON_ACTIVE            (LOW)
#define BUTTON_DEBOUNCE_DLY      ( 55)      // in msecs
#define BUTTON_LONG_PRESS_DLY   (1555)      // in msecs


// Digital output Controls
#define BUUZZER_CNTRL(x)        (do_control(BUZZER_CONTROL, x))
#define COMPRSSR_CNTRL(x)       (do_control(COMPRESSOR_CONTROL, x))



// Re-arrange all digital logics as active high logics

//------------------------------------------------
// Practical observations:
// Relay ON --> Valve Open
// Relay OFF -> valve Close
// The valves are NC (Normally Closed) type
//------------------------------------------------
#define OPEN_VALVE          (LOW)
#define CLOSE_VALVE         (HIGH)

//------------------------------------------------
// Practical observations:
// Pin High --> Compressor  ON
// pin Low  --> Compressor  OFF
//------------------------------------------------
#define COMPRSSR_ON         (HIGH)
#define COMPRSSR_OFF        (LOW)


#define BUZZ_ON             (1) 
#define BUZZ_OFF            (0) 


extern volatile unsigned long systemtick_msecs;

void          platform_init   (void);
unsigned long time_elapsed    (unsigned long time_delay);
void          new_delay_msecs (unsigned int  time_delay);


#endif
