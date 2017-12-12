/* 3pi_app1 - an application for the Pololu 3pi Robot
 *
 * This application uses the Pololu AVR C/C++ Library.  For help, see:
 * -User's guide: http://www.pololu.com/docs/0J20
 * -Command reference: http://www.pololu.com/docs/0J18
 *
 * Created: 7/19/2017 7:53:30 AM
 *  Author: yt71905
 */

#include <pololu/3pi.h>
#include <stdio.h>
#include "my_types.h"
#include "calibration.h"
#include "follow_line.h"
#include "follow_line_2.h"
#include "position.h"
#include "readBarcode.h"
#include "readSH.h"

#define MAX_NBR_OF_LOOPS_IN_90_TURN 150
#define MAX_NBR_OF_LOOPS_IN_180_TURN 450

void print_and_wait_for_button_c(uint16_t tmp);
void print_press_b_and_wait_for_button_b(uint16_t delay);
void print_x_y_and_wait_for_button_c(void);
void print_finnish_and_wait_for_button_c(void);
void print_x_y_visiting_stakeholder(void);
void print_dest_and_stakeholder(void);

uint8_t find_crossing(uint16_t* values);
uint16_t read_line_and_inc(uint16_t* values);
void move_forward(uint16_t nbr_of_loops);
void read_bar_code(void);

void turn_right(void);
void turn_left(void);
void turn_back(void);
void turn_north(void);
void turn_south(void);
void turn_west(void);
void turn_east(void);
void turn_to(uint8_t req_dir);
void pass_crossing(void);
int8_t get_diff_in_x_pos(void);
int8_t get_diff_in_y_pos(void);
void pass_or_turn_in_crossing(void);
void set_next_x_y_pos_based_on_id(uint8_t id);

void go_west(void);
void go_east(void);
void go_north(void);
void go_south(void);
void visit_next_destination(void);
void visit_stakeholder(void);
boolean visit_station(void);
void go_to_next_x_pos(void);
void go_to_next_y_pos(void);
void go_to_next_pos(uint8_t pos, uint8_t next_pos, boolean is_out_of_bounds);
void read_line_and_go_to_next_cross(void);
void go_x_direction(void);
void go_y_direction(void);
void add_place_visited(uint8_t id);
void add_stakeholder_visisted(uint8_t sh_id);
void update_visited_places(char* new_char, uint8_t char_length);
boolean pos_and_next_pos_ok_before_moving_on(void);


typedef enum move_states{forward,t_right,t_left,t_back,stop} move_states;
static uint32_t nbr_of_loops = 0;
static char visited_places[30];
static uint8_t nbr_of_visited_places = 0;
//static uint8_t places_to_visit[3] = {3,10,14};
//static uint8_t next_index_to_visit = 0;

static void init(void){
	
	// Magic command to initialize the sensors.
	pololu_3pi_init(2000);
	set_analog_mode(MODE_10_BIT);
	lcd_init_printf();
	init_position();
	
	// Print a message on the LCD.
	clear();
	int x = read_battery_millivolts_3pi();
	
	print_and_wait_for_button_c(x);
	
	// Start the sensor calibration. Needed to adapt values to ambient light, etc...
	calibrate_sensors();
}


// main is a function and is the brain of everything. That is where everything starts.
int main()
{
	// Initialize the robot.
	init();
	boolean back_at_home = false;
	// Wait for the B button to be pushed and then clear the LCD message
	print_press_b_and_wait_for_button_b(0);
	//set_next_stakeholder_id(1);
	while (!back_at_home){
		//if(has_stakeholder_to_visit()){
			//print_x_y_and_wait_for_button_c();
			//visit_stakeholder();
			//read_line_and_go_to_next_cross();
		//}
		back_at_home = visit_station();
		if(!back_at_home)
		{
			read_bar_code();
			print_dest_and_stakeholder();
			read_line_and_go_to_next_cross();	
		}else{
			move_forward(200);
		}
		
	}
	clear();
	print_finnish_and_wait_for_button_c();
	return 0;
}

