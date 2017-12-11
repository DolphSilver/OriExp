/*
 * signals.c
 *
 *  Created on: 9 dec. 2017
 *      Author: perpettersson
 */

#include <stdint.h>

//%%% position_x
//Owner: Position estimation function (sensor fusion function) (writes to)
//Users: Move functions for closed loop control (reads)
static uint16_t position_x = 0;
void position_x_Get(uint16_t *pos_x){
	*pos_x = position_x;
}
void position_x_Set(uint16_t *pos_x){
	position_x = pos_x;
}

//%%% position_y
//Owner: Position estimation function (sensor fusion function) (writes)
//Users: Move functions for closed loop control (reads)
static uint16_t position_y = 0;
void position_y_Get(uint16_t *pos_y){
	*pos_y = position_y;
}
void position_y_Set(uint16_t *pos_y){
	position_y = pos_y;
}

//%%% orientation
//Owner: Position estimation function (sensor fusion function) (writes)
//Users: Move functions for closed loop control
static uint16_t orientation = 0;
void orientation_Get(uint16_t *orient){
	*orient = orientation;
}
void orientation_Set(uint16_t *orient){
	orientation = orient;
}

//%%% engine_speed_L
//Owner: Engine control function (writes)
// (Strategy -> Move (vehicle model) -> Engine Control)
//Users: Position estimation function
// (-> vehicle model -> position estimation)
static uint8_t engine_speed_L = 0;
void engine_speed_L_Get(uint8_t *e_spd_L){
	*e_spd_L = engine_speed_L;
}
void engine_speed_L_Set(uint8_t *e_spd_L){
	engine_speed_L = e_spd_L;
}

//%%% engine_speed_L
//Owner: Engine control function (writes)
// (Strategy -> Move (vehicle model) -> Engine Control)
//Users: Position estimation function
// (-> vehicle model -> position estimation)
static uint8_t engine_speed_R = 0;
void engine_speed_R_Get(uint8_t *e_spd_R){
	*e_spd_R = engine_speed_R;
}
void engine_speed_R_Set(uint8_t *e_spd_R){
	engine_speed_R = e_spd_R;
}

//%%%
//Owner: Global (writes)
//Users: Display function
static uint8_t disp_line_1[LINE_LENGTH];
void disp_line_1_Get(pointerType *dsp_l_1){
	*dsp_l_1 = &disp_line_1;
}
void engine_speed_R_Set(uint8_t *e_spd_R){
	engine_speed_R = e_spd_R;
}
