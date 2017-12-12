/*
 * follow_line_2.c
 *
 * Created: 2017-12-05 19:28:08
 *  Author: A243403
 */ 
#define AVR_3PI  (1)

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

//#include "utils.h"

#include "follow_line_2.h"

/* Speed constants. */
//const int           SpeedLeft2 = 50;
//const int           SpeedRight2 = 50;

static int i = 0;
const int cont_treshold = 500; //kalib param
static int base_speed = 0;
static int k_p_inv = 0;
static int k_i_inv = 0;

void follow_line_2_init(int speed, int p_gain_inv, int i_gain_inv)
{
	base_speed = speed;
	k_p_inv = p_gain_inv;
	k_i_inv = i_gain_inv;
	i = 0;
}

void follow_line_2(void)
{
	int p;
	int motorDelta;
	unsigned int values[5];
	
	(void)read_line(values,IR_EMITTERS_ON);

	//Sensors numebered from left to right
	if( (values[3]>500) && values[1]>500 ){
		p = values[4]-values[0];
	}
	else {
		p = values[3]-values[1];		
	}

	i += p;

	motorDelta = p / k_p_inv + i / k_i_inv;
	set_m1_speed(base_speed + motorDelta);
	set_m2_speed(base_speed);
}