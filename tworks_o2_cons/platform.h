// platform.h

#ifndef _PLATFORM_h
#define _PLATFORM_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include "config.h"




/* Board selection

	use the preprocessor directives to incorporate board specific changes
	'#if (HW_REVISION == HW_REV_1_0)'

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	#Rev1_0
	Date   : 21 May 2021
	Details:
		O2 Concentrator Rev 1.0 - having 3 boards(Display board, sensor board
		and control board), with control board MCU Atmega1284/p,
		designed @ Tworks

	Modifications	:
	Additions		:
	Deletions		:
	
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
#define HW_REV_1_0          (10)        // for Rev 1.0
//----------------------------------------------------------------------------



/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	#Rev2_0
	Date			: 25 Oct 2021
	Details			:
		O2 Concentrator Rev 2.0 - having 2 boards(Display board, and control 
		board, with control	board MCU Atmega1284/p, designed @ Tworks
		
	Modifications	:
	Additions		:
	Deletions		:
		
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
#define HW_REV_2_0          (20)        // for Rev 2.0
//----------------------------------------------------------------------------




#if   (HW_REVISION == HW_REV_1_0)
	#define		HW_REVISION_TXT      "Rev 1.0"
	
#elif (HW_REVISION == HW_REV_2_0)
	#define		HW_REVISION_TXT      "Rev 2.0"

#else
	#error "Please make proper board selsection..!!! (in 'config.h' file)"
	
#endif 


//----------------------------------------------------------------------------
// A. Common settings across the revisions
//----------------------------------------------------------------------------

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
#if   (HW_REVISION == HW_REV_1_0)
	#define RLY_4           (A3)
	
#elif (HW_REVISION == HW_REV_2_0)
	// no 4th relay
#else
	// nop
#endif


// 3. Buzzer
#if   (HW_REVISION == HW_REV_1_0)
	#define buzzr_cntrl_pin  	(A5)

#elif (HW_REVISION == HW_REV_2_0)
	#define buzzr_cntrl_pin  	(A4)
#else
// nop
#endif


// 4. Compressor Control
#if   (HW_REVISION == HW_REV_1_0)
	#define compr_cntrl_pin     (RLY_4)

#elif (HW_REVISION == HW_REV_2_0)
	#define compr_cntrl_pin     (A3)
#else
	// nop
#endif


// 5. Sensor inputs
#if   (HW_REVISION == HW_REV_1_0)
	#define Prss_sns_1_adc		(A7)    // 'A7' temp assignment to avoid compiler errors

#elif (HW_REVISION == HW_REV_2_0)
	// nop
#else
	// nop
#endif


// 6. Button input
	#define startSwitchPin			(3)    
	#define alarmClearButton		(4)    


// Serial 7-segment display signals
#if   (HW_REVISION == HW_REV_1_0)
	#define dataPin_7segment    (5)
	#define clckPin_7segment    (7)
	#define loadPin_7segment    (13)

#elif (HW_REVISION == HW_REV_2_0)
	#define dataPin_7segment    (5)
	#define clckPin_7segment    (7)
	#define loadPin_7segment    (A5)
#else
	// nop
#endif


// 8. Encoder input
#if   (HW_REVISION == HW_REV_1_0)
	#define encoderButtonPin    (0)
	#define encoderClk          (1)
	#define encoderData         (2)

#elif (HW_REVISION == HW_REV_2_0)
	#define encoderButtonPin    (0)
	#define encoderClk          (1)
	#define encoderData         (2)

#else
// nop
#endif

// 9. Neo-pixel out-put pins
#if   (HW_REVISION == HW_REV_1_0)
	#define miso_neo_data1      (6)
	#define pd6_neo_data2       (14)
	#define pd7_neo_data3       (15)

#elif (HW_REVISION == HW_REV_2_0)
	#define miso_neo_data1      (6)
	#define pd6_neo_data2       (A6)
	#define pd7_neo_data3       (A7)

#else
	// nop
#endif


// 10. Serial Communication
	#define DDIR				(12)




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


//------------------------------------------------
// Practical observations:
// Pin High --> Buzzer  ON
// pin Low  --> Buzzer  OFF
//------------------------------------------------
	#define BUZZ_ON             (0)
	#define BUZZ_OFF            (1)




extern volatile unsigned long systemtick_msecs;



// Function declarations -----------------------------------------------------
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
