/*
 * readBarcode.c
 *
 * Created: 2017-12-06 16:13:14
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
//#include "debug_print.h"
//#include "utils.h"
//#include "nodesLog.h"
#include "position.h"
#include "readBarcode.h"
#include "follow_line_2.h"
#include "follow_line.h"

#define MAX_NBR_OF_LOOPS_IN_180_TURN 450

static uint8_t lastByte = 0;					//last byte read from sensors
static uint8_t readByte = 0;					//current sensor reading
// Containers for output
uint8_t bc = 255; //barcode, 255 = error
uint8_t sh_bc = 255; //stakeholder barcode, 255 = no read
	
	enum STATE {
		INIT,
		READING_BC,
		FINISHED_BC,
		READING_SH,
		FINISHED_READING,
		FINISHED
	};
	static enum STATE curr_state = INIT;
	static enum STATE prev_state;
	
void init_read_bar_code(void)
{
	lastByte = 0;
	readByte = 0;
	curr_state = INIT;
}

void debugPrint(char *r1, char *r2){
	if(DEBUG_MODE){
		stop();
		print_two_messages(r1, r2);
		delay_ms(1000);
		clear();
	}
}

void readSensorByte(){
/*
* Reads sensor values and...
*/
//Samling averaging
	uint16_t N_samp = N_SAMPLES_AVERAGE;	//number of samples to average over, max 64 if 16 bit...
	static uint8_t j = 0;							//itteration variable for sample averaging
// Definitions of valid sensor values
	const int allowedReads[6] = {SR_ON_LINE, SR_BC_START, SR_BC_STOP, SR_LEFT_BIT, SR_RIGHT_BIT};
	const int N_allowed = ARRAYSIZE(allowedReads);
	unsigned int values[5];					//container for sampled sensor values
	static unsigned int sens_sum[5] = {0, 0, 0, 0, 0}; //container for sum of sensor values over number of samples to average over
	const unsigned int treshold[5] = {N_samp*TH0, N_samp*TH1, N_samp*TH2, N_samp*TH1, N_samp*TH0}; //tresholds for different sensors
//general
	uint8_t mask;							//bit mask container
	uint8_t i;								//itteration variable
	uint8_t readByte_temp = 0;				//current sensor reading temporary
	
	//read sensor values
	(void)read_line(values,IR_EMITTERS_ON);
	
	//averaging
	for(i=0; i<5; i++){
		sens_sum[i] = sens_sum[i] + values[i];
	}
	j += 1;
	
	//Evaluate if valid
	if(j >= N_samp){ //if enough samples have been taken
	// transform to byte
		readByte_temp = 0;
		for(i=0; i<5; i++){
			if(sens_sum[i] > treshold[i]){
				mask = 128;
				mask >>= i;
				readByte_temp |= mask;
			}
			sens_sum[i] = 0;
		}
		j = 0;
		for(i=0; i < N_allowed; i++){
			if(readByte_temp == allowedReads[i]){
				readByte = readByte_temp;
			}
		}
	}
}


