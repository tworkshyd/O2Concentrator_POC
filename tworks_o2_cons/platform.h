// platform.h

#ifndef _PLATFORM_h
#define _PLATFORM_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif


// LCD display port mappings
#define RS		(7)
#define EN		(8)
#define D4		(9)
#define D5		(10)
#define D6		(11)
#define D7		(12)



void platform_init (void);



#endif

