#ifdef AVR_3PI
// The 3pi include file must be at the beginning of any program that
// uses the Pololu AVR library and 3pi.
#include <pololu/3pi.h>

// This include file allows data to be stored in program space.  The
// ATmega168 has 16k of program space compared to 1k of RAM, so large
// pieces of static data should be stored in program space.
#include <avr/pgmspace.h>

#else
#include "stubs.h"
#endif

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "follow_line.h"

/* Speed constants. */
//const int           SpeedLeft = 50;
//const int           SpeedRight = 50;
const int           SpeedLeft = 30;
const int           SpeedRight = 30;
static int i = 0;

void clear_integral()
{
  i = 0;
}

void follow_line(signed int l)
{
  int p;
  int d;
  static int prev_p = 0;
  int motorDelta;
   
	p = l-2000;
    i += p;
	d = p - prev_p;
	prev_p = p;

	motorDelta = p / 150 + i / 5000;// + d * 3/2;
	//motorDelta = p / 100 + i / 10000 ;
	set_m1_speed(SpeedLeft + motorDelta);
    set_m2_speed(SpeedRight - motorDelta);
}

