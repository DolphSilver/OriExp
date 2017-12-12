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
#include "orienteering.h"
#include "debug_print.h"
#include "utils.h"
#include "nodesLog.h"
//#include "events.h"
#include "readBarcode.h"
#include "follow_line_2.h"
#include "readSH.h"

// Initializes the 3pi, displays a welcome message, calibrates, and
// plays the initial music.
void initialize()
{
	// This must be called at the beginning of 3pi code, to set up the
	// sensors.  We use a value of 2000 for the timeout, which
	// corresponds to 2000*0.4 us = 0.8 ms on our 20 MHz processor.
	pololu_3pi_init(2000);
    set_analog_mode(MODE_10_BIT);
}

/* Speed constants. */
const int           LowSpeedLeft   = 50;
const int           LowSpeedRight  = 50;

/* This function handle the calibration of the line sensors. */
void calibrate_sensors(void)
{
	int i;
	for (i = 0; i<10;i++) 
	{
		set_m1_speed(-LowSpeedLeft);
		set_m2_speed(LowSpeedRight);
		calibrate_line_sensors(IR_EMITTERS_ON);
		delay_ms(10);
	}
	stop();
	for (i = 0; i<20;i++) 
	{
		set_m1_speed(LowSpeedLeft);
		set_m2_speed(-LowSpeedRight);
		calibrate_line_sensors(IR_EMITTERS_ON);
		delay_ms(10);
	} 
	stop();
	for (i = 0; i<10;i++) 
	{
		set_m1_speed(-LowSpeedLeft);
		set_m2_speed(LowSpeedRight);
		calibrate_line_sensors(IR_EMITTERS_ON);
		delay_ms(10);
	}
	stop();
}

unsigned char buttonPressed;
  
void startup(void)
{

  // set up the 3pi
  initialize();
	
  // Set up the LCD
  lcd_init_printf_with_dimensions(8,2);
  
  clear();
  lcd_goto_xy(0,0);
  print("Press");
  lcd_goto_xy(0,1);
  print("Button A");
  
  wait_for_button(BUTTON_A);

  delay_ms(200);
  // Calibrate the line sensors
  calibrate_sensors();
  // Set baud rate of the serial interface
  serial_set_baud_rate(115200);
  
  /* Indicate calibration completed. */
  play_beep();
  clear();
  lcd_goto_xy(0,0);
  print("Ready");
  lcd_goto_xy(0,1);
  print("Pres ABC");

  while(1)
  {
    buttonPressed = button_is_pressed(ANY_BUTTON);
    if(buttonPressed)
    {
      break;
    }
  }

  orienteering();
  
	// This part of the code is never reached.  A robot should
	// never reach the end of its program, or unpredictable behavior
	// will result as random code starts getting executed.  If you
	// really want to stop all actions at some point, set your motors
	// to 0,0 and run the following command to loop forever:
	//
  stop();
  while(1);
}

// -----------------------------------------------------------------
// -----------------------------------------------------------------

void run_orienteering(uint8_t stop_at_landmark)
{
  uint16_t line;
  uint16_t values[5];
	
  while(1)
  {
	/* This is a sample code  */
    line = read_line(values,IR_EMITTERS_ON);
	follow_line(line);
	/* End of sample code*/
	
	/****************************************************************************
	* Here you should add your code												*
	****************************************************************************/
	
	/**************************End of your code *********************************/
  }
}

uint16_t log_buffer[500];

void print_log()
{
  uint16_t i, temp_val;
  int8_t j;
  char print_buf[5];
  uint8_t digit;
  
  serial_send_blocking("  --- START ---\n", 16);
  
  for(i = 0; i < 500; i++)
  {
    temp_val = log_buffer[i]; 
	if((i % 5) == 0)
    {
      serial_send_blocking(" ", 1);
    }
    for(j = 3; j >= 0; j--)
    {
      digit = temp_val % 10; 
      if(temp_val == 0 && j != 3)
      {
        print_buf[j] = ' ';
      }
      else
      {
        print_buf[j] = '0' + digit;
      }
      temp_val /= 10;
    }
    serial_send_blocking(print_buf, 4);
    if((i % 5) == 4)
    {
      serial_send_blocking("\n", 1);
    }
    else
    {
      serial_send_blocking(", ", 2);
    }
  }
  
  serial_send_blocking("  --- END ---\n", 14);
  
}

