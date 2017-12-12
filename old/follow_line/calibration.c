
/*
 * calibration.c
 * 
 * 
 * 
 */

#include "calibration.h"
#include "follow_line.h"
#include <pololu/3pi.h>
#include <avr/pgmspace.h>

// Calibration done sound
const char go[] PROGMEM = "L16 cdegreg4";
/* Speed constants. */
const int           LowSpeedLeft   = 50;
const int           LowSpeedRight  = 50;

extern void calibrate (){
  unsigned int counter; // used as a simple timer
  
  // Calibrate the sensors. 
  for(counter=0;counter<80;counter++)
    {
        if(counter < 20 || counter >= 60)
            set_motors(40,-40);
        else
            set_motors(-40,40);
 
        // This function records a set of sensor readings and keeps
        // track of the minimum and maximum values encountered.  The
        // IR_EMITTERS_ON argument means that the IR LEDs will be
        // turned on during the reading, which is usually what you
        // want.
        calibrate_line_sensors(IR_EMITTERS_ON);
 
        // Since our counter runs to 80, the total delay will be
        // 80*20 = 1600 ms.
        delay_ms(20);
    }
    // Stop the motors
    set_motors(0,0);
    
    // Play music and wait for it to finish before we start driving.
    play_from_program_space(go);
    while(is_playing());
}


/* This function handle the calibration of the line sensors. */
void calibrate_sensors(void)
{
	int i;
	for (i = 0; i<10;i++)
	{
		set_m1_speed(-LowSpeedLeft);
		set_m2_speed(LowSpeedRight);
		calibrate_line_sensors(IR_EMITTERS_ON);
		delay_ms(10);
	}
	fl_stop();
	for (i = 0; i<20;i++)
	{
		set_m1_speed(LowSpeedLeft);
		set_m2_speed(-LowSpeedRight);
		calibrate_line_sensors(IR_EMITTERS_ON);
		delay_ms(10);
	}
	fl_stop();
	for (i = 0; i<10;i++)
	{
		set_m1_speed(-LowSpeedLeft);
		set_m2_speed(LowSpeedRight);
		calibrate_line_sensors(IR_EMITTERS_ON);
		delay_ms(10);
	}
	fl_stop();
}