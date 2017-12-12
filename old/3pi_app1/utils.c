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
#include <stdlib.h>

const char beep[] PROGMEM = "!L16 V8 cde";
const char aaa[] PROGMEM = "!L16 V8 aaa";
const char ccc[] PROGMEM = "!L16 V8 ccc";
const char fff[] PROGMEM = "!L16 V8 fff";

void play_beep(void)
{
  play_from_program_space(beep);
}

void play_c(void)
{
  play_from_program_space(ccc);
}

void play_a(void)
{
  play_from_program_space(aaa);
}

void play_f(void)
{
  play_from_program_space(fff);
}
  
/* Stop robot. */
void stop(void){
	set_m1_speed(0);
  set_m2_speed(0);
  delay_ms(100);
}
