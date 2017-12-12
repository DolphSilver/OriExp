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
const int           SpeedLeft = 60;
const int           SpeedRight = 60;
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

	motorDelta = p / 30 + i / 10000 + d * 3/2;
	set_m1_speed(SpeedLeft + motorDelta);
    set_m2_speed(SpeedRight - motorDelta);
}

void fl_stop(void)
{
	set_m1_speed(0);
	set_m2_speed(0);
}

void fl_turn_right(void)
{
	follow_line(3800);
}

void fl_turn_left(void)
{
	follow_line(200);
}

void fl_turn_180(void)
{		
	int m1 = 50, m2 = 50;
	set_m1_speed(m1);
	set_m2_speed(-m2);
}
