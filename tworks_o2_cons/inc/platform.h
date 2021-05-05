// platform.h

#ifndef _PLATFORM_h
#define _PLATFORM_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif


void platform_init (void);



#endif

