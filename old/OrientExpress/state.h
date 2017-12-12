/*
 * state.h
 *
 * Created: 2017-11-29 17:45:32
 *  Author: A243403
 */ 


#ifndef STATE_H_
#define STATE_H_

typedef struct {
	float pos_x;
	float pos_y;
} posStruct;

typedef struct {
	posStruct pos;
	float dir_rad;
} stateStruct;

void state_Get(stateStruct *value);
void state_Set(stateStruct *value);

void state_Get_pos(posStruct *value);
void state_Set_pos(posStruct *value);

void state_Get_pos_X(float *value);
void state_Set_pos_X(float *value);

void state_Get_pos_Y(float *value);
void state_Set_pos_Y(float *value);

void state_Get_dir_rad(float *value);
void state_Set_dir_rad(float *value);

void state_Get_dir_deg(float *value);
void state_Set_dir_deg(float *value);

void initializeState(void);

#endif /* STATE_H_ */