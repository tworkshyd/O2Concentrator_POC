// ui.h

#ifndef _UI_h
#define _UI_h


#define LCD_CHARS_MAX		(16)
#define LCD_NO_ROWS 		(2)


// GPIO allocations
#define ADC_pin     (A0)
#define BUZZER_PIN  (A1)
#define LED_PIN     (A3)



#define	DEBOUNCE_DELAY  	(55)


#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5

//------------------------------------------------
// Practical observations:
// Pin High --> Buzzer  ON
// pin Low  --> Buzzer  OFF
//------------------------------------------------
#define BUZZ_ON             (0)
#define BUZZ_OFF            (1)

//------------------------------------------------
// Pin High --> LED  ON
// pin Low  --> LED  OFF
//------------------------------------------------
#define LED_ON              (1)
#define LED_OFF             (0)

#define HIGH_BEEP           (200)
#define LOW_BEEP            ( 50)
#define SYS_ON_BEEP         (555)
#define SYS_OFF_BEEP        (111)



void ui_init            (void);
void ui_task_main       (void);
void power_on_self_test (void);
void beep_for           (int msecs);






#endif






