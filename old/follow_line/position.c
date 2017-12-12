#include "position.h"


/*UPDATE THESE BEFORE START!*/
#define NBR_OF_VERTICAL_LINES 4
#define NBR_OF_HORISONTAL_LINES 3

static const uint8_t all_stakeholders_x [5] = {3,8,10,6,3};
static const uint8_t all_stakeholders_y[5] = {6,7,3,1,2};

/*CALIBRATION END*/

#define	TOT_NBR_OF_LINES (NBR_OF_HORISONTAL_LINES + NBR_OF_VERTICAL_LINES)

static const uint8_t MAX_Y = NBR_OF_HORISONTAL_LINES * 2;
static const uint8_t MAX_X = NBR_OF_VERTICAL_LINES * 2;
static uint8_t starting_pos_y;
static uint8_t starting_pos_x;
static uint8_t x_pos;
static uint8_t y_pos;
static uint8_t next_stop_x_pos;
static uint8_t next_stop_y_pos;
static int8_t stakeholder_x_pos; //Always the case since the we don't have any stakeholders before the first stop
static int8_t stakeholder_y_pos; //Always the case since the we don't have any stakeholders before the first stop
static uint8_t next_station_id;
static uint8_t next_sh_id;
static int8_t req_direction = 1; /*-1 -> No turn. 0= north, 1 =east ...*/

static uint8_t dir_num = 1; //North = 0, East = 1, South = 2; West = 3;
static boolean crossing_found_last_loop = false;

void update_position(void);
void update_direction_in_right_turn(void);
void update_direction_in_left_turn(void);

boolean is_id_on_the_top(uint8_t id);
boolean is_id_on_right_side(uint8_t id);
boolean is_id_on_left_side(uint8_t id);
boolean is_id_on_the_botton(uint8_t id);
boolean heading_west(void);
boolean heading_east(void);
boolean heading_south(void);
boolean heading_north(void);

void init_position(void)
{
	starting_pos_y = MAX_Y-1;
	starting_pos_x = 0;
	x_pos = starting_pos_x;
	y_pos = starting_pos_y;
	dir_num = 1; //East!
	next_stop_x_pos = MAX_X;
	next_stop_y_pos = MAX_Y - 1;
	stakeholder_x_pos = -1;
	stakeholder_y_pos = -1;
	next_sh_id = 0;
	next_station_id = NBR_OF_VERTICAL_LINES+1;
}

uint8_t get_next_station_id(void)
{
	return next_station_id;	
}

void set_next_station_id(uint8_t id)
{
	next_station_id = id;
}

uint8_t get_next_stakeholder_id(void)
{
	return next_sh_id;
}

void set_next_stakeholder_id(uint8_t sh_id)
{
	next_sh_id = sh_id;	
}

int8_t get_stakeholder_x_pos(void)
{
	return stakeholder_x_pos;
}

int8_t get_stakeholder_y_pos(void)
{
	return stakeholder_y_pos;
}

void stake_holder_visited(void)
{
	stakeholder_x_pos = -1;
	stakeholder_y_pos = -1;
	set_next_stakeholder_id(0);
}

void set_stake_holder_x_pos(uint8_t x_pos)
{
	stakeholder_x_pos = (uint8_t)x_pos;
}

void set_stake_holder_y_pos(uint8_t y_pos)
{
	stakeholder_y_pos = (uint8_t)y_pos;
}

uint8_t get_x_pos(void)
{
	return x_pos;
}

uint8_t get_y_pos(void)	
{
	return y_pos;
}

uint8_t get_next_stop_x_pos(void)
{
	return next_stop_x_pos;
}

uint8_t get_next_stop_y_pos(void)
{
	return next_stop_y_pos;
}

void set_next_stop_x_pos(uint8_t next_stop_x_pos_)
{
	next_stop_x_pos = next_stop_x_pos_;
}

void set_next_stop_y_pos(uint8_t next_stop_y_pos_)
{
	next_stop_y_pos = next_stop_y_pos_;
}

void crossing_entered(void)
{
	update_position();
	crossing_found_last_loop = true;
}

void crossing_exited(void)
{
	update_position();
	crossing_found_last_loop = false;
}

void update_position(void)
{
	switch(dir_num){
		case(0): //North
			y_pos = y_pos + 1;
			break;
		case(1): //East
			x_pos = x_pos + 1;
			break;
		case(2): //South
			y_pos = y_pos - 1;
			break;
		case(3): //West
			x_pos = x_pos - 1;
			break;
		default:
		//Should not end up in here!
		break;
	}
}

void right_turn_completed(void)
{
	update_direction_in_right_turn();
	crossing_exited();
}

void left_turn_completed(void)
{
	update_direction_in_left_turn();
	crossing_exited();
}

/* We are not supposed to turn back in a CROSSING. That's why the position is not updated!*/
void turn_back_completed(void)
{
	update_direction_in_left_turn();
	update_direction_in_left_turn();		
}

/*North = 0 -> (0 + 1) % 4 = 1 -> East*/
void update_direction_in_right_turn(void)
{
	dir_num = (dir_num + 1) % 4;
}

/*North = 0 -> (0 + 4 - 1) % 4 = 3 -> West */
void update_direction_in_left_turn(void)
{
	dir_num = (dir_num + 4 - 1) % 4;
}

boolean is_inside_crossing(void)
{
	return crossing_found_last_loop;
}