boolean visit_station(void)
{
	uint8_t id = get_next_station_id();
	set_next_x_y_pos_based_on_id(id);
	visit_next_destination();
	move_forward(200);
	add_place_visited(id); //Update the text that is shown at the end
	return is_back_home();
}

/*Test function! To be filled with PSPs stuff*/
void read_bar_code(void)
{
	uint8_t station_id, sh_id;
	delay(200);
	init_read_bar_code();
	readBarcode(&station_id, &sh_id);
	set_next_station_id(station_id);
	if(sh_id>0 && sh_id <=5)
	{
		set_next_stakeholder_id(sh_id);	
	}
/*	uint8_t x,y;
	if(next_index_to_visit>sizeof(places_to_visit)){
		x = get_home_x_pos();
		y = get_home_y_pos();	
		set_next_stop_x_pos(x);
		set_next_stop_y_pos(y);	
	}else{
		uint8_t id = places_to_visit[next_index_to_visit];
		set_next_station_id(id);
		set_next_x_y_pos_based_on_id(id);
		next_index_to_visit++;
	}*/
}

void set_next_x_y_pos_based_on_id(uint8_t id)
{
	uint8_t x,y;
	x = get_x_position_for_id(id);
	y = get_y_position_for_id(id);	
	set_next_stop_x_pos(x);
	set_next_stop_y_pos(y);
}


void set_next_stakeholder_x_y_based_on_id(uint8_t id)
{
	uint8_t x,y;
	x = get_x_pos_for_stakeholder_based_on_id(id);
	y = get_y_pos_for_stakeholder_based_on_id(id);
	set_stake_holder_x_pos(x);
	set_stake_holder_y_pos(y);
}

void visit_stakeholder(void)
{
	uint8_t sh_id = get_next_stakeholder_id();
	set_next_stakeholder_x_y_based_on_id(sh_id);
	uint8_t tmp_x = get_next_stop_x_pos();
	uint8_t tmp_y = get_next_stop_y_pos();
	uint8_t sh_name;
	set_next_stop_x_pos((uint8_t)get_stakeholder_x_pos());
	set_next_stop_y_pos((uint8_t)get_stakeholder_y_pos());
	visit_next_destination();
	//move_forward(50);
	readSH(&sh_name);
	add_stakeholder_visisted(sh_name);
	stake_holder_visited();
	set_next_stop_x_pos(tmp_x);
	set_next_stop_y_pos(tmp_y);	
}

void move_forward(uint16_t nbr_of_loops)
{
	uint16_t values[5];
	uint16_t line;
	uint16_t x = 0;
	while (x <= nbr_of_loops)
	{
		line = read_line_and_inc(values);
		follow_line(line);
		x++;
	}	
	fl_stop();
}

void visit_next_destination(void)
{
	if(pos_and_next_pos_ok_before_moving_on())
	{
				//print_x_y_and_wait_for_button_c();
		while(get_diff_in_x_pos() != 0 || get_diff_in_y_pos() != 0)
		{
			//print_x_y_and_wait_for_button_c();
			if(get_x_pos()%2==0)
			{
				go_x_direction();
			}
			else if(get_y_pos()%2==0)
			{
				go_y_direction();
			}	
			else//You are in a CROSSING
			{
				if(is_next_x_out_of_bounds() && get_y_pos() != get_next_stop_y_pos())
				{
					go_y_direction();
				}
				else if (is_next_y_out_of_bounds() && get_x_pos() != get_next_stop_x_pos())
				{
					go_x_direction();
				}
				else
				{ 
					if(get_x_pos() != get_next_stop_x_pos())
					{
						go_x_direction();
					}
					else
					{
						go_y_direction();
					}
				}
			}
		}
		fl_stop();
	}
}

boolean pos_and_next_pos_ok_before_moving_on(void)
{
	/*If x or y are out of bounds, we must move to the other direction first!*/
	if(get_next_stop_y_pos()%2 == 0 && get_next_stop_x_pos()%2==0){
		clear();
		printf("Cant go there!");
		fl_stop();
		return false;
	}
	else if (get_x_pos()%2 == 0 && get_y_pos()%2 == 0){
		clear();
		printf("Cant be here!");
		fl_stop();
		return false;
	}
	return true;
}

