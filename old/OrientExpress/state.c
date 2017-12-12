/*
 * state.c
 *
 * Created: 2017-11-29 17:37:02
 *  Author: A243403
 */ 

#include <math.h>

#include "state.h"

static stateStruct state_object;

void state_Set(stateStruct *value){
	state_object = *value;
}
void state_Get(stateStruct *value){
	*value = state_object;
}
void state_Set_pos(posStruct *value){
	state_object.pos = *value;
}
void state_Get_pos(posStruct *value){
	*value = state_object.pos;
}
void state_Set_pos_X(float *value){
	state_object.pos.pos_x = *value;
}
void state_Get_pos_X(float *value){
	*value = state_object.pos.pos_x;
}
void state_Set_pos_Y(float *value){
	state_object.pos.pos_y = *value;
}
void state_Get_pos_Y(float *value){
	*value = state_object.pos.pos_y;
}
void state_Set_dir_rad(float *value){
	auto float dir_rad = *value;
	while(dir_rad > 2*M_PI){
		dir_rad -= 2*M_PI;
	}
	while(dir_rad < 0){
		dir_rad += 2*M_PI;
	}
	state_object.dir_rad = dir_rad;
}
void state_Get_dir_rad(float *value){
	*value = state_object.dir_rad;
}
void state_Set_dir_deg(float *value){
	auto float dir_deg = *value;
	auto float dir_rad = 2*M_PI*dir_deg/360;
	state_Set_dir_rad(&dir_rad) //Funkar detta, pekare..
}
void state_Get_dir_deg(float *value){
	auto float dir_rad = state_object.dir_rad;
	auto float dir_deg = 360*dir_rad/(2*M_PI); 
	*value = dir_deg;
}

void initializeState(void){
	posStruct initial_pos;
	initial_pos.pos_x = -0.1;
	initial_pos.pos_y = 2;
	stateStruct initial_state;
	initial_state.pos = initial_pos;
	initial_state.dir_rad = 0;
	state_Set(&initial_state);
}