boolean is_x_pos_out_of_bounds(void)
{
	if((x_pos > (MAX_X-1) && dir_num == 1) || (x_pos < 1 && dir_num == 3))
	{
		return true;	
	}
	return false;
}


boolean is_y_pos_out_of_bounds(void)
{
	if((y_pos > (MAX_Y-1) && dir_num == 0) || (y_pos < 1 && dir_num == 2))
	{
		return true;
	}
	return false;
}

boolean is_next_x_out_of_bounds(void)
{
	if(next_stop_x_pos > MAX_X-1 || next_stop_x_pos <1)	
	{
		return true;
	}
	return false;
}

boolean is_next_y_out_of_bounds(void)
{
	if(next_stop_y_pos > MAX_Y-1 || next_stop_y_pos <1)
	{
		return true;
	}
	return false;
}

boolean is_the_last_row_passed(void)
{
	return y_pos > (MAX_Y -1);
}

uint8_t get_direction(void)
{
	return dir_num;
}

boolean is_back_home(void)
{
	if(get_x_pos() == starting_pos_x && get_y_pos() == starting_pos_y )
	{
		return true;
	}
	return false;
}

boolean has_stakeholder_to_visit(void)
{
	return get_next_stakeholder_id() > 0 && get_next_stakeholder_id()<=sizeof(all_stakeholders_x);	
}

uint8_t get_x_position_for_id(uint8_t id)
{
	if(is_id_on_the_top(id))
	{
		return (uint8_t)(id*2)-1;
	}
	else if(is_id_on_right_side(id))
	{
		return MAX_X;	
	}
	else if(is_id_on_the_botton(id))
	{	
		return (TOT_NBR_OF_LINES+NBR_OF_VERTICAL_LINES-id)*2+1;
	}
	else if(is_id_on_left_side(id))
	{
		return 0;
	}
	//SHOULD NOT END UP IN HERE!
	return 0;
}

uint8_t get_y_position_for_id(uint8_t id)
{
	if(is_id_on_the_top(id))
	{
		return MAX_Y;
	}
	else if(is_id_on_right_side(id))
	{
		return (TOT_NBR_OF_LINES-id)*2+1;
	}
	else if(is_id_on_the_botton(id))
	{
		return 0;
	}
	else if(is_id_on_left_side(id))
	{
		return 	2*(id-TOT_NBR_OF_LINES-NBR_OF_VERTICAL_LINES)-1;
	}
	//SHOULD NOT END UP IN HERE!
	return 0;	
}

boolean is_id_on_the_top(uint8_t id)
{
	if(id>0 && id <= NBR_OF_VERTICAL_LINES)
	{
		return true;
	}
	return false;
}

boolean is_id_on_right_side(uint8_t id)
{
	if(id>NBR_OF_VERTICAL_LINES && id <= NBR_OF_VERTICAL_LINES+NBR_OF_HORISONTAL_LINES)
	{
		return true;
	}
	return false;
}

boolean is_id_on_left_side(uint8_t id)
{
	if (id>(2*NBR_OF_VERTICAL_LINES)+NBR_OF_HORISONTAL_LINES)
	{
		return true;
	}
	return false;
}

boolean is_id_on_the_botton(uint8_t id)
{
	if(id>TOT_NBR_OF_LINES && id <= NBR_OF_HORISONTAL_LINES+NBR_OF_VERTICAL_LINES*2)
	{
		return true;
	}
	return false;
}

uint8_t get_home_x_pos(void)
{
	return 0;
}

uint8_t get_home_y_pos(void)
{
	return MAX_Y-1;
}

uint8_t get_x_pos_for_stakeholder_based_on_id(uint8_t id)
{
	if(id <= sizeof(all_stakeholders_x))
	{
		return all_stakeholders_x[id-1];
	}
	return 0;
}

uint8_t get_y_pos_for_stakeholder_based_on_id(uint8_t id)
{
	if(id <= sizeof(all_stakeholders_y))
	{
		return all_stakeholders_y[id-1];
	}
	return 0;
}

boolean any_crossings_ahead_x(void)
{
	if((heading_east() && get_x_pos() >= MAX_X-1 ) || (heading_west() && get_x_pos() <= 1)){
		return false;
	}
	return true;
}

boolean any_crossings_ahead_y(void)
{
	if((heading_north() && get_y_pos()<=1) || (heading_south() && get_y_pos()>=MAX_Y-1))
	{
		return false;
	}
	return true;
}

boolean any_crossings_in_dir(uint8_t dir)
{
	switch(dir){
		case(0):
			return get_y_pos()<MAX_Y-1;
			break;
		case(1): 
			return get_x_pos()<MAX_X-1;
			break;
		case(2):
			return get_y_pos()>1;
			break;
		case(3):
			return get_x_pos()>1;
			break;
	}
	return false; // SHOULD NOT BE HERE!
}

void set_req_dir(uint8_t req_dir)
{
	req_direction = req_dir;
}

uint8_t get_req_dir(void)
{
	return req_direction;
}

boolean heading_west(void)
{
	return (dir_num == 3 || req_direction == 3);
}

boolean heading_east(void)
{
	return (dir_num == 1 || req_direction == 1);
}

boolean heading_south(void)
{
	return (dir_num == 2 || req_direction == 2);
}

boolean heading_north(void)
{
	return (dir_num == 0 || req_direction == 0);
}