void go_x_direction(void)
{
	int8_t diff_in_x_pos = get_diff_in_x_pos();
	if (diff_in_x_pos > 0)
	{
		go_west();
	}
	else if (diff_in_x_pos < 0)
	{
		go_east();
	}
}

void go_y_direction(void)
{
	int8_t diff_in_y_pos = get_diff_in_y_pos();
	if (diff_in_y_pos > 0)
	{
		go_south();
	}
	else if (diff_in_y_pos < 0)
	{
		go_north();
	}
}

void go_west(void)
{
	turn_west();
	go_to_next_x_pos();	
}

void go_east(void)
{
	turn_east();
	go_to_next_x_pos();	
}

void go_north(void)
{
	turn_north();
	go_to_next_y_pos();
}

void go_south(void){
	turn_south();
	go_to_next_y_pos();	
}

void go_to_next_x_pos(void)
{
	if(is_inside_crossing() && get_x_pos() != get_next_stop_x_pos())
	{
		pass_or_turn_in_crossing();
	}
	read_line_and_go_to_next_cross();
	if(is_inside_crossing() && get_x_pos() != get_next_stop_x_pos())
	{
		pass_or_turn_in_crossing();
	}
}

void go_to_next_y_pos(void)
{
	if(is_inside_crossing() && get_y_pos() != get_next_stop_y_pos() )
	{
		pass_or_turn_in_crossing();
	}
	read_line_and_go_to_next_cross();
	if(is_inside_crossing() && get_y_pos() != get_next_stop_y_pos() )
	{
		pass_or_turn_in_crossing();
	}
}

void pass_or_turn_in_crossing(void)
{
	if(get_req_dir() == get_direction())
	{
		pass_crossing();
		}else{
		turn_to(get_req_dir());
	}
}

void read_line_and_go_to_next_cross(void)
{
	uint16_t values[5];
	uint16_t line;
	boolean crossing_found = false;
	while (!is_x_pos_out_of_bounds() && !is_y_pos_out_of_bounds() && !crossing_found)
	{
		line = read_line_and_inc(values);
		follow_line(line);
		crossing_found = find_crossing(values);
		if(crossing_found){
			crossing_entered();
		}
	}	
	print_x_y_and_wait_for_button_c();
}


/*west = 3*/
void turn_west(void)
{
	set_req_dir(3);
	if(is_inside_crossing())
	{
		turn_to(get_req_dir());
	}
}

/*East = 1*/
void turn_east(void)
{
	set_req_dir(1);
	if(is_inside_crossing())
	{
		turn_to(get_req_dir());
	}
}

/*North = 0*/
void turn_north(void)
{
	set_req_dir(0);
	if(is_inside_crossing())
	{
		turn_to(get_req_dir());
	}
}
/*South = 2*/
void turn_south(void)
{
	set_req_dir(2);
	if(is_inside_crossing())
	{
		turn_to(get_req_dir());
	}
}

void turn_to(uint8_t req_dir)
{
	uint8_t tmp = (4 + req_dir - get_direction()) % 4;
	switch(tmp)
	{
		case(0):
		default:
		//do nothing
			break;
		case(1):
			turn_right();
			break;
		case(2):
			turn_back();
			break;
		case(3):
			turn_left();
			break;
	}
}

void pass_crossing(void)
{
	crossing_exited();	
}

void turn_back(void)
{	
	if(is_y_pos_out_of_bounds() || is_x_pos_out_of_bounds()){
		uint16_t max_nbr_of_loops = 0;
		uint16_t values[5];
		while(max_nbr_of_loops < MAX_NBR_OF_LOOPS_IN_180_TURN)
		{
			read_line_and_inc(values);
			fl_turn_180();
			max_nbr_of_loops++;
		}
		turn_back_completed();
		fl_stop();
	}
}

