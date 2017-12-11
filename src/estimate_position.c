/*
 * estimate_position.c
 *
 *  Created on: 10 dec. 2017
 *      Author: perpettersson
 */


#include "estimate_position.h"
#include "signals.h"

//variables used by process (behöver ej vara static?)
int8_t e_spd_L_last;
int8_t e_spd_R_last;
uint16_t t_last;

uint16_t pos_x_update;
uint16_t pos_y_update;
uint16_t orient_update;

uint8_t kernel_waiting = 0;
void estimate_position_waiting_Set(){
	kernel_waiting = 1;
}

void estimate_position_init(void){
	e_spd_L_last = 0;
	e_spd_R_last = 0;
	t_last = 0; //good?
	position_x_Get(pos_x_update);
	position_y_Get(pos_y_update);
	orientation_Get(orient_update);
}

void estimate_position_process(void){
	int8_t e_spd_L_new;
	int8_t e_spd_L_avg;
	int8_t e_spd_R_new;
	int8_t e_spd_R_avg;
	uint16_t t_new;
	uint16_t t_diff;
while(1){
	//Write last calculation to signal object and read new data for next calculation
	accessing_Set();
	position_x_Set(&pos_x_update);
	position_y_Set(&pos_y_update);
	orientation_Set(&orient_update)
	engine_speed_L_Get(&e_spd_L_new);
	engine_speed_R_Get(&e_spd_L_new);
	time_Get(&e_spd_L_new);
	accessing_Reset();
	if(kernel_waiting == 1){
		kernel_waiting = 0;
		return_to_kernel();
	};
	t_diff = t_new - t_last;
	if(t_diff == 0){
		;
	}
	else{ //perform task
		e_spd_L_avg = (e_spd_L_new-e_spd_L_last)/2;
		e_spd_R_avg = (e_spd_R_new-e_spd_R_last)/2;

	}
}
}