void record_bonus_marker(void)
{
  uint16_t log_index = 0;
  uint8_t sensor_index;
  uint8_t start_logging = 0;
  uint16_t line;
  uint16_t values[5];
  uint32_t sample_T = 0;
  uint32_t prev_ticks = 0;

  delay_ms(1000);
  
  //lcd_init_printf();
  //while(1)
  //{
  //	  prev_ticks = get_ticks();
  //	  delay_ms(500);
  //	  sample_T = ticks_to_microseconds(get_ticks() - prev_ticks);
  //	  clear();
  //	  printf("T: %u", sample_T);
  //}

  clear_integral();
  while(1)
  {
    line = read_line(values,IR_EMITTERS_ON);
    follow_line(line);
	//follow_line_2();
	/*******************************************************************************************************************
	* Suggestion: You might want to add something here which consumes some time (similar to your intended orienteering *
	* code) otherwise the logging runs a bit fast                                                                      *
	********************************************************************************************************************/
    //if((values[1] > 350 && values[3] > 350) || (values[0] > 250 && values[4] > 250))
    //{
       start_logging = 1;
    //}
	sample_T = ticks_to_microseconds(get_ticks() - prev_ticks);
	if((start_logging) && (sample_T > 4000))
    {
	  prev_ticks = get_ticks();
      for(sensor_index = 0; sensor_index < 5; sensor_index++)
      {
        log_buffer[log_index++] = values[sensor_index];
      }
    }
    if(log_index >= 500)
    {
      print_two_messages("Buf full", "Press A");
      break;
    }
  }
    
  while(1)
  {
    if(button_is_pressed(BUTTON_A))
    {
      print_two_messages("Printing","");
      print_log();
      delay_ms(500);
      print_two_messages("Resend", "Press A");
    }
  }
}