void turn_right(void)
{
	uint16_t max_nbr_of_loops = 0;
	uint16_t values[5];
	while(max_nbr_of_loops < MAX_NBR_OF_LOOPS_IN_90_TURN)
	{	
		read_line_and_inc(values);
		fl_turn_right();
		max_nbr_of_loops++;
	}
	right_turn_completed();
}

void turn_left(void)
{
	uint16_t max_nbr_of_loops = 0;
	uint16_t values[5];
	while(max_nbr_of_loops < MAX_NBR_OF_LOOPS_IN_90_TURN)
	{
		read_line_and_inc(values);
		fl_turn_left();
		max_nbr_of_loops++;
	}
	left_turn_completed();
}

uint16_t read_line_and_inc(uint16_t* values)
{	
	nbr_of_loops = nbr_of_loops + 1;
	return read_line(values, IR_EMITTERS_ON);
}

boolean find_crossing(uint16_t* values)
{
	static uint8_t count_nbr_of_sens_3k = 0;
	uint16_t sum_of_mid_sensors = values[1]+values[2]+values[3];
	uint16_t sum_of_outer_sensors = values[0]+values[4];
	if(!is_x_pos_out_of_bounds() && !is_y_pos_out_of_bounds())
	{
		if (sum_of_mid_sensors == 3000 && sum_of_outer_sensors>700){
			count_nbr_of_sens_3k++;
		}
		if(count_nbr_of_sens_3k > 10){
			count_nbr_of_sens_3k = 0;
			return true;
		}
	}
	return false;
}

void print_and_wait_for_button_c(uint16_t tmp)
{
	clear();
	printf("PC %d",tmp);
	fl_stop();
	wait_for_button(BUTTON_C);
	clear();	
}

void print_press_b_and_wait_for_button_b(uint16_t delay)
{
	clear();
	print("Press B!");
	wait_for_button(BUTTON_B);
	delay_ms(delay);
	clear();
}

void print_x_y_visiting_stakeholder(void)
{
	clear();
	fl_stop();
	printf("SH:n%d,n%d,d%d",get_next_stop_x_pos(),get_next_stop_y_pos(),get_direction());
	wait_for_button(BUTTON_C);
	clear();
}

void print_x_y_and_wait_for_button_c(void)
{
	clear();
	fl_stop();
	printf("%d,%d,%d,%d,%d,%d",get_x_pos(),get_y_pos(),get_next_stop_x_pos(),get_next_stop_y_pos(),get_next_station_id(),get_next_stakeholder_id());
	wait_for_button(BUTTON_C);
	clear();
}

/*Addr MUST be in "01" format! */
void add_place_visited(uint8_t id)
{
	char addr[3];
	if(id < 10)
	{
		sprintf(addr,"0%d,",id);
	}
	else
	{
		sprintf(addr,"%d,",id);
	}
	update_visited_places(addr,sizeof(addr));
}

/**/
void add_stakeholder_visisted(uint8_t sh_id)
{
	char addr[3];
	sprintf(addr,"#%d,",sh_id);
	update_visited_places(addr,sizeof(addr));
}

void update_visited_places(char* new_char, uint8_t char_length)
{
	uint8_t i = 0;
	while(nbr_of_visited_places < sizeof(visited_places) && i<char_length)
	{
		visited_places[nbr_of_visited_places] = new_char[i];
		nbr_of_visited_places++;
		i++;
	}	
}

void print_finnish_and_wait_for_button_c(void)
{
	clear();
	fl_stop();
	printf("FIN:%s",visited_places);
	wait_for_button(BUTTON_C);
}

int8_t get_diff_in_x_pos(void)
{
	return get_x_pos()-get_next_stop_x_pos();
}

int8_t get_diff_in_y_pos(void)
{
	return get_y_pos()-get_next_stop_y_pos();
}

void print_dest_and_stakeholder(void)
{
	clear();
	printf("dest:%d\nsh:%d", get_next_station_id(), get_next_stakeholder_id());
}
void print_and_wait_for_button_a(uint16_t a,uint16_t b)
{
	clear();
	printf("%d\n%d",a,b);
	wait_for_button(BUTTON_A);
}