uint8_t eventAction(){
	//Sensor value transformation
	uint8_t mask;							//bit mask container
	uint16_t symbIdent = 0;					//container for sensor-read event combination
	
	//static uint8_t turn_flag = 1;
	
	//barcode reading
	static uint8_t bc_index = 0; //current index in barcode
	static uint8_t sh_bc_index = 0; //current index in stakeholder code
	
	static uint8_t dist_home = 0;

	
	//return value
	uint8_t finished_flag = 0;
	
	
	//check if new value added
	if(readByte == lastByte){
		; //if not do nothing
	}
	else{ //new byte: we have 2 unique -> evaluate if valid symbol read
		symbIdent = ((uint16_t)lastByte <<8) | readByte;
		lastByte = readByte;
		
		if(symbIdent == EV_BC_START){
			debugPrint("EV:", "BC_START");
			if(curr_state == INIT){
				//dist_home += 1; add istead  when moving to state reading bc... always return to same spot..	
			}
			else if(curr_state == READING_BC){
				prev_state = curr_state;
				curr_state = FINISHED_BC;
				dist_home += 1;
			}
			else if(curr_state == READING_SH){
				prev_state = curr_state;
				curr_state = FINISHED_READING;
				dist_home += 1; //one extra in to account for "fake read" due to turn
				//new_turn_back();
			}
			else if(curr_state == FINISHED_READING){
				dist_home -= 1;
				if(dist_home == 0){
					prev_state = curr_state;
					curr_state = FINISHED;
				}
			}
			else{
				;
			}
		}
		if(symbIdent == EV_BC_CONT){
			debugPrint("EV:", "BC_CONT");
			if(curr_state == INIT){
				prev_state = curr_state;
				curr_state = READING_BC;
				bc = 0;
				dist_home += 2; //one extra in order to always return robot at the same spot.
			}
			else if(curr_state == FINISHED_BC){
				prev_state = curr_state;
				curr_state = READING_SH;
				sh_bc = 0;
				dist_home += 1;
			}
			else if(curr_state == FINISHED_READING){
				dist_home -= 1;
				if(dist_home == 0){
					prev_state = curr_state;
					curr_state = FINISHED;
				}
			}
			else{
				;
			}
		}
		else if(symbIdent == EV_BC_STOP){
			debugPrint("EV:", "BC_STOP");
			bc_index = 0;
			sh_bc_index = 0;
			if(curr_state == FINISHED_BC){
				prev_state = curr_state;
				curr_state = FINISHED_READING;
				dist_home = 2; //account for skipping one when turning
			}
			else if(curr_state == FINISHED_READING)
			{
				prev_state = curr_state;
				curr_state = FINISHED_READING;
				dist_home = 4;
			}
			else{
				;
			}
			new_turn_back();
		}
		else if((symbIdent == EV_LEFT_BIT_HIGH_1) || (symbIdent == EV_LEFT_BIT_HIGH_2)){
			debugPrint("EV:", "LEFTBIT");
			if(curr_state == READING_BC){
				mask = 1;
				mask <<= bc_index;
				bc |= mask;
				bc_index += 1;
			}
			else if(curr_state == READING_SH){
				mask = 1;
				mask <<= sh_bc_index;
				sh_bc |= mask;
				sh_bc_index += 1;
			}
			else{
				;
			}
		}
		else if((symbIdent == EV_RIGHT_BIT_HIGH_1) || (symbIdent == EV_RIGHT_BIT_HIGH_2)){
			debugPrint("EV:", "RIGHTBIT");
			if(curr_state == READING_BC){
				bc_index += 1;
			}
			else if(curr_state == READING_SH){
				sh_bc_index += 1;
			}
			else{
				;
			}
		}
		else{
			debugPrint("EV:", "Fail");
			//wait_for_button(BUTTON_B);
			//printf("0:%03u\n", lastByte);
			//printf("1:%03u", readByte);
			//wait_for_button(BUTTON_B);
			//delay_ms(1000);
			//clear();
		}
		lastByte = readByte;
	}
	
	if(DEBUG_MODE_2){
		switch (curr_state){
			case INIT:
				clear();
				printf("INIT\n");
				printf("dh: %u", dist_home);
				break;
			case READING_BC:
				clear();
				printf("READ_BC\n");
				printf("dh: %u", dist_home);
			break;
			case FINISHED_BC:
				clear();
				printf("FIN_BC\n");
				printf("dh: %u", dist_home);
			break;
			case READING_SH:
				clear();
				printf("READ_SH\n");
				printf("dh: %u", dist_home);
			break;
			case FINISHED_READING:
				clear();
				printf("FIN_READ\n");
				printf("dh: %u", dist_home);
			break;
			case FINISHED:
				clear();
				printf("FIN\n");
				printf("dh: %u", dist_home);
			break;
		}
	}
	
	//if((curr_state == FINISHED_READING)){// && (turn_flag == 1)){
		//Call turn function
		/*stop();
		clear();
		printf("Turning..");
		wait_for_button(BUTTON_A);
		delay_ms(1000);
		clear();*/
		//new_turn_back();
		//turn_flag = 0;
	//}
	if(curr_state == FINISHED){
		finished_flag = 1;
	}
	return finished_flag;
}


uint8_t readBarcode(uint8_t *barcode, uint8_t *sh_code){

//General
	uint8_t finished_flag = 0;					//true if barcode reading finished

// Follow function parameters	
	int follow_speed = BC_FOLLOW_SPEED;
	int follow_p_gain_inv = BC_FOLLOW_P_GAIN_INV;
	int follow_i_gain_inv = BC_FOLLOW_I_GAIN_INV;
	
	uint8_t returnValue;
	
	follow_line_2_init(follow_speed, follow_p_gain_inv, follow_i_gain_inv);
	
	while(!finished_flag){
		//follow line 
		follow_line_2();
		//read sensors
		readSensorByte();
		//take action
		finished_flag = eventAction();
	}
	*barcode = bc;
	*sh_code = sh_bc;
	//check result
	if( (bc>0) && (bc<50) && (sh_bc>=0) && (sh_bc<5) ){
		returnValue = 0;
	}
	else{
		returnValue = 1;
	}
	return returnValue;
}
void new_turn_back(void);
void new_turn_back(void)
{
	uint16_t max_nbr_of_loops = 0;
	uint16_t values[5];
	while(max_nbr_of_loops < MAX_NBR_OF_LOOPS_IN_180_TURN)
	{
	read_line(values, IR_EMITTERS_ON);
		fl_turn_180();
		max_nbr_of_loops++;
	}
	turn_back_completed();
	fl_stop();
}