void orienteering(void)
{

  if(buttonPressed == BUTTON_A)
  {
	/* Read the values measured by the sensors */
   /* When displayed "Press ABC" -> press button A to enter this function.
	  Put the robot over a black and white pattern.
	  Then press again button A to start the measurement.
	  The values measured by the most left two sensors will be displayed.
      Press B to view the values measured by the next two sensors.
      Press B again to see the value measured by the last sensor.
	  If you press B again the process will be repeated until you reset the robot.
   */
    //sensor_printing();
	
	//stop();
	//unsigned int x = read_battery_millivolts_3pi();
	//clear();
	//printf("U=%u", x);
	//wait_for_button(BUTTON_A);
	//delay_ms(1000);
	
	//while(1)
	//{
	//	follow_line_2();
	//}
	
	while(1){
		//uint8_t result = 0;
		//uint8_t barcode = 0;
		//uint8_t sh_code = 0;
	
		//result = readBarcode(&barcode, &sh_code);
	
		//clear();
		//printf("BC=%u\n", barcode);
		//printf("SH=%u", sh_code);
		//stop();
		//wait_for_button(BUTTON_A);
		//delay_ms(1000);
		
		clear();
		
		uint8_t sh_nbr = 0;
		readSH(&sh_nbr);
		
		clear();
		printf("SH=%u", sh_nbr);
		stop();
		wait_for_button(BUTTON_A);
		delay_ms(1000);
		
		
		//int speed = 50;
		//int p_gain_inv = 30;
		//int i_gain_inv = 10000;
		//follow_line_2_init(speed, p_gain_inv, i_gain_inv);
		//follow_line_2();
	}
  }
  else if(buttonPressed == BUTTON_B)
  {
	/* Run the challenge */
    /* When displayed "Press ABC" -> press button B to enter this function. */
    /* Add you application software to this function in order to solve the challenge */
    
	//run_orienteering(0);
	
	const int allowedReads[6] = {(int)0x20, (int)0x70, (int)0x00, (int)0x20, (int)0xA0, (int)0x28};
	const int N_allowed = 6;
	
	//uint16_t line;
	uint16_t symbIdent = 0;
	uint8_t finishedFlag;
	uint8_t i;
	uint8_t j = 0;
	uint16_t N_samp = 20; //max 64 if 16 bit...
	unsigned int values[5];
	unsigned int sens_sum[5] = {0, 0, 0, 0, 0};
	unsigned int treshold[5] = {N_samp*400, N_samp*700, N_samp*800, N_samp*700, N_samp*400};
	uint8_t mask;
	uint8_t readByte = 0;
	uint8_t readByte_temp = 0;
	uint8_t lastByte = 0;
	
	uint8_t read_bc = 0;
	uint8_t read_sh_bc = 0;
	
	uint8_t bc_index = 0;
	uint8_t sh_bc_index = 0;
	
	uint8_t bc = 0;
	uint8_t sh_bc = 0;
	
	//char last_str[] = {'T',':','0','0','0'};
	//char read_str[] = {'T',':','0','0','0'};
		
	uint16_t speed = 50;
	uint16_t p_gain_inv = 30;
	uint16_t i_gain_inv = 10000;

	follow_line_2_init(speed, p_gain_inv, i_gain_inv);
	
	while(!finishedFlag){
		//line = read_line(values,IR_EMITTERS_ON);
		//follow_line(line);
		follow_line_2();
		
		//(void)sprintf(last_str, "0:%03u", lastByte);
		//clear();
		//printf("0:%03u\n", lastByte);
		//printf("1:%03u", readByte);
		//(void)sprintf(read_str, "1:%03u", readByte);
		
		//print_two_messages(last_str, read_str);
		//wait_for_button(BUTTON_B);
		//delay_ms(1000);
		
		(void)read_line(values,IR_EMITTERS_ON);
		for(i=0; i<5; i++){
			sens_sum[i] = sens_sum[i] + values[i];
		}
		j += 1;
		if(j >= N_samp){
			// transform to byte
			readByte_temp = 0;
			for(i=0; i<5; i++){
				if(sens_sum[i] > treshold[i]){
					//clear();
					//printf("%u\n", i);
					//delay_ms(1000);
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
		if(readByte == lastByte){
			;
			//stop();
			//clear();
			//printf("0:%03u\n", lastByte);
			//printf("1:%03u", readByte);
			//printf("Hej\n");
			//wait_for_button(BUTTON_B);
			//delay_ms(1000);
		}
		else{ //new byte: we have 2 unique
			symbIdent = ((uint16_t)lastByte <<8) | readByte;
			//clear();
			//printf("%u\n", symbIdent);
			//delay_ms(1000);
			//stop();
			//clear();
			//printf("0:%03u\n", lastByte);
			//printf("1:%03u", readByte);
			//wait_for_button(BUTTON_B);
			//delay_ms(1000);
			if(symbIdent == EV_BC_START){
				stop();
				print_two_messages("EV:", "BC_START");	
				delay_ms(1000);
				if(read_bc == 1){
					read_bc = 0;
					read_sh_bc = 1;
				}
				else{
					read_bc = 1;	
				}
			}
			else if(symbIdent == EV_BC_STOP){
				stop();
				print_two_messages("EV:", "BC_STOP");
				delay_ms(1000);
				read_bc = 0;
				read_sh_bc = 0;
			}
			else if((symbIdent == EV_LEFT_BIT_HIGH_1) || (symbIdent == EV_LEFT_BIT_HIGH_2)){
				stop();
				print_two_messages("EV:", "LEFTBIT");
				delay_ms(1000);
				if(read_bc == 1){
					mask = 1;
					mask <<= bc_index;
					bc |= mask;
					bc_index += 1;
				}
				else if(read_sh_bc == 1){
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
				stop();
				print_two_messages("EV:", "RIGHTBIT");
				delay_ms(1000);
				if(read_bc == 1){
					bc_index += 1;
				}
				else if(read_sh_bc == 1){
					sh_bc_index += 1;
				}
				else{
					;
				}
			}
			else{
				stop();
				clear();
				print_two_messages("EV:", "Fail");
				delay_ms(1000);
				//wait_for_button(BUTTON_B);
				clear();
				//printf("0:%03u\n", lastByte);
				//printf("1:%03u", readByte);
				//wait_for_button(BUTTON_B);
				delay_ms(1000);
			}
			//delay_ms(1000);
			lastByte = readByte;
		}
	}
		
  }
  else // BUTTON_C
  {
	/* Run the robot for a short while and record the values measured by the sensors  */
	/* When displayed "Press ABC" -> press button C to enter this function.
	This function will move the robot forward along the line until the following condition is fulfilled:
	Sensor 1 and 3 measures a value larger than 350
	OR 
	Sensor 0 and 4 measures a value larger than 250.
    Then it will continue forward for approx. 3-4 cm (depending on your speed in "follow_line" function)
	and the values measured by the sensors will be recorded to a buffer. When the distance was traveled 
	the message "Buf full Press A" will come up. If you press A the recorded data will be transmitted over 
	the serial port to a terminal software running on the PC while the message "Printing" will be displayed.
    Then the message "Resend Press A" will be displayed. If you press button A the same buffer will be 
    retransmitted to the PC terminal. For more information related to RS 232 communication setup please
    refer to the user's guide.
    */
    //record_bonus_marker();
	while(1){
		uint8_t result = 0;
		uint8_t barcode = 0;
		uint8_t sh_code = 0;
		
		result = readBarcode(&barcode, &sh_code);
		
		clear();
		printf("BC=%u\n", barcode);
		printf("SH=%u", sh_code);
		stop();
		wait_for_button(BUTTON_A);
		delay_ms(1000);
	}
  }
  stop_with_message("END");

}

// -----------------------------------------------------------------
// -----------------------------------------------------------------
