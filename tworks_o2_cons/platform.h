// platform.h

#ifndef _PLATFORM_h
#define _PLATFORM_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include "config.h"

/* Prototype general purpose board with aurdino nano
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

/* pin assignments for prototyped board
// 1. LCD display port mappings
#define RS              (7)
#define EN              (8)
#define D4              (9)
#define D5              (10)
#define D6              (11)
#define D7              (12)

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
  
*/


 
/* Rev 1.0 O2 Concentrator Main Board, 
		dated 2021-05-18, developed @ Tworks
 

	PB5(PCINT13/ICP3/MOSI)	 5  |	MOSI --> MOSI --> Din ================> 5
	PB6(PCINT14/OC3A/MISO)	 6  |	MISO  
	PB7(PCINT15/OC3B/SCK)	 7  |	SCK  --> SCK  --> sck ================> 7 
	#RESET                	 4  |	RESET 
	VCC                  	 5  |	VCC   
										  
	PD0(PCINT24/RXD0/T3*)	 8  |	RXD   
	PD1(PCINT25/TXD0)		 9  |	TXD   
	PD2(PCINT26/RXD1/INT0)	10  |	INT0  
										  
	PD3(PCINT27/TXD1/INT1)  11 	|	INT1  
	PD4(PCINT28/XCK1/OC1B)  12  |	DDIR  ================================> 12 
	PD5(PCINT29/OC1A)       13  |	PD5 --> PD5  --> PD5 --> LOAD ========> 13
	PD6(PCINT30/OC2B/ICP)   14  |	PD6   
	PD7(PCINT31/OC2A)       15  |	PD7   
										  
	PC0(PCINT16/SCL)		19  |	SCL   
	PC1(PCINT17/SDA)        20  |   SDA   
	PC2(PCINT18/TCK)        21  |   RS    ================================> 18
	PC3(PCINT19/TMS)        22  |   EN    ================================> 19
										  
    PA0(ADC0/PCINT0) 		37  | 	OUT1  --> SOL1 --> RLY1 ==> ZT1     ==> A0
    PA1(ADC1/PCINT1) 		36  | 	OUT2  --> SOL2 --> RLY2 ==> ZT2     ==> A1
    PA2(ADC2/PCINT2) 		35  | 	OUT3  --> SOL3 --> RLY3 ==> BCF     ==> A2
    PA3(ADC3/PCINT3) 		34  | 	OUT4  --> SPR  --> RLY4 ==> SPRR --> Compressor SSR  ==> A3
	PA4(ADC4/PCINT4) 		33	| 	OUT5  --> CMP  =======================> A4 (not in use for now)
	PA5(ADC5/PCINT5) 		32  | 	OUT8  --> BZR  =======================> A5
	PA6(ADC6/PCINT6) 		31  | 	OUT6  --> x 
	PA7(ADC7/PCINT7) 		30  | 	OUT7  --> x 
										  
	PC7(TOSC2/PCINT23) 		26  |	DB7   ================================> 23
	PC6(TOSC1/PCINT22) 		25  |	DB6   ================================> 22
	PC5(TDI/PCINT21)   		24  |	DB5   ================================> 21
	PC4(TDO/PCINT20)   		23  |	DB4   ================================> 20

    PB4(#SS/OC0B/PCINT12)   44	| 	SW5   ================================>  4
    PB3(AIN1/OC0A7PCINT11)  43  | 	sw4   ================================>  3
    PB2(AIN0/INT2/PCINT10)  42  | 	DT    ================================>  2
    PB1(T1/CLK0/PCINT9)     41  | 	CLK   ================================>  9
    PB0(XCK0/T0/PCINT8)     40  | 	SW    ================================>  8


 */



// 1. LCD display port mappings
#define RS		        (18)
#define EN		        (19)
#define D4		        (20)
#define D5		        (21)
#define D6		        (22)
#define D7		        (23)

// 2. Relay Controls
#define RLY_1           (A0)
#define RLY_2           (A1)
#define RLY_3           (A2)
#define RLY_4           (A3)
#define DDIR            (12)

// 3. Buzzer
#define buzzr_cntrl_pin  	(A5)

// 4. Compressor Control
// #define compr_cntrl_pin  (A4)
#define compr_cntrl_pin     (RLY_4)


// 5. Sensor inputs
#define Prss_sns_1_adc      (A7)    // 'A7' temp assignement to avoid compiler errors
//#define LM35_Out_adc      (A1)

// 6. Button input
#define startSwitchPin      (3)    
#define alarmClearButton    (4)    

// Serial 7-segment display signals
#define dataPin_7segment    (5)     
#define clckPin_7segment    (7)    
#define loadPin_7segment    (13)    

// 8. Encoder input
#define encoderButtonPin    (8)    
#define encoderClk          (9)    
#define encoderData         (10)    

// 9. Neo-pixel out-put pins
#define miso_neo_data1      (6)    
#define pd6_neo_data2       (14)    
#define pd7_neo_data3       (15)    



// RELAY PIN ASSIGNMENT
//**************************************************************************
#ifdef ENABLE_USE_OF_RELAY_3_FOR_Z1    
    // temp arrangement to over-come h/w issue
    #define  Sieve_A_Valve_Z1       RLY_3       // Z1TSOL
    #define  Sieve_B_Valve_Z2       RLY_2       // Z2TSOL
    #define  PreCharge_Valve_BCKF   RLY_1       // BACKFSOL // not used in circuit

#else
    #define  Sieve_A_Valve_Z1       RLY_1       // Z1TSOL
    #define  Sieve_B_Valve_Z2       RLY_2       // Z2TSOL
    #define  PreCharge_Valve_BCKF   RLY_3       // BACKFSOL

#endif

#define BUTTON_ACTIVE       (LOW)

// Digital output Controls
#define BUUZZER_CNTRL(x)            (do_control(BUZZER_CONTROL, x))
#define COMPRSSR_CNTRL(x)           (do_control(COMPRESSOR_CONTROL, x))



// Re-arrange all digital logics as active high logics

//------------------------------------------------
// Practical observations:
// Relay ON --> Valve Open
// Relay OFF -> valve Close
// The valves are NC (Normally Closed) type
//------------------------------------------------
#define OPEN_VALVE          (HIGH)
#define CLOSE_VALVE         (LOW)

//------------------------------------------------
// Practical observations:
// Pin High --> Compressor  ON
// pin Low  --> Compressor  OFF
//------------------------------------------------
#define COMPRSSR_ON         (HIGH)
#define COMPRSSR_OFF        (LOW)


#define BUZZ_ON             (0) 
#define BUZZ_OFF            (1) 




extern volatile unsigned long systemtick_msecs;

void          platform_init   (void);
unsigned long time_elapsed    (unsigned long time_delay);
void          new_delay_msecs (unsigned int  time_delay);


////////////////// external eeprom driver //////////////////
bool eeprom_init  (void);
void eepwrite     (unsigned int address, byte * buff_p, uint8_t n_bytes);
void eepread      (unsigned int address, byte * buff_p, uint8_t n_bytes);
void eeptest      (void);
void save_record  (void);



#endif
