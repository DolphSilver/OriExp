/* OrientExpress - an application for the Pololu 3pi Robot
 *
 * This application uses the Pololu AVR C/C++ Library.  For help, see:
 * -User's guide: http://www.pololu.com/docs/0J20
 * -Command reference: http://www.pololu.com/docs/0J18
 *
 * Created: 11/29/2017 5:32:06 PM
 *  Author: A243403
 */

#include <pololu/3pi.h>

#include "state.h"
#include "destinationList.h"

static void init(void);

int main()
{
	play_from_program_space(PSTR(">g32>>c32"));  // Play welcoming notes.

	while(1)
	{
		// Print battery voltage (in mV) on the LCD.
		clear();
		print_long(read_battery_millivolts_3pi());
		delay_ms(1000);  // Wait for 200 ms.
		
		// Initialize the robot.
		init();
		
		// Wait for the B button to be pushed and then clear the LCD message
		print("Press B");
		wait_for_button(BUTTON_B);
		clear();
		
		// Enter an endless loop. Do the function for ever.
		
		auto posStruct next_position;
		auto OBJ_TYPE objective;
		
		auto int lost_flag = 0;  //static?
		auto int finish_flag = 0;
		
		while (1){
			strategy(&next_position, &objective);
			lost_flag = move(next_position);
			if(lost_flag != 0){
				objective = NULL;
			}
			switch(objective){
				case READ_BARCODE:
					lost_flag = readBarcode();
				case READ_SH_CODE:
					lost_flag = readSHCode();
				case FINISHED:
					displayLogg(logg);
				default:
					;
			}
			if(finish_flag == 1){
				play_from_program_space(blabla)
				while(1){
					;
				}
			}
			if(lost_flag != 0){
				findStartPos();
				initializeState();
			}
		}
	}
}
static void strategy(int *next_pos, int *fin_flag){
	
}

static void init(void){
	// Initialize extern variables
	size_t i;
	for (i=0; i< ARRAYSIZE(TURN_SPEED_MAP_x){
		float v = (255/ARRAYSIZE(TURN_SPEED_MAP_x))*(i + 0.5);
		TURN_SPEED_MAP_x[i] = (int)v;
	}
	
	// Magic command to initialize the sensors.
	pololu_3pi_init(2000);
	
	// Print a message on the LCD.
	clear();
	print("Press B");
	
	// Wait for the B button to be pressed
	wait_for_button(BUTTON_B);
	// Once pressed, wait 500ms for you to remove your hand... Increase if you are a slow mover :-)
	delay_ms(500);
	
	// Remove the LCD message
	clear();
	
	// Start the sensor calibration. Needed to adapt values to ambient light, etc...
	calibrate();
	
	initializeState(); 
}
