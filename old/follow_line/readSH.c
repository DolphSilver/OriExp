/*
 * readBC.c
 *
 * Created: 2017-12-07 08:50:40
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
#include "readSH.h"
#include "follow_line_2.h"

static uint8_t lastByteSH = 0;					//last byte read from sensors
static uint8_t readByteSH = 0;					//current sensor reading
// Containers for output
uint8_t sh_read = 255; //stakeholder, 255 = no read

void readSensorByteSH(){
	/*
	* Reads sensor values and...
	*/
	//Samling averaging
	uint16_t N_samp = SH_N_SAMPLES_AVERAGE;	//number of samples to average over, max 64 if 16 bit...
	static uint8_t j = 0;							//itteration variable for sample averaging
	// Definitions of valid sensor values
	
	//const int allowedReads[5] = {SR_ON_LINE, SR_SHX_1, SR_SHX_2, SR_SHX_3, SR_SHX_4};
	const int allowedReads[6] = {SR_ON_LINE, SR_SHX_1, SR_SHX_2, SR_SHX_3, SR_SHX_4, SR_SHX_5};
	
	const int N_allowed = ARRAYSIZE(allowedReads);
	unsigned int values[5];					//container for sampled sensor values
	static unsigned int sens_sum[5] = {0, 0, 0, 0, 0}; //container for sum of sensor values over number of samples to average over
	const unsigned int treshold[5] = {N_samp*SH_TH0, N_samp*SH_TH1, N_samp*SH_TH2, N_samp*SH_TH1, N_samp*SH_TH0}; //tresholds for different sensors
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
				readByteSH = readByte_temp;
			}
		}
	}
}

uint8_t eventActionSH(){
	//Sensor value transformation
	uint16_t symbIdent = 0;					//container for sensor-read event combination
	
	
	//reading
	static uint16_t read_store = 0;
	
	//return value
	uint8_t finished_flag = 0;
	
	
	//check if new value added
	if(readByteSH == lastByteSH){
		; //if not do nothing
	}
	else{ //new byte: we have 2 unique -> evaluate if valid symbol read
		
		//stop();
		//clear();
		//printf("p=%u\n", lastByteSH);
		//printf("c=%u", readByteSH);
		//wait_for_button(BUTTON_A);
		//delay_ms(1000);
		//clear();
		
		
		symbIdent = (read_store <<5) | (readByteSH>>3);
		read_store = symbIdent;
		symbIdent <<= 1; 
		lastByteSH = readByteSH;
		
		//stop();
		//clear();
		//printf("p=%u\n", readByteSH);
		//printf("c=%u", symbIdent);
		//wait_for_button(BUTTON_A);
		//delay_ms(1000);
		//clear();
		
		
		if((symbIdent == EV_SH_4_w) || (symbIdent == EV_SH_4_a)){
			sh_read = 4;
			//need further testing to determine 1 or 4 if a
			finished_flag = 1;
			read_store = 0;
		}
		else if(symbIdent == EV_SH_3){
			sh_read = 3;
			finished_flag = 1;
			read_store = 0;
		}
		else if(symbIdent == EV_SH_2){
			sh_read = 2;
			finished_flag = 1;
			read_store = 0;
		}
		else if((symbIdent == EV_SH_1_a) || (symbIdent == EV_SH_1_w1) || (symbIdent == EV_SH_1_w2) || (symbIdent == EV_SH_1_w3) || (symbIdent == EV_SH_1_w4) || (symbIdent == EV_SH_1_w5) || (symbIdent == EV_SH_1_w6)){
			sh_read = 1;
			finished_flag = 1;
			read_store = 0;
		}
		else{
			;
		}
	}
	return finished_flag;
}

void readSH(uint8_t *sh_nbr){

	//General
	uint8_t finished_flag = 0;					//true if barcode reading finished

	// Follow function parameters
	int follow_speed = SH_FOLLOW_SPEED;
	int follow_p_gain_inv = SH_FOLLOW_P_GAIN_INV;
	int follow_i_gain_inv = SH_FOLLOW_I_GAIN_INV;
	uint16_t loop_counter = 0;
	follow_line_2_init(follow_speed, follow_p_gain_inv, follow_i_gain_inv);
	
	while(!finished_flag || loop_counter <290){
		//follow line
		follow_line_2();
		//read sensors
		readSensorByteSH();
		//take action
		finished_flag = eventActionSH();
		loop_counter++;
	}
	
	*sh_nbr = sh_read;
}
