/* Set the address based on the map below [ZERO INDEXED]
5 -|-|-|-|-
3 -|-|-|-|-
1 -|-|-|-|-
  012345678

All lines will be ODD
All spaces between lines will be EVEN

*/
#include "my_types.h"
#include <pololu/3pi.h>
#include <stdio.h>

void init_position(void);

void crossing_entered(void);
void crossing_exited(void);
void right_turn_completed(void);
void left_turn_completed(void);
void turn_back_completed(void);
boolean is_x_pos_out_of_bounds(void);
boolean is_y_pos_out_of_bounds(void);
uint8_t get_next_stop_x_pos(void);
uint8_t get_next_stop_y_pos(void);
uint8_t get_x_pos(void);
uint8_t get_y_pos(void);
uint8_t get_direction(void);
void set_next_stop_x_pos(uint8_t next_stop_x_pos_);
void set_next_stop_y_pos(uint8_t next_stop_y_pos_);
boolean is_back_home(void);
boolean has_stakeholder_to_visit(void);
int8_t get_stakeholder_x_pos(void);
int8_t get_stakeholder_y_pos(void);
void stake_holder_visited(void);
void set_stake_holder_x_pos(uint8_t x_pos);
void set_stake_holder_y_pos(uint8_t y_pos);
boolean is_inside_crossing(void);

uint8_t get_x_position_for_id(uint8_t id);
uint8_t get_y_position_for_id(uint8_t id);

uint8_t get_home_x_pos(void);
uint8_t get_home_y_pos(void);

boolean is_next_x_out_of_bounds(void);
boolean is_next_y_out_of_bounds(void);

uint8_t get_next_station_id(void);
void set_next_station_id(uint8_t id);

uint8_t get_next_stakeholder_id(void);
void set_next_stakeholder_id(uint8_t sh_id);

uint8_t get_x_pos_for_stakeholder_based_on_id(uint8_t id);
uint8_t get_y_pos_for_stakeholder_based_on_id(uint8_t id);

boolean any_crossings_ahead_x(void);
boolean any_crossings_ahead_y(void);

void set_req_dir(uint8_t req_dir);
uint8_t get_req_dir(void);
boolean any_crossings_in_dir(uint8_